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
    client_version_minor(CLIENT_VERSION_MINOR),
    num_cores(0),
    vertical_fd(-1),
    horizontal_fd(-1),
    client_ip()
{
}

RecyclrClient::~RecyclrClient()
{
    if (horizontal_fd != -1) {
        if (::close(horizontal_fd)) {
            int err = errno;
	    cout << "ERROR: Unable to close horizontal file descriptor: " << horizontal_fd << " | err: " << err << "\n";
            // TODO: Throw once we build out exceptions
        }
    }

    if (vertical_fd != -1) {
        if (::close(vertical_fd)) {
	    int err = errno;
            cout << "ERROR: Unable to close vertical file descriptor: " << vertical_fd << " | err: " << err << "\n";
            // TODO: Throw once we build out exceptions
        }
    }
}

u32 RecyclrClient::setup_client(bool verbose /*=false*/)
{
    if (verbose) {
        cout << "Setting up Client\n";
    }

    if (!client_version_major) {
        return -1;
    }

    if (verbose) {
        cout << "Client Version: " << get_client_version_str() << "\n";
    }

    // Load client address
    if (!client_ip.s_addr) {
        load_ipv4_address(&client_ip);

        if (!client_ip.s_addr) {
            return -1;
        }
    }

    if (verbose) {
        cout << "Client Local IPv4: " << get_local_ip_address() << "\n";
    }

    // Load architecture data
    num_cores = std::thread::hardware_concurrency();

    if (!num_cores) {
        return -1;
    }

    if (verbose) {
        cout << "Num cores: " << num_cores << "\n";
    }

    return 0;
}

u32 RecyclrClient::load_ipv4_address(struct in_addr* ipv4_addr)
{
    if (!ipv4_addr) {
        return EINVAL;
    }

    char host_address[1<<8];
    if (gethostname(host_address, sizeof(host_address))) {
        return EINVAL;
    }

    struct hostent* host_entry = gethostbyname(host_address);
    if (!host_entry) {
        return EINVAL;
    }

    *ipv4_addr = *((struct in_addr*) host_entry->h_addr_list[0]);
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
    RecyclrClient client;
    client.setup_client(true);
    return 0;
}
