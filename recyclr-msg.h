#pragma once

#include "recyclr-types.h"

#define MSG_MAGIC 0x8370202083702020

enum RecyclrTarget {
    NETWORK_TARGET = 1,
    IMAGE_SEGMENTATION_TARGET = 2,
    DRONE_CONTROLLER_TARGET = 3,
    HEALTH_MONITOR_TARGET = 4,
    LOCALIZATION_TARGET = 5
};

enum MessageCode {
    OPEN_CONNECTION     = 1,
    ACK_OPEN_CONNECTION = 2,
    TEST_OP             = 3
};

struct recyclr_msg_header {
    byte message_code : 8;
    byte destination  : 8;
    byte priority     : 8;
    u64  micro_ts     : 64;
    u32  payload_len  : 32;
    u64  msg_magic    : 64;
} __attribute__((packed));
