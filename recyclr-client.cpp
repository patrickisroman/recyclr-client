#include <stdio.h>
#include <iostream>
#include <thread>

#include "recyclr-client.h"
#include "recyclr-commands.h"

#define cout std::cout
#define new_ln cout << "\n"

int main() {
    cout << "Starting Recyclr Client\n";
    cout << "Client version: ";
    cout << CLIENT_VERSION_MAJOR << "." << CLIENT_VERSION_MINOR << "\n\n";

    unsigned int num_cores = std::thread::hardware_concurrency();
    cout << "CPU Cores: " << num_cores << "\n";

    std::string local_machine_ip = exec("ifconfig -a | grep \"wlan0\" -A1");
    cout << "Local machine IP Address: " << local_machine_ip << "\n";
    
    return 0;
}