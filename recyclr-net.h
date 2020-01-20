#include "recyclr-types.h"

#include <thread>
#include <string>

#define BASE_SYNC_PORT  5440
#define CLIENT_SYNC_PORT 5441

struct client_config {
    client_config() :
        client_ip_address(""),
        server_ip_address(""),
        port(0)
        {}
    
    std::string client_ip_address;
    std::string server_ip_address;
    u64 port;
};

struct channel_ctl {
    void (*start_channel)();
    void (*run_channel)();
    void (*close_channel)();
};

class NetworkManager
{
    private:
    channel_ctl ctl;
    client_config config;
    std::thread *thr;

    public:
    NetworkManager() {};
    ~NetworkManager() {};

    void talk();
};