
#include "libpdr.h"

namespace pdr {

#define PARSE_DATA(data)                            \
    tinyxml2::XMLDocument doc;                      \
    doc.Parse(data);                                \
    auto root = doc.FirstChildElement("s:Envelope") \
                    ->FirstChildElement("s:Body")   \
                    ->FirstChildElement(std::string{"u:" + action}.c_str())
#define GET_VAR(var, key) \
    tinyxml2::XMLElement* var = root->FirstChildElement(key)
#define DEFAULT_RES() generateXMLResponse(self->getName(), action, {})

RendererService::RendererService(const std::string& name, int version,
                                 const std::string& xml)
    : name(name), version(version) {
    doc.Parse(xml.c_str());
    auto scpd = doc.FirstChildElement("scpd");

    // 解析 Action 列表
    auto actionList = scpd->FirstChildElement("actionList");
    for (auto item = actionList->FirstChildElement("action"); item;
         item      = item->NextSiblingElement("action")) {
        std::string actionName = item->FirstChildElement("name")->GetText();
        std::vector<ActionArgumentItem> inList;
        std::vector<ActionArgumentItem> outList;
        auto argumentList = item->FirstChildElement("argumentList");
        for (auto argument      = argumentList->FirstChildElement("argument");
             argument; argument = argument->NextSiblingElement("argument")) {
            std::string argumentName =
                argument->FirstChildElement("name")->GetText();
            std::string direction =
                argument->FirstChildElement("direction")->GetText();
            std::string state =
                argument->FirstChildElement("relatedStateVariable")->GetText();
            if (direction == "in") {
                inList.emplace_back(argumentName, state);
            } else {
                outList.emplace_back(argumentName, state);
            }
        }
        actionTable[actionName] = {actionName, inList, outList};
    }

    // 解析 State 表
    auto serviceStateTable = scpd->FirstChildElement("serviceStateTable");
    for (auto item  = serviceStateTable->FirstChildElement("stateVariable");
         item; item = item->NextSiblingElement("stateVariable")) {
        bool eventSending     = item->Attribute("sendEvents", "yes");
        std::string dataType  = item->FirstChildElement("dataType")->GetText();
        std::string stateName = item->FirstChildElement("name")->GetText();
        if (dataType == "string") {
            stateTable[stateName]    = {StateType::STRING, stateName,
                                        eventSending};
            auto defaultValueElement = item->FirstChildElement("defaultValue");
            if (defaultValueElement)
                stateTable[stateName].value = defaultValueElement->GetText();
        } else if (dataType == "ui4" || dataType == "ui2" || dataType == "i4" ||
                   dataType == "i2") {
            stateTable[stateName]       = {StateType::NUMBER, stateName,
                                           eventSending};
            stateTable[stateName].value = 0;
        } else if (dataType == "boolean") {
            stateTable[stateName] = {StateType::BOOL, stateName, eventSending};
            stateTable[stateName].value = false;
        } else {
            printf("Find unsupported type: %s\n", dataType.c_str());
        }
    }

    // 设置播放器状态
    eventSubscribeID =
        PLAYER_EVENT.subscribe([this](const std::string& event, void* data) {
            if (stateTable.count(event) == 0) return;
            auto& stateItem = stateTable[event];
            switch (stateItem.type) {
                case StateType::STRING:
                    stateItem.value = std::string{(char*)data};
                    break;
                case StateType::NUMBER:
                    stateItem.value = *static_cast<int*>(data);
                    break;
                case StateType::BOOL:
                    stateItem.value = *static_cast<bool*>(data);
                    break;
            }
        });
}

RendererService::~RendererService() {
    PLAYER_EVENT.unsubscribe(eventSubscribeID);
}

int RendererService::getVersion() const { return version; }

std::string RendererService::getName() const { return name; }

std::string RendererService::request(const std::string& service,
                                     const std::string& action,
                                     const std::string& data) {
    if (functionMap.count(action) == 0) {
        return RendererService::dummyRequest(this, service, action);
    }
    return functionMap[action](this, action, data);
}

std::string RendererService::dummyRequest(RendererService* self,
                                          const std::string& name,
                                          const std::string& actionName) {
    if (self->actionTable.count(actionName) == 0) {
        return generateXMLResponse(name, actionName, {});
    }

    auto& action = self->actionTable[actionName];
    std::unordered_map<std::string, std::string> res;
    for (auto& arg : action.outList) {
        res[arg.name] = self->stateTable[arg.state].getValue();
    }
    return generateXMLResponse(name, actionName, res);
}

std::string RendererService::getString() {
    tinyxml2::XMLPrinter printer;
    doc.Accept(&printer);
    return std::string{printer.CStr()};
}

std::string RendererService::generateXMLResponse(
    const std::string& service, const std::string& action,
    const std::unordered_map<std::string, std::string>& res) {
    tinyxml2::XMLDocument doc;

    // 创建根元素
    tinyxml2::XMLElement* root = doc.NewElement("s:Envelope");
    root->SetAttribute("xmlns:s", "http://schemas.xmlsoap.org/soap/envelope/");
    root->SetAttribute("s:encodingStyle",
                       "http://schemas.xmlsoap.org/soap/encoding/");
    doc.InsertFirstChild(root);

    // 创建Body元素
    tinyxml2::XMLElement* body = doc.NewElement("s:Body");
    root->InsertEndChild(body);

    // 创建Response元素
    std::string namespaceURI = "urn:schemas-upnp-org:service:" + service + ":1";
    tinyxml2::XMLElement* response = doc.NewElement(namespaceURI.c_str());
    response->SetName(("u:" + action + "Response").c_str());
    response->SetAttribute("xmlns:u", namespaceURI.c_str());
    body->InsertEndChild(response);

    // 添加属性和值
    for (const auto& entry : res) {
        tinyxml2::XMLElement* prop = doc.NewElement(entry.first.c_str());
        prop->SetText(entry.second.c_str());
        response->InsertEndChild(prop);
    }

    // 将XML文档转换为字符串
    tinyxml2::XMLPrinter printer;
    doc.Print(&printer);
    std::string xmlString = printer.CStr();

    return xmlString;
}

/// AVTransport

RendererServiceAVTransport::RendererServiceAVTransport()
    : RendererService("AVTransport", 1, AVTransport) {
    functionMap["SetAVTransportURI"] = SetAVTransportURI;
    functionMap["Stop"]              = Stop;
    functionMap["Play"]              = Play;
    functionMap["Pause"]             = Pause;
    functionMap["Seek"]              = Seek;
}

std::string RendererServiceAVTransport::SetAVTransportURI(ACTION_PARAMS) {
    PARSE_DATA(data.c_str());

    // video/audio link
    GET_VAR(CurrentURI, "CurrentURI");
    DLNA_EVENT.fire("CurrentURI", (void*)CurrentURI->GetText());
    ((RendererServiceAVTransport*)self)->stateTable["CurrentTrackURI"].value =
        CurrentURI->GetText();

    // title
    GET_VAR(CurrentURIMetaData, "CurrentURIMetaData");
    ((RendererServiceAVTransport*)self)
        ->stateTable["CurrentTrackMetaData"]
        .value = CurrentURIMetaData->GetText();
    tinyxml2::XMLDocument metaData;
    metaData.Parse(CurrentURIMetaData->GetText());
    if (!metaData.Error()) {
        auto title = metaData.FirstChildElement("DIDL-Lite")
                         ->FirstChildElement("item")
                         ->FirstChildElement("dc:title");
        if (title && title->GetText())
            DLNA_EVENT.fire("CurrentURIMetaData", (void*)title->GetText());
    }
    return DEFAULT_RES();
}

std::string RendererServiceAVTransport::Stop(ACTION_PARAMS) {
    DLNA_EVENT.fire("Stop", nullptr);
    return DEFAULT_RES();
}

std::string RendererServiceAVTransport::Play(ACTION_PARAMS) {
    DLNA_EVENT.fire("Play", nullptr);
    return DEFAULT_RES();
}

std::string RendererServiceAVTransport::Pause(ACTION_PARAMS) {
    DLNA_EVENT.fire("Pause", nullptr);
    return DEFAULT_RES();
}

std::string RendererServiceAVTransport::Seek(ACTION_PARAMS) {
    PARSE_DATA(data.c_str());
    GET_VAR(Target, "Target");

    DLNA_EVENT.fire("Seek", (void*)Target->GetText());
    return DEFAULT_RES();
}

/// RendererServiceRenderingControl

RendererServiceRenderingControl::RendererServiceRenderingControl()
    : RendererService("RenderingControl", 1, RenderingControl) {
    functionMap["SetVolume"] = SetVolume;
}

std::string RendererServiceRenderingControl::SetVolume(ACTION_PARAMS) {
    PARSE_DATA(data.c_str());
    GET_VAR(Target, "DesiredVolume");
    DLNA_EVENT.fire("SetVolume", (void*)Target->GetText());
    return DEFAULT_RES();
}

/// RendererServiceConnectionManager

RendererServiceConnectionManager::RendererServiceConnectionManager()
    : RendererService("ConnectionManager", 1, ConnectionManager) {}
};  // namespace pdr