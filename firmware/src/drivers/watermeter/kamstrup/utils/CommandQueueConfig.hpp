#ifndef _COMMAND_QUEUE_CONFIG_HPP_
#define _COMMAND_QUEUE_CONFIG_HPP_

#include <cstdint>

namespace drivers::watermeter::kamstrup::transport {

struct CommandQueueConfig {
    uint32_t commandTimeoutUs;
};

}

#endif //_COMMAND_QUEUE_CONFIG_HPP_
