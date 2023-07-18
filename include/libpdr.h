
#pragma once
#include <string>
#include <vector>
#include <list>
#include <thread>
#include <unordered_map>
#include <functional>
#include <memory>

#include <tinyxml2.h>

namespace pdr {
    /// Event

    typedef std::function<void(std::string, void*)> pdrEvent;
    typedef std::list<pdrEvent> CallbacksList;
    typedef CallbacksList::iterator Subscription;

#define DLNA_EVENT pdr::Event::instance()

    class Event {
    public:
        Subscription subscribe(const pdrEvent& event) {
            subscribers.push_back(event);
            return --subscribers.end();
        }

        void unsubscribe(Subscription sub) {
            if (subscribers.size() > 0)
                subscribers.erase(sub);
        }

        void fire(std::string event, void* data) {
            for (const auto& subscriber : subscribers) {
                subscriber(event, data);
            }
        }

        static Event& instance()
        {
            static Event instance;
            return instance;
        }

    private:
        CallbacksList subscribers;
    };

    /// Protocol

    class RendererService {
    public:
        RendererService() = default;

        RendererService(const std::string& name, int version, const std::string& xml);

        std::string getString();

        int getVersion() const;

        std::string getName() const;

        std::string request(const std::string& name, const std::string& action, const std::string& data);

        static std::string dummyRequest(const std::string& name, const std::string& action);

    protected:
        std::string name;
        int version = 1;
        tinyxml2::XMLDocument doc;
        std::unordered_map<std::string, std::function<std::string(RendererService*, std::string, std::string)>> functionMap;

        static std::string generateXMLResponse(const std::string& service,
                                               const std::string& action,
                                               const std::unordered_map<std::string, std::string>& res);
    };

    class RendererServiceAVTransport: public RendererService {
    public:
        RendererServiceAVTransport();

        static std::string SetAVTransportURI(RendererService* self, const std::string& action, const std::string& data);

        static std::string Stop(RendererService* self, const std::string& action, const std::string& data);
    };

    class RendererServiceRenderingControl: public RendererService {
    public:
        RendererServiceRenderingControl();
    };

    class RendererServiceConnectionManager: public RendererService {
    public:
        RendererServiceConnectionManager();
    };
    /// SOAP

    class RendererDevice {
    public:
        RendererDevice();

        std::string getString(const std::string& type = "");

        void print();

        void updateDeviceInfo(const std::string& name, const std::string& uuid,
                              const std::string& manufacturer = "", const std::string& manufacturerUrl = "",
                              const std::string& modelDesc = "", const std::string& modelName = "",
                              const std::string& modelNumber = "", const std::string& modelUrl = "",
                              const std::string& serialNum = "");

        void setDeviceInfo(const std::string& key, const std::string& value);

        void addService(std::shared_ptr<RendererService> service);

        std::unordered_map<std::string, std::shared_ptr<RendererService>>& getServiceList();

        std::string parseRequest(const std::string& service, const std::string& action, const std::string& data);

    protected:
        tinyxml2::XMLDocument doc;
        tinyxml2::XMLElement* device;
        tinyxml2::XMLElement* deviceType;
        tinyxml2::XMLElement* serviceList;
        std::unordered_map<std::string, std::shared_ptr<RendererService>> serviceMap;

        void addServiceXML(const std::string& name, int version);
    };

    class SOAP {
    public:
        SOAP() = default;

        void start(const std::string& url="http://0.0.0.0:8000");

        void stop(bool wait);

        RendererDevice& getDevice();

        ~SOAP();

    protected:
        std::thread runningThread;
        bool running = false;
        pdr::RendererDevice device;

        static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data);
    };

    /// SSDP

    class SSDPService {
    public:
        SSDPService() = default;

        // uuid example: "uuid:00000000-0000-0000-0000-000000000000"
        // scope example: "upnp:rootdevice", "urn:schemas-upnp-org:device", "urn:schemas-upnp-org:service"
        // name example: "MediaRenderer:1", "ConnectionManager:1"
        SSDPService(const std::string uuid, const std::string &scope, const std::string &name, const std::string& location);

        std::string getST() const {
            if (scope.empty() && name.empty()) return uuid;
            if (name.empty()) return scope;
            return scope + ":" + name;
        }

        std::string getUSN() const {
            if (scope.empty()) return uuid;
            if (name.empty()) return uuid + "::" + scope;
            return uuid + "::" + scope + ":" + name;
        }

        std::string uuid, scope, name;
        std::string location, serverName = "libpdr/1.0", cacheControl = "max-age=1800";
    };

    typedef std::vector<SSDPService> SSDPServiceList;
    typedef std::unordered_map<std::string, SSDPService> SSDPServiceMap;

    class SSDP {
    public:
        static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data);

        void registerServices(const SSDPServiceList& service);

        SSDPServiceMap& getServices();

        void start(const std::string& ip = "udp://0.0.0.0:1900");

        void stop(bool wait = false);

        ~SSDP();

    protected:
        std::thread ssdpThread;
        bool running = false;
        SSDPServiceMap services;
    };

    /// DLNA

    class DLNA {
    public:
        DLNA(const std::string ip, size_t port = 9958, const std::string& uuid = "");

        void setDeviceInfo(const std::string& key, const std::string& value);

        void start();

        void stop(bool wait = true);
    protected:
        std::string uuid;
        size_t port;
        pdr::SSDP ssdp;
        pdr::SOAP soap;

        bool running = false;
        std::thread runningThread;
    };

}
