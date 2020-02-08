# Recyclr Client

The recyclr client is the node stack of recyclr's logic. Recyclr allows for a single server and multiple clients to communicate and coordinate their actions. The client is able to create and accept connections with the server and other clients to allow vertical and horizontal communication.

## Build

    # ./build.sh
    (C) recyclr-client.o
    (C) recyclr-utils.o
    (C) recyclr-net.o
    (L) recyclr

## Run

    ./recyclr
    [LOG] [1581136621.122028] (0) [recyclr-client.cpp:44] Setting up Client
    [LOG] [1581136621.122536] (0) [recyclr-client.cpp:52] Client Version: 1.0
    [LOG] [1581136621.123901] (0) [recyclr-client.cpp:65] Client Local IPv4: 127.0.1.1
    [LOG] [1581136621.123955] (0) [recyclr-client.cpp:76] Num cores: 4
    [LOG] [1581136629.490558] (1) [recyclr-net.cpp:353] Accepted a connection from: 10.0.0.98 [fd=5]

