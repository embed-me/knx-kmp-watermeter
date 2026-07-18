#ifndef _WATERMETER_APP_HPP_
#define _WATERMETER_APP_HPP_

#include "IApplication.hpp"
#include "../drivers/watermeter/kamstrup/transport/layers/physical/PhysicalLayer.hpp"
#include "../drivers/watermeter/kamstrup/transport/layers/data_link/DataLinkLayer.hpp"
#include "../drivers/watermeter/kamstrup/transport/layers/application/ApplicationLayer.hpp"
#include "../drivers/watermeter/kamstrup/commands/GetSerialNumberCommand.hpp"
#include "../drivers/watermeter/kamstrup/commands/GetRegisterCommand.hpp"
#include "../drivers/watermeter/kamstrup/utils/CommandQueue.hpp"
#include "../drivers/watermeter/wakeup/IWatermeterWakeupDriver.hpp"

#include "../drivers/timer/ArduinoTimerFactory.hpp"
#include "../drivers/timer/ITimer.hpp"

#include <memory>
#include <cstdint>
#include <vector>

namespace application {

class WatermeterApp : public IApplication {
public:
    WatermeterApp();
    ~WatermeterApp() override = default;

    void init(
        std::shared_ptr<drivers::uart::IUartDriver> uart,
        const drivers::uart::UartConfig& uartConfig,
        drivers::knx::KnxConfig& knxConfig,
        const drivers::watermeter::kamstrup::transport::CommandQueueConfig& queueConfig,
        std::shared_ptr<drivers::watermeter::wakeup::IWatermeterWakeupDriver> wakeupDriver,
        std::shared_ptr<drivers::timer::ITimerDriverFactory> timerFactory
    );
    void process() override;

private:
    void initTransport(std::shared_ptr<drivers::uart::IUartDriver> uart);
    void initKeepAliveCommand();
    void initRegisterCommands();
    void initQueue(std::shared_ptr<drivers::timer::ITimerDriverFactory> timerFactory);
    void initTimers(std::shared_ptr<drivers::timer::ITimerDriverFactory> timerFactory);
    void enqueuePendingCommands();
    void onQueueEmpty();
    uint32_t compensatedInterval(uint32_t intervalSec) const;

    std::shared_ptr<drivers::watermeter::kamstrup::transport::IPhysicalLayer> kmpPhysical_;
    std::shared_ptr<drivers::watermeter::kamstrup::transport::IDataLinkLayer> kmpDataLink_;
    std::shared_ptr<drivers::watermeter::kamstrup::transport::IApplicationLayer> kmpApplication_;

    std::shared_ptr<drivers::watermeter::kamstrup::transport::GetSerialNumberCommand> keepAliveCmd_;
    std::vector<std::shared_ptr<drivers::watermeter::kamstrup::transport::GetRegisterCommand>> registerCmds_;

    std::shared_ptr<drivers::watermeter::kamstrup::transport::CommandQueue> commandQueue_;
    std::shared_ptr<drivers::timer::ITimer> keepAliveTimer_;
    std::shared_ptr<drivers::timer::ITimer> dataTimer_;

    std::shared_ptr<drivers::uart::IUartDriver> kmpUart_;
    drivers::knx::KnxConfig* knxConfig_ = nullptr;
    drivers::watermeter::kamstrup::transport::CommandQueueConfig queueConfig_;

    std::shared_ptr<drivers::watermeter::wakeup::IWatermeterWakeupDriver> wakeupDriver_;
    bool dataPending_ = false;
    bool keepAlivePending_ = false;
};

}

#endif // _WATERMETER_APP_HPP_
