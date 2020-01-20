#include "recyclr-types.h"

#include <string>
#include <netinet/in.h>

const static u32 CLIENT_VERSION_MAJOR = 1;
const static u32 CLIENT_VERSION_MINOR = 0;

class RecyclrClient {
    private:
        u32 client_version_major;
        u32 client_version_minor;

        struct in_addr client_ip;

    public:
        RecyclrClient();
        ~RecyclrClient();

        u32            load_ipv4_address(struct in_addr* ipv4_addr);
        std::string    get_local_ip_address();
        std::string    get_client_version_str();
};
