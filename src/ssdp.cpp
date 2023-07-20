
#include <mongoose.h>

#include "libpdr.h"

#define FD(c_) ((MG_SOCKET_TYPE)(size_t)(c_)->fd)
#define SSDP_MULTICAST_ADDR "239.255.255.250"  // SSDP组播地址
#define SSDP_PORT 1900

namespace pdr {

SSDPService::SSDPService(const std::string uuid, const std::string &scope,
                         const std::string &name, const std::string &location)
    : uuid(uuid), scope(scope), name(name), location(location) {}

void SSDP::fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
    MG_DEBUG(("%p got event: %d %p %p", c, ev, ev_data, fn_data));

    auto ssdp = static_cast<SSDP *>(fn_data);

    if (ev == MG_EV_READ) {
        struct mg_http_message hm {};
        std::unordered_map<std::string, std::string> headers;
        if (mg_http_parse((const char *)c->recv.buf, c->recv.len, &hm) <= 0)
            return;

        // Discard the content of this response as we expect each SSDP response
        // to generate at most one MG_EV_READ event.
        c->recv.len = 0UL;

        if (mg_strcmp(hm.method, mg_str("M-SEARCH")) != 0) return;
        MG_INFO(("Got a request"));

        size_t i, max = sizeof(hm.headers) / sizeof(hm.headers[0]);
        // Iterate over request headers
        for (i = 0; i < max && hm.headers[i].name.len > 0; i++) {
            struct mg_str *k = &hm.headers[i].name, *v = &hm.headers[i].value;
            headers[std::string{k->ptr, k->len}] = std::string{v->ptr, v->len};
            printf("\t%.*s -> %.*s\n", (int)k->len, k->ptr, (int)v->len,
                   v->ptr);
        }
        printf("\n");

        if (headers.count("ST") == 0) return;
        for (auto &service : ssdp->getServices()) {
            if (service.second.getST() == headers["ST"] ||
                headers["ST"] == "ssdp:all") {
                mg_printf(c,
                          "HTTP/1.1 200 OK\r\n"
                          "USN: %s\r\n"
                          "ST: %s\r\n"
                          "LOCATION: %s\r\n"
                          "EXT:\r\n"
                          "SERVER: %s\r\n"
                          "CACHE-CONTROL: %s\r\n"
                          "DATE: %s\r\n"
                          "\r\n",
                          service.second.getUSN().c_str(),
                          service.second.getST().c_str(),
                          service.second.location.c_str(),
                          service.second.serverName.c_str(),
                          service.second.cacheControl.c_str(),
                          gmtTime().c_str());
                break;
            }
        }
    }
}

void SSDP::registerServices(const SSDPServiceList &service) {
    for (auto &s : service) services[s.getUSN()] = s;
}

SSDPServiceMap &SSDP::getServices() { return services; }

void SSDP::start(const std::string &url) {
    running    = true;
    ssdpThread = std::thread([this, url]() {
        struct mg_mgr mgr {};
        static struct mg_connection *c;
        mg_mgr_init(&mgr);
        c = mg_listen(&mgr, url.c_str(), fn, this);

        if (!c) {
            mg_mgr_free(&mgr);
            DLNA_ERROR("SSDP: Cannot listen to: " + url);
            return;
        }

        // 加入 SSDP 组播
        struct ip_mreq mreq {};
        mreq.imr_multiaddr.s_addr = inet_addr(SSDP_MULTICAST_ADDR);
        mreq.imr_interface.s_addr = htonl(INADDR_ANY);
        if (setsockopt(FD(c), IPPROTO_IP, IP_ADD_MEMBERSHIP,
                       reinterpret_cast<const char *>(&mreq),
                       sizeof(mreq)) < 0) {
            mg_mgr_free(&mgr);
            DLNA_ERROR("SSDP: Cannot join to multicast group");
            return;
        }
        while (running) mg_mgr_poll(&mgr, 200);
        mg_mgr_free(&mgr);
    });
}

void SSDP::stop(bool wait) {
    running = false;
    if (wait && ssdpThread.joinable()) {
        ssdpThread.join();
    }
}

SSDP::~SSDP() { stop(true); }

}  // namespace pdr