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
    [18446744071906060105] [0] Setting up Client
    [18446744071906060386] [0] Client Version: 1.0
    [18446744071906062197] [0] Client Local IPv4: 127.0.1.1
    [18446744071906062418] [0] Num cores: 4
    ...
    [18446744071963319832] [1] Accepted a connection on fd 5
    [18446744071963415932] [1] Opened a connection on fd 9

