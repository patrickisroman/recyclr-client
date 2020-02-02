#pragma once

#include "recyclr-types.h"

#define MSG_MAGIC 0x8370202083702020

enum MessageCode {
    open_connection     = 1,
    ack_open_connection = 2
};

struct recyclr_msg_header {
    byte message_code : 8;
    byte priority     : 8;
    u64  micro_ts     : 64;
    u32  msg_length   : 32;
    u64  msg_magic    : 64;
} __attribute__((packed));

//
//  +------------+-----------+------------------------+
//  |   Message  |  Priority |      Microsecond       |
//  |    Code    |           |       Timestamp        |
//  +------------+-----------+------------------------+
//