#include "recyclr-types.h"
#include "recyclr-net.h"
#include "recyclr-controller.h"
#include "recyclr-image-controller.h"
#include "config/recyclr-client-config.h"

#include <string>

const static u32 CLIENT_VERSION_MAJOR = 1;
const static u32 CLIENT_VERSION_MINOR = 0;

extern RecyclrClientConfig* g_config;

class RecyclrClient {
    private:
        struct in_addr client_ip;
        NetClient*       net_client;
        Controller*      controller;
        ImageController* image_controller;
        u32              num_cores;
        u32              client_version_major;
        u32              client_version_minor;

    public:
        RecyclrClient();
        ~RecyclrClient();

        u32            setup_client(bool verbose=false);
        u32            load_ipv4_address(struct in_addr* ipv4_addr);
        std::string    get_local_ip_address();
        std::string    get_client_version_str();
};
