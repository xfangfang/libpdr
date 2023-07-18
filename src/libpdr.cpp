
#include <stdexcept>
#include "libpdr.h"

namespace pdr {
    DLNA::DLNA(const std::string ip, size_t port, const std::string& uuid):uuid(uuid), port(port) {
        SSDPServiceList services;

        char temp[100];
        snprintf(temp, sizeof(temp), "http://%s:%d/description.xml", ip.c_str(), (int)port);
        std::string location{temp};

        ssdp.registerServices({{uuid, "upnp:rootdevice", "", location},
                               {uuid, "","", location},
                               {uuid, "urn:schemas-upnp-org:device", "MediaRenderer:1", location},
                               {uuid, "urn:schemas-upnp-org:service", "AVTransport:1", location},
                               {uuid, "urn:schemas-upnp-org:service", "RenderingControl:1", location},
                               {uuid, "urn:schemas-upnp-org:service", "ConnectionManager:1", location},
                               });
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
}