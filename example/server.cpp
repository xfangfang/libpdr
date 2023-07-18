//
// Created by fang on 2023/7/16.
//

#include <libpdr.h>
#include <iostream>

int main(){
    std::cout << "welcome to libpdr example: server" << std::endl;
    std::string command;

    // 订阅控制信息
    pdr::Event::instance().subscribe([](const std::string& event, void* data){
        if (event == "CurrentURI") {
            printf("%s: %s\n", event.c_str(), (char*)data);
        } else if (event == "Stop") {
            printf("Stop\n");
        }
    });

    // 推送播放器状态
    double position = 13.0;
    pdr::Event::instance().fire("Position", &position);

    // 创建 DLNA Renderer
    pdr::DLNA dlna("192.168.1.206", 8000, "uuid:00000000-0000-0000-0000-000000000000");
    dlna.setDeviceInfo("friendlyName", "Portable DLNA Renderer");

    while (true) {
        std::cout << "Command: start, stop; Enter a command (or 'q' to quit): " << std::endl << "pdr_shell $ ";

        std::getline(std::cin, command);
        if (command == "q") break;

        std::cout << "Executing command: " << command << std::endl;
        if (command == "start") {
            dlna.start();
        } else if (command == "stop") {
            dlna.stop();
        }
    }

    return 0;
}