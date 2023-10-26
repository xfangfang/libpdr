
#include <iomanip>
#include <chrono>
#include <sstream>
#include <stdexcept>
#include "libpdr.h"

namespace pdr {

DLNA::DLNA(const std::string& ip, size_t port, const std::string& uuid)
    : uuid(uuid), port(port) {
    SSDPServiceList services;

    char temp[100];
    snprintf(temp, sizeof(temp), "http://%s:%d/description.xml", ip.c_str(),
             (int)port);
    std::string location{temp};

    ssdp.setIP(ip);

    ssdp.registerServices({
        {uuid, "upnp:rootdevice", "", location, ip},
        {uuid, "", "", location, ip},
        {uuid, "urn:schemas-upnp-org:device", "MediaRenderer:1", location, ip},
        {uuid, "urn:schemas-upnp-org:service", "AVTransport:1", location, ip},
        {uuid, "urn:schemas-upnp-org:service", "RenderingControl:1", location, ip},
        {uuid, "urn:schemas-upnp-org:service", "ConnectionManager:1", location, ip},
    });

    setDeviceInfo("UDN", uuid);
}

void DLNA::setDeviceInfo(const std::string& key, const std::string& value) {
    if (running) throw std::logic_error("cannot set info when DLNA is running");
    soap.getDevice().setDeviceInfo(key, value);
}

void DLNA::start() {
    char url[30];
    snprintf(url, sizeof(url), "http://0.0.0.0:%d", (int)port);
    soap.start(url);
    ssdp.start();
    running = true;
}

void DLNA::stop(bool wait) {
    soap.stop(wait);
    ssdp.stop(wait);
    running = false;
}

Subscription Event::subscribe(const pdrEvent& event) {
    subscribers.push_back(event);
    return --subscribers.end();
}

void Event::unsubscribe(Subscription sub) {
    if (subscribers.size() > 0) subscribers.erase(sub);
}

void Event::fire(std::string event, void* data) {
    for (const auto& subscriber : subscribers) {
        subscriber(event, data);
    }
}

Event& Event::dlnaEvent() {
    static Event instance;
    return instance;
}

Event& Event::playerEvent() {
    static Event instance;
    return instance;
}

void Event::showError(std::string msg, bool withErrno) {
    if (withErrno && errno != 0) {
        char ErrnoMsgShareBuf[256];
#ifdef _WIN32
        strerror_s(ErrnoMsgShareBuf, sizeof(ErrnoMsgShareBuf), errno);
#else
        strerror_r(errno, ErrnoMsgShareBuf, sizeof(ErrnoMsgShareBuf));
#endif
        msg += "; ERRNO: " + std::string{ErrnoMsgShareBuf};
    }
    DLNA_EVENT.fire("Error", (void*)std::string{msg}.c_str());
}

/// Utils
std::string gmtTime() {
    auto currentTime = std::chrono::system_clock::now();
    std::time_t currentTimeT =
        std::chrono::system_clock::to_time_t(currentTime);
    std::tm timeInfo{};
#ifdef _WIN32
    gmtime_s(&timeInfo, &currentTimeT);
#else
    gmtime_r(&currentTimeT, &timeInfo);
#endif
    std::stringstream ss;
    ss << std::put_time(&timeInfo, "%a, %d %b %Y %H:%M:%S GMT");
    return ss.str();
}
}  // namespace pdr