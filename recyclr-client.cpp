#include <stdio.h>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "recyclr-client.h"
#include "recyclr-commands.h"

#define cout std::cout
#define new_ln cout << "\n"

RecyclrClient::RecyclrClient() :
    client_version_major(CLIENT_VERSION_MAJOR),
    client_version_minor(CLIENT_VERSION_MINOR)
{
}

RecyclrClient::~RecyclrClient()
{
}

u32 RecyclrClient::load_ipv4_address(struct in_addr* ipv4_addr)
{
    if (!ipv4_addr) {
        return EINVAL;
    }

    char host_address[1<<8];
    int  hostname = gethostname(host_address, sizeof(host_address));
    if (hostname) {
        return EINVAL;
    }

    struct hostent* host_entry = gethostbyname(host_address);
    if (!host_entry) {
        return EINVAL;
    }

    ipv4_addr = (struct in_addr*) host_entry->h_addr_list[0];
    return 0;
}

std::string RecyclrClient::get_local_ip_address()
{
    if (!client_ip.s_addr) {
        load_ipv4_address(&client_ip);
        if (!client_ip.s_addr) {
            return "";
        }
    }

    return std::string(inet_ntoa(client_ip));
}

std::string RecyclrClient::get_client_version_str()
{
    char version[32];
    int n = sprintf(version, "%d.%d", client_version_major, client_version_minor);
    return n ? std::string(version) : "";
}

int main() {
    cout << "Starting Recyclr Client\n";
    
    RecyclrClient client;
    cout << "Client version: " << client.get_client_version_str() << "\n";

    unsigned int num_cores = std::thread::hardware_concurrency();
    cout << "CPU Cores: " << num_cores << "\n";

    std::string local_ip_address = client.get_local_ip_address(); 
    cout << "Local Machine IP: " << local_ip_address << "\n";

    return 0;
}
