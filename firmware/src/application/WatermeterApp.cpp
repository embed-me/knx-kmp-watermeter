#include "WatermeterApp.hpp"
#include "../drivers/logger/Logger.hpp"
#include "../utils/scheduler/Scheduler.hpp"

using namespace drivers::logger;

constexpr uint32_t USEC_PER_SEC = 1000000;

namespace kmp = drivers::watermeter::kamstrup::transport;

namespace application {

WatermeterApp::WatermeterApp()
{
}

void WatermeterApp::init(
    std::shared_ptr<drivers::uart::IUartDriver> uart,
    const drivers::uart::UartConfig& uartConfig,
    drivers::knx::KnxConfig& knxConfig)
{
    knxConfig_ = &knxConfig;
    kmpUart_ = uart;
    if (!kmpUart_) {
        logWarning("KMP UART driver not available");
        return;
    }

    if (!kmpUart_->init(uartConfig)) {
        logError("KMP UART initialization failed");
        return;
    }

    initTransport(kmpUart_);
    if (kmpApplication_) {
        initKeepAliveCommand();
        initRegisterCommands();
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
    uint8_t addr = knxConfig_->getWatermeterConfig().destinationAddress;
    kmpPhysical_ = std::make_shared<kmp::PhysicalLayer>(uart);
    kmpDataLink_ = std::make_shared<kmp::DataLinkLayer>(kmpPhysical_, addr);
    kmpApplication_ = std::make_shared<kmp::ApplicationLayer>(kmpDataLink_);
    logInfo("KMP transport stack initialized");
}

void WatermeterApp::initKeepAliveCommand()
{
    auto linked = knxConfig_->getKeepAliveLinkedState();

    keepAliveCmd_ = std::make_shared<kmp::GetSerialNumberCommand>(kmpApplication_);
    keepAliveCmd_->registerListener([go = linked.groupObject, dpt = linked.dpt](const kmp::CommandResult& res){
        bool isOk = res.result == kmp::CommandResult::Result::OK;
        if (go) {
            go->valueCompare(KNXValue(isOk), dpt);
        }
        logInfo("KeepAlive: %s", isOk ? "OK" : "FAILED");
    });
}

void WatermeterApp::initRegisterCommands()
{
    auto regCfgs = knxConfig_->getWatermeterRegisterConfigs();

    for (auto& reg : regCfgs) {
        if (!reg.enabled) {
            continue;
        }

        auto cmd = std::make_shared<kmp::GetRegisterCommand>(kmpApplication_, reg.registerId);
        cmd->registerListener([name = reg.name, go = reg.groupObject, dpt = reg.dpt](const kmp::CommandResult& res){
            if (go && (res.result == kmp::CommandResult::Result::OK)) {
                go->value(KNXValue(res.value), dpt);
                logInfo("%s: %g %s", name, res.value,
                        kmp::unitToString(res.unit));
            } else {
                logWarning("Failed to get %s: with error %d", name, res.result);
            }
        });
        registerCmds_.push_back(cmd);
    }
}

void WatermeterApp::initQueue()
{
    commandQueue_ = std::make_shared<kmp::CommandQueue>(kmpApplication_);
}

void WatermeterApp::initTimers()
{
    auto knxWaterCfg = knxConfig_->getWatermeterConfig();

    auto timerFactory = std::make_shared<drivers::timer::TimerFactory>();

    keepAliveTimer_ = timerFactory->getTimer();
    keepAliveTimer_->setupInterruptHandler([](void* arg){
        auto* self = static_cast<WatermeterApp*>(arg);
        utils::Scheduler::schedule([self](void*) {
            if (self->keepAliveCmd_ && self->commandQueue_) {
                self->commandQueue_->enqueue(self->keepAliveCmd_);
            }
        });
    }, this);
    keepAliveTimer_->start(knxWaterCfg.keepAliveIntervalSec * USEC_PER_SEC, drivers::timer::TimerMode::RECURRING);

    dataTimer_ = timerFactory->getTimer();
    dataTimer_->setupInterruptHandler([](void* arg){
        auto* self = static_cast<WatermeterApp*>(arg);
        utils::Scheduler::schedule([self](void*) {
            for (auto& cmd : self->registerCmds_) {
                if (cmd && self->commandQueue_) {
                    self->commandQueue_->enqueue(cmd);
                }
            }
        });
    }, this);
    dataTimer_->start(knxWaterCfg.dataIntervalSec * USEC_PER_SEC, drivers::timer::TimerMode::RECURRING);
}

} // namespace application
