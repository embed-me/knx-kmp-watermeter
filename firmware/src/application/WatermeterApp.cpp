#include "WatermeterApp.hpp"
#include "../drivers/logger/Logger.hpp"
#include "../utils/scheduler/Scheduler.hpp"

using namespace drivers::logger;

namespace application {

constexpr uint8_t KMP_WATERMETER_ADDRESS = 0x3F;

struct RegisterDef {
    uint16_t id;
    const char* name;
};

static constexpr RegisterDef REGISTER_DEFS[] = {
    {0x0044, "Volume"},
    {0x004A, "Flow"},
    {0x0124, "Temperature"},
    {0x0246, "Battery Life"},
};

WatermeterApp::WatermeterApp()
{
}

void WatermeterApp::init(std::shared_ptr<drivers::uart::IUartDriver> uart, const WatermeterConfig& config)
{
    config_ = config;
    kmpUart_ = uart;
    if (!kmpUart_) {
        logWarning("KMP UART driver not available");
        return;
    }

    struct drivers::uart::UartConfig kmpUartConfig = {
        .baud = 1200,
        .dataBits = 8,
        .parity = 0,
        .stopBits = 2,
        .txPin = 20,
        .rxPin = 21
    };
    if (!kmpUart_->init(kmpUartConfig)) {
        logError("KMP UART initialization failed");
        return;
    }

    initTransport(kmpUart_);
    if (kmpApplication_) {
        initCommands();
        initQueue();
        initTimers();
    }
}

void WatermeterApp::process()
{
    if (kmpUart_) {
        kmpUart_->process();
    }
}

void WatermeterApp::initTransport(std::shared_ptr<drivers::uart::IUartDriver> uart)
{
    kmpPhysical_ = std::make_shared<drivers::watermeter::kamstrup::transport::PhysicalLayer>(uart);
    kmpDataLink_ = std::make_shared<drivers::watermeter::kamstrup::transport::DataLinkLayer>(kmpPhysical_, KMP_WATERMETER_ADDRESS);
    kmpApplication_ = std::make_shared<drivers::watermeter::kamstrup::transport::ApplicationLayer>(kmpDataLink_);
    logInfo("KMP transport stack initialized");
}

void WatermeterApp::initCommands()
{
    using Result = drivers::watermeter::kamstrup::transport::CommandResult::Result;

    pingCmd_ = std::make_shared<drivers::watermeter::kamstrup::transport::GetSerialNumberCommand>(kmpApplication_);
    pingCmd_->registerListener([](const drivers::watermeter::kamstrup::transport::CommandResult& res){
        if (res.result == Result::OK) {
            logInfo("Ping successful: %s", res.value_str.c_str());
        } else if (res.result == Result::TIMEOUT) {
            logError("Ping timeout");
        } else {
            logError("Ping failed");
        }
    });

    for (const auto& reg : REGISTER_DEFS) {
        auto cmd = std::make_shared<drivers::watermeter::kamstrup::transport::GetRegisterCommand>(kmpApplication_, reg.id);
        cmd->registerListener([name = reg.name](const drivers::watermeter::kamstrup::transport::CommandResult& res){
            if (res.result == Result::OK) {
                const char *value = res.value_str.c_str();
                logInfo("%s: %s %s", name, value, drivers::watermeter::kamstrup::transport::unitToString(res.unit));
            } else if (res.result == Result::TIMEOUT) {
                logError("Failed to get %s: timeout", name);
            } else {
                logError("Failed to get %s", name);
            }
        });
        registerCmds_.push_back(cmd);
    }
}

void WatermeterApp::initQueue()
{
    kmpQueue_ = std::make_shared<drivers::watermeter::kamstrup::transport::CommandQueue>();
}

void WatermeterApp::initTimers()
{
    auto timerFactory = std::make_shared<drivers::timer::TimerFactory>();

    pingTimer_ = timerFactory->getTimer();
    pingTimer_->setupInterruptHandler([](void* arg){
        auto* self = static_cast<WatermeterApp*>(arg);
        utils::Scheduler::schedule([self](void*) {
            if (self->pingCmd_ && self->kmpQueue_) {
                self->kmpQueue_->enqueue(self->pingCmd_);
            }
        });
    }, this);
    pingTimer_->start(config_.pingIntervalUs, drivers::timer::TimerMode::RECURRING);

    dataTimer_ = timerFactory->getTimer();
    dataTimer_->setupInterruptHandler([](void* arg){
        auto* self = static_cast<WatermeterApp*>(arg);
        utils::Scheduler::schedule([self](void*) {
            for (auto& cmd : self->registerCmds_) {
                if (cmd && self->kmpQueue_) {
                    self->kmpQueue_->enqueue(cmd);
                }
            }
        });
    }, this);
    dataTimer_->start(config_.dataIntervalUs, drivers::timer::TimerMode::RECURRING);
}

} // namespace application
