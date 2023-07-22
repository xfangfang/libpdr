# Portable DLNA Renderer [WIP]

Adding DLNA renderer to your project in just a few lines of code.

```c++
DLNA_EVENT.subscribe([](const std::string& event, void* data){
    if (event == "CurrentURI") {
        printf("%s: %s\n", event.c_str(), (char*)data);
    } else if (event == "Stop") {
        printf("Stop\n");
    } else if (event == "Error") {
        printf("Error: %s\n", (char*)data);
    }
});

pdr::DLNA dlna("192.168.1.206", 8000, "uuid:00000000-0000-0000-0000-000000000000");
dlna.setDeviceInfo("friendlyName", "Portable DLNA Renderer");

dlna.start();
```

For more information, please refer to my
project: [wiliwili/activity/dlna_activity.cpp](https://github.com/xfangfang/wiliwili/blob/dev/wiliwili/source/activity/dlna_activity.cpp)
