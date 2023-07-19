
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
#define DEFAULT_RES() \
    generateXMLResponse(self->getName(), action, {{"InstanceID", "0"}})

RendererService::RendererService(const std::string& name, int version,
                                 const std::string& xml)
    : name(name), version(version) {
    doc.Parse(xml.c_str());
    //        auto scpd = doc.FirstChildElement("scpd");
    //        auto actionList = scpd->FirstChildElement("actionList");
    //        for (auto item = actionList->FirstChildElement("action"); item; item = item->NextSiblingElement("action")) {
    //            std::string name = item->FirstChildElement("name")->GetText();
    //            auto argumentList = item->FirstChildElement("argumentList");
    //            for (auto argument = argumentList->FirstChildElement("argument"); argument; argument = argument->NextSiblingElement("argument")) {
    //
    //            }
    //        }
    //        printf("%s\n", getString());
}

int RendererService::getVersion() const { return version; }

std::string RendererService::getName() const { return name; }

std::string RendererService::request(const std::string& name,
                                     const std::string& action,
                                     const std::string& data) {
    if (functionMap.count(name) == 0) {
        return RendererService::dummyRequest(name, action);
    }
    return functionMap[name](this, name, data);
}

std::string RendererService::dummyRequest(const std::string& name,
                                          const std::string& action) {
    return generateXMLResponse(name, action, {{"InstanceID", "0"}});
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
    response->SetName((action + "Response").c_str());
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
    functionMap["GetTransportInfo"]  = GetTransportInfo;
    functionMap["GetPositionInfo"]   = GetPositionInfo;
}

std::string RendererServiceAVTransport::SetAVTransportURI(ACTION_PARAMS) {
    PARSE_DATA(data.c_str());

    // video/audio link
    GET_VAR(CurrentURI, "CurrentURI");
    DLNA_EVENT.fire("CurrentURI", (void*)CurrentURI->GetText());

    // title
    GET_VAR(CurrentURIMetaData, "CurrentURIMetaData");
    tinyxml2::XMLDocument metaData;
    metaData.Parse(CurrentURIMetaData->GetText());
    if (!metaData.Error()) {
        auto title = metaData.FirstChildElement("DIDL-Lite")
                         ->FirstChildElement("item")
                         ->FirstChildElement("dc:title");
        if (title)
            DLNA_EVENT.fire("CurrentURIMetaData", (void*)title->GetText());
    }

    return DEFAULT_RES();
}

std::string RendererServiceAVTransport::Stop(ACTION_PARAMS) {
    Event::instance().fire("Stop", nullptr);
    return DEFAULT_RES();
}

std::string RendererServiceAVTransport::Play(ACTION_PARAMS) {
    Event::instance().fire("Play", nullptr);
    return DEFAULT_RES();
}

std::string RendererServiceAVTransport::GetTransportInfo(ACTION_PARAMS) {
    return DEFAULT_RES();
}

std::string RendererServiceAVTransport::GetPositionInfo(ACTION_PARAMS) {
    return DEFAULT_RES();
}

/// RendererServiceRenderingControl

RendererServiceRenderingControl::RendererServiceRenderingControl()
    : RendererService("RenderingControl", 1, RenderingControl) {}

/// RendererServiceConnectionManager

RendererServiceConnectionManager::RendererServiceConnectionManager()
    : RendererService("ConnectionManager", 1, ConnectionManager) {}
};  // namespace pdr