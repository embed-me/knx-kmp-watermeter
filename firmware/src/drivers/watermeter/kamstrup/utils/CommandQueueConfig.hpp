#ifndef _COMMAND_QUEUE_CONFIG_HPP_
#define _COMMAND_QUEUE_CONFIG_HPP_

#include <cstdint>

namespace drivers::watermeter::kamstrup::transport {

struct CommandQueueConfig {
    uint32_t commandTimeoutUs = 1600000;
    uint32_t settleDelayUs = 5000000;
};

}

#endif //_COMMAND_QUEUE_CONFIG_HPP_
