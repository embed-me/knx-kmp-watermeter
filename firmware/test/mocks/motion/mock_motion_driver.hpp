#ifndef _MOCK_MOTION_DRIVER_HPP_
#define _MOCK_MOTION_DRIVER_HPP_

#include "src/drivers/motion/IMotionDriver.hpp"
#include "src/drivers/motion/MotionConfig.hpp"
#include <cstdint>

struct MockMotionDriver : public drivers::motion::IMotionDriver {
    bool setConfigCalled = false;
    uint8_t lastAngle = 255;
    drivers::motion::MotionConfig lastConfig{};

    void setConfig(const drivers::motion::MotionConfig& cfg) override {
        setConfigCalled = true;
        lastConfig = cfg;
    }

    void write(uint8_t angle) override {
        lastAngle = angle;
    }
};

#endif //_MOCK_MOTION_DRIVER_HPP_
