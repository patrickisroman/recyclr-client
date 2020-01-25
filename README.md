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
    [1579917519.193415] [recyclr-client.cpp:53] (0) Setting up Client
    [1579917519.194615] [recyclr-client.cpp:61] (0) Client Version: 1.0
    [1579917519.196614] [recyclr-client.cpp:74] (0) Client Local IPv4: 127.0.1.1
    [1579917519.197371] [recyclr-client.cpp:85] (0) Num cores: 4
    ...
    [18446744071963319832] [1] Accepted a connection on fd 5
    [18446744071963415932] [1] Opened a connection on fd 9

