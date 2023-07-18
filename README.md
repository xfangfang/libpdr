# Portable DLNA Renderer [WIP]

Adding DLNA renderer to your project in just a few lines of code.

```c++
pdr::Event::instance().subscribe([](const std::string& event, void* data){
    if (event == "CurrentURI") {
        printf("%s: %s\n", event.c_str(), (char*)data);
    } else if (event == "Stop") {
        printf("Stop\n");
    }
});

pdr::DLNA dlna("192.168.1.206", 8000, "uuid:00000000-0000-0000-0000-000000000000");
dlna.setDeviceInfo("friendlyName", "Portable DLNA Renderer");

dlna.start();
```
