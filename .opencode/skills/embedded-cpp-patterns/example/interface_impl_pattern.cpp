// Interface
class ITimer {
public:
    virtual void start(uint32_t timeout_us, TimerMode mode) = 0;
    virtual void stop() = 0;
    virtual void setupInterruptHandler(Handler h, void* arg) = 0;
};

// Arduino implementation
class ArduinoTimer : public ITimer {
    // uses Pico SDK add_alarm_in_us / cancel_alarm
};

// Mock for testing
struct MockTimer : public ITimer {
    bool started = false;
    void start(uint32_t us, TimerMode m) override { started = true; }
    void stop() override { started = false; }
    void setupInterruptHandler(Handler h, void* arg) override {}
    void fire() { /* trigger handler */ }
};
