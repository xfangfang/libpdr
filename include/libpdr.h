
#pragma once
#include <string>
#include <vector>
#include <list>
#include <thread>
#include <unordered_map>
#include <functional>
#include <memory>
#include <variant>

#include <tinyxml2.h>

namespace pdr {
/// Utils

std::string gmtTime();

/// Event

typedef std::function<void(std::string, void*)> pdrEvent;
typedef std::list<pdrEvent> CallbacksList;
typedef CallbacksList::iterator Subscription;

#define DLNA_EVENT pdr::Event::dlnaEvent()
#define PLAYER_EVENT pdr::Event::playerEvent()
#define DLNA_ERROR(m) Event::showError(m)

class Event {
public:
    Subscription subscribe(const pdrEvent& event);

    void unsubscribe(Subscription sub);

    void fire(std::string event, void* data);

    static Event& dlnaEvent();

    static Event& playerEvent();

    static void showError(std::string msg, bool withErrno = true);

private:
    CallbacksList subscribers;
};

/// Protocol

enum StateType { STRING, NUMBER, BOOL };

struct StateItem {
    StateItem() = default;
    StateItem(StateType type, const std::string& name, bool sendEvents)
        : type(type), name(name), sendEvents(sendEvents) {}

    std::string getValue() {
        switch (type) {
            case StateType::STRING: {
                auto data = std::get_if<std::string>(&value);
                if (data) return *data;
                break;
            }
            case StateType::NUMBER: {
                auto data = std::get_if<int>(&value);
                if (data) return std::to_string(*data);
                break;
            }
            case StateType::BOOL: {
                auto data = std::get_if<bool>(&value);
                if (data) return std::to_string(*data);
                break;
            }
        }
        return "";
    }

    StateType type;
    std::string name;
    bool sendEvents;
    std::variant<std::string, int, unsigned int, bool> value;
};

struct ActionArgumentItem {
    ActionArgumentItem() = default;
    ActionArgumentItem(const std::string& name, const std::string& state)
        : name(name), state(state){};
    std::string name;
    std::string state;
};

struct ActionItem {
    ActionItem() = default;
    ActionItem(const std::string& name, std::vector<ActionArgumentItem> in,
               std::vector<ActionArgumentItem> out)
        : name(name), inList(std::move(in)), outList(std::move(out)){};
    std::string name;
    std::vector<ActionArgumentItem> inList;
    std::vector<ActionArgumentItem> outList;
};

#define ACTION_PARAMS \
    RendererService *self, const std::string &action, const std::string &data
class RendererService {
public:
    RendererService() = default;

    RendererService(const std::string& name, int version,
                    const std::string& xml);

    virtual ~RendererService();

    std::string getString();

    int getVersion() const;

    std::string getName() const;

    std::string request(const std::string& service, const std::string& action,
                        const std::string& data);

    static std::string dummyRequest(RendererService* self,
                                    const std::string& name,
                                    const std::string& action);

    static std::string generateXMLResponse(
        const std::string& service, const std::string& action,
        const std::unordered_map<std::string, std::string>& res);

protected:
    std::string name;
    int version = 1;
    tinyxml2::XMLDocument doc;
    std::unordered_map<
        std::string,
        std::function<std::string(RendererService*, std::string, std::string)>>
        functionMap;
    std::unordered_map<std::string, StateItem> stateTable;
    std::unordered_map<std::string, ActionItem> actionTable;
    pdr::Subscription eventSubscribeID;
};

class RendererServiceAVTransport : public RendererService {
public:
    RendererServiceAVTransport();

    static std::string SetAVTransportURI(ACTION_PARAMS);

    static std::string Stop(ACTION_PARAMS);

    static std::string Play(ACTION_PARAMS);

    static std::string Pause(ACTION_PARAMS);

    static std::string Seek(ACTION_PARAMS);

protected:
    static const std::string AVTransport;
};

class RendererServiceRenderingControl : public RendererService {
public:
    RendererServiceRenderingControl();

    static std::string SetVolume(ACTION_PARAMS);

protected:
    static const std::string RenderingControl;
};

class RendererServiceConnectionManager : public RendererService {
public:
    RendererServiceConnectionManager();

protected:
    static const std::string ConnectionManager;
};

/// SOAP

class RendererDevice {
public:
    RendererDevice();

    std::string getString(const std::string& type = "");

    void print();

    void updateDeviceInfo(const std::string& name, const std::string& uuid,
                          const std::string& manufacturer    = "",
                          const std::string& manufacturerUrl = "",
                          const std::string& modelDesc       = "",
                          const std::string& modelName       = "",
                          const std::string& modelNumber     = "",
                          const std::string& modelUrl        = "",
                          const std::string& serialNum       = "");

    void setDeviceInfo(const std::string& key, const std::string& value);

    void addService(std::shared_ptr<RendererService> service);

    std::unordered_map<std::string, std::shared_ptr<RendererService>>&
    getServiceList();

    std::string parseRequest(const std::string& service,
                             const std::string& action,
                             const std::string& data);

protected:
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* device;
    tinyxml2::XMLElement* deviceType;
    tinyxml2::XMLElement* serviceList;
    std::unordered_map<std::string, std::shared_ptr<RendererService>>
        serviceMap;

    void addServiceXML(const std::string& name, int version);
};

class SOAP {
public:
    SOAP() = default;

    void start(const std::string& url = "http://0.0.0.0:8000");

    void stop(bool wait);

    RendererDevice& getDevice();

    ~SOAP();

protected:
    std::thread runningThread;
    bool running = false;
    pdr::RendererDevice device;
    struct mg_mgr* mgr = nullptr;
    struct mg_connection* connection = nullptr;

    static void fn(struct mg_connection* c, int ev, void* ev_data);
};

/// SSDP

class SSDPService {
public:
    SSDPService() = default;

    // uuid example: "uuid:00000000-0000-0000-0000-000000000000"
    // scope example: "upnp:rootdevice", "urn:schemas-upnp-org:device", "urn:schemas-upnp-org:service"
    // name example: "MediaRenderer:1", "ConnectionManager:1"
    // location example: ":9958/description.xml"
    // ipList example: ["192.168.3.224", "*.*.*.*"]
    SSDPService(const std::string& uuid, const std::string& scope,
                const std::string& name, const std::string& location,
                const std::string& ip);

    [[nodiscard]] std::string getST() const;

    [[nodiscard]] std::string getUSN() const;

    [[nodiscard]] std::string getBroadcastAddr() const;

    std::string ip, uuid, scope, name, location,
        serverName   = "System/1.0 UPnP/1.0 libpdr/1.0",
        cacheControl = "max-age=1800";
};

typedef std::vector<SSDPService> SSDPServiceList;
typedef std::unordered_map<std::string, SSDPService> SSDPServiceMap;

class SSDP {
public:
    static void fn(struct mg_connection* c, int ev, void* ev_data);

    void registerServices(const SSDPServiceList& service);

    SSDPServiceMap& getServices();

    void start(const std::string& ip = "udp://0.0.0.0:1900");

    void stop(bool wait = false);

    void sendNotify(const std::string& NTS);

    void setIP(const std::string& ip);

    ~SSDP();

protected:
    std::thread ssdpThread;
    bool running       = false;
    bool sendBroadcast = false;
    SSDPServiceMap services;
    struct mg_mgr* mgr = nullptr;
    struct mg_connection* connection = nullptr;  // listen on: 0.0.0.0:1900
    struct mg_connection* notifyConnection = nullptr;
    std::string ip;
};

/// DLNA

class DLNA {
public:
    DLNA(const std::string& ip, size_t port = 9958,
         const std::string& uuid = "");

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

}  // namespace pdr
