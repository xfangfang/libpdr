//
// Created by fang on 2023/7/16.
//

#include <libpdr.h>
#include <iostream>

bool getIPAndPort(std::string& ip, int& port) {
    std::cout << "Input server ip: ";
    std::getline(std::cin, ip);
    std::cout << "Input server port: ";
    std::cin >> port;
    if (std::cin.fail()) {
        std::cout << "Invalid port" << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return true;
}

int main() {
    std::cout << "welcome to libpdr example: server" << std::endl;
    std::string command, ip;
    int port;
    while (!getIPAndPort(ip, port))
        ;

    // 订阅控制信息
    DLNA_EVENT.subscribe([](const std::string& event, void* data) {
        if (event == "CurrentURI") {
            printf("%s: %s\n", event.c_str(), (char*)data);
        } else if (event == "Stop") {
            printf("Stop\n");
        } else if (event == "Error") {
            printf("Error: %s\n", (char*)data);
        }
    });

    // 推送播放器状态
    std::string msg = "STOPPED";
    PLAYER_EVENT.fire("TransportState", (void*)msg.c_str());

    // 创建 DLNA Renderer
    pdr::DLNA dlna(ip, 8000, "uuid:00000000-0000-0000-0000-000000000000");
    dlna.setDeviceInfo("friendlyName", "Portable DLNA Renderer");
    dlna.start();
    std::cout << "========= DLNA Media Renderer started =========" << std::endl;
    std::cout << "SOAP: http://0.0.0.0:" << port << std::endl;
    std::cout << "SSDP: udp://0.0.0.0:1900"
              << " (DLNA: " << ip << ":" << port << ")" << std::endl;

    while (true) {
        std::cout << "========= Enter 'q' to quit =========" << std::endl;
        std::getline(std::cin, command);
        if (command == "q") break;
    }
    dlna.stop();
    return 0;
}