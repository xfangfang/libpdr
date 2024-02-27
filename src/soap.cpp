
#include <mongoose.h>

#include "libpdr.h"

namespace pdr {

static std::string xmlHeader() {
    return "Content-Type: text/xml;charset=utf-8\r\n"
           "Server: System/1.0 UPnP/1.0 libpdr/1.0\r\n"
           "Allow: GET, HEAD, POST, SUBSCRIBE, UNSUBSCRIBE\r\n"
           "Date: " +
           gmtTime() + "\r\n";
}

const std::string descriptionXML = R"xml(<?xml version="1.0" encoding="UTF-8"?>
<root
    xmlns:dlna="urn:schemas-dlna-org:device-1-0"
    xmlns="urn:schemas-upnp-org:device-1-0">
    <specVersion>
        <major>1</major>
        <minor>0</minor>
    </specVersion>
    <device>
        <deviceType>urn:schemas-upnp-org:device:MediaRenderer:1</deviceType>
        <dlna:X_DLNADOC xmlns:dlna="urn:schemas-dlna-org:device-1-0">DMR-1.50</dlna:X_DLNADOC>
        <serviceList>
        </serviceList>
    </device>
</root>
)xml";

/// Device

RendererDevice::RendererDevice() {
    doc.Parse(descriptionXML.c_str());
    auto root   = doc.FirstChildElement("root");
    device      = root->FirstChildElement("device");
    deviceType  = device->FirstChildElement("deviceType");
    serviceList = device->FirstChildElement("serviceList");

    updateDeviceInfo("Portable DLNA Renderer",
                     "uuid:00000000-0000-0000-0000-000000000000", "libpdr",
                     "https://github.com/xfangfang/libpbr",
                     "Portable DLNA Renderer", "libpdr", "1.0",
                     "https://github.com/xfangfang/libpbr", "1024");

    addService(std::make_shared<RendererServiceAVTransport>());
    addService(std::make_shared<RendererServiceRenderingControl>());
    addService(std::make_shared<RendererServiceConnectionManager>());
}

void RendererDevice::updateDeviceInfo(
    const std::string& name, const std::string& uuid,
    const std::string& manufacturer, const std::string& manufacturerUrl,
    const std::string& modelDesc, const std::string& modelName,
    const std::string& modelNumber, const std::string& modelUrl,
    const std::string& serialNum) {
    setDeviceInfo("serialNumber", serialNum);
    setDeviceInfo("modelURL", modelUrl);
    setDeviceInfo("modelNumber", modelNumber);
    setDeviceInfo("modelName", modelName);
    setDeviceInfo("modelDescription", modelDesc);
    setDeviceInfo("manufacturerURL", manufacturerUrl);
    setDeviceInfo("manufacturer", manufacturer);
    setDeviceInfo("friendlyName", name);
    setDeviceInfo("UDN", uuid);
}

void RendererDevice::setDeviceInfo(const std::string& key,
                                   const std::string& value) {
    if (key.empty()) return;
    auto e = device->FirstChildElement(key.c_str());
    if (e) {
        e->SetText(value.c_str());
    } else {
        e = doc.NewElement(key.c_str());
        e->SetText(value.c_str());
        device->InsertAfterChild(deviceType, e);
    }
}

void RendererDevice::addService(std::shared_ptr<RendererService> service) {
    this->addServiceXML(service->getName(), service->getVersion());
    serviceMap[service->getName()] = service;
}

void RendererDevice::addServiceXML(const std::string& name, int version) {
    auto service = doc.NewElement("service");
    char buf[100];

    auto e1 = doc.NewElement("serviceType");
    snprintf(buf, sizeof(buf), "urn:schemas-upnp-org:service:%s:%d",
             name.c_str(), version);
    e1->SetText(buf);
    service->InsertEndChild(e1);

    auto e2 = doc.NewElement("serviceId");
    snprintf(buf, sizeof(buf), "urn:upnp-org:serviceId:%s", name.c_str());
    e2->SetText(buf);
    service->InsertEndChild(e2);

    auto e3 = doc.NewElement("controlURL");
    snprintf(buf, sizeof(buf), "%s/action", name.c_str());
    e3->SetText(buf);
    service->InsertEndChild(e3);

    auto e4 = doc.NewElement("eventSubURL");
    snprintf(buf, sizeof(buf), "%s/event", name.c_str());
    e4->SetText(buf);
    service->InsertEndChild(e4);

    auto e5 = doc.NewElement("SCPDURL");
    snprintf(buf, sizeof(buf), "%s.xml", name.c_str());
    e5->SetText(buf);
    service->InsertEndChild(e5);

    serviceList->InsertEndChild(service);
}

std::string RendererDevice::getString(const std::string& type) {
    if (type.empty()) {
        tinyxml2::XMLPrinter printer;
        doc.Accept(&printer);
        return std::string{printer.CStr()};
    }
    if (serviceMap.count(type) != 0) {
        return serviceMap[type]->getString();
    }
    return "";
}

void RendererDevice::print() {
    auto xml = getString();
    printf("xml: %s;\n", xml.c_str());
}

std::unordered_map<std::string, std::shared_ptr<RendererService>>&
RendererDevice::getServiceList() {
    return serviceMap;
}

std::string RendererDevice::parseRequest(const std::string& service,
                                         const std::string& action,
                                         const std::string& data) {
    if (serviceMap.count(service) == 0) {
        MG_ERROR(("Unknown service: %s/%s", service.c_str(), action.c_str()));
        return RendererService::generateXMLResponse(service, action, {});
    }
    return serviceMap[service]->request(service, action, data);
}

/// SOAP

static void printClientIP(struct mg_connection* c) {
    char client_ip[INET6_ADDRSTRLEN];
    if (c->rem.is_ip6) {
        inet_ntop(AF_INET6, &(c->rem.ip), client_ip, INET6_ADDRSTRLEN);
    } else {
        inet_ntop(AF_INET, &(c->rem.ip), client_ip, INET6_ADDRSTRLEN);
    }
    printf("Client IP: %s:%d\n", client_ip, c->rem.port);
}

static void printHeader(mg_http_message* hm) {
    size_t i, max = sizeof(hm->headers) / sizeof(hm->headers[0]);
    for (i = 0; i < max && hm->headers[i].name.len > 0; i++) {
        struct mg_str *k = &hm->headers[i].name, *v = &hm->headers[i].value;
        printf("\t%.*s -> %.*s\n", (int)k->len, k->ptr, (int)v->len, v->ptr);
    }
    printf("\n\t%.*s\n", (int)hm->body.len, hm->body.ptr);
}

inline void parsePostAction(mg_http_message* hm, std::string& service,
                            std::string& action) {
    size_t i, max = sizeof(hm->headers) / sizeof(hm->headers[0]);
    for (i = 0; i < max && hm->headers[i].name.len > 0; i++) {
        struct mg_str *k = &hm->headers[i].name, *v = &hm->headers[i].value;
        if (mg_vcasecmp(k, "SOAPACTION") == 0) {
            // v: "urn:schemas-upnp-org:service:AVTransport:1#SetAVTransportURI"
            size_t count = 0, last = 0;
            for (size_t index = 0; index < v->len; index++) {
                if (v->ptr[index] == '#' && index + 1 < v->len) {
                    action =
                        std::string{v->ptr + index + 1, v->len - index - 2};
                    break;
                }
                if (v->ptr[index] == ':') {
                    count++;
                    if (count == 4) {
                        service =
                            std::string{v->ptr + last + 1, index - last - 1};
                    }
                    last = index;
                }
            }
            return;
        }
    }
}

void SOAP::fn(struct mg_connection* c, int ev, void* ev_data) {
    auto soap = static_cast<SOAP*>(c->fn_data);
    if (ev == MG_EV_HTTP_MSG) {
        auto hm = static_cast<mg_http_message*>(ev_data);
        MG_INFO(("Got a HTTP request"));
        printf("Method %.*s: %.*s\n", (int)hm->method.len, hm->method.ptr,
               (int)hm->uri.len, hm->uri.ptr);
        printClientIP(c);
        printHeader(hm);
        if (mg_vcasecmp(&hm->method, "GET") == 0) {
            if (mg_strcmp(hm->uri, MG_C_STR("/")) == 0) {
                return mg_http_reply(c, 200, "", "Portable DLNA Renderer");
            } else if (mg_strcmp(hm->uri, MG_C_STR("/description.xml")) == 0) {
                return mg_http_reply(c, 200, xmlHeader().c_str(),
                                     soap->device.getString().c_str());
            } else {
                for (auto& service : soap->device.getServiceList()) {
                    std::string link = "/" + service.first + ".xml";
                    if (mg_vcmp(&hm->uri, link.c_str()) == 0) {
                        return mg_http_reply(
                            c, 200, xmlHeader().c_str(),
                            soap->device.getString(service.first).c_str());
                    }
                }
            }
        } else if (mg_vcasecmp(&hm->method, "POST") == 0) {
            try {
                std::string service, action;
                parsePostAction(hm, service, action);
                auto res = soap->device.parseRequest(
                    service, action, std::string{hm->body.ptr, hm->body.len});
                if (!res.empty()) {
                    return mg_http_reply(c, 200, xmlHeader().c_str(),
                                         res.c_str());
                }
            } catch (const std::exception& ex) {
                MG_ERROR(("unknown post request"));
            }
        }
        mg_http_reply(c, 404, "", "HTTP 404");
    }
}

void SOAP::start(const std::string& url) {
    running       = true;
    runningThread = std::thread([this, url]() {
        mgr = new struct mg_mgr;
        mg_mgr_init(mgr);
        mg_wakeup_init(mgr);
        connection = mg_http_listen(mgr, url.c_str(), fn, this);
        if (!connection) {
            DLNA_ERROR("SOAP: Cannot listen to: " + url);
            mg_mgr_free(mgr);
            return;
        }
        while (running) mg_mgr_poll(mgr, 2000);
        mg_mgr_free(mgr);
        delete mgr;
        mgr        = nullptr;
        connection = nullptr;
    });
}

void SOAP::stop(bool wait) {
    if (!running) return;
    running = false;
    if (mgr && connection)
        mg_wakeup(mgr, connection->id, nullptr, 0);
    if (wait && runningThread.joinable()) {
        runningThread.join();
    }
}

SOAP::~SOAP() { stop(true); }

RendererDevice& SOAP::getDevice() { return device; }
};  // namespace pdr