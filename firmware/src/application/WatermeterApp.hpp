#ifndef _WATERMETER_APP_HPP_
#define _WATERMETER_APP_HPP_

#include "../drivers/uart/IUartDriver.hpp"
#include "../drivers/watermeter/kamstrup/transport/layers/physical/PhysicalLayer.hpp"
#include "../drivers/watermeter/kamstrup/transport/layers/data_link/DataLinkLayer.hpp"
#include "../drivers/watermeter/kamstrup/transport/layers/application/ApplicationLayer.hpp"
#include "../drivers/watermeter/kamstrup/commands/GetSerialNumberCommand.hpp"
#include "../drivers/watermeter/kamstrup/commands/GetRegisterCommand.hpp"
#include "../drivers/watermeter/kamstrup/utils/CommandQueue.hpp"

#include "../drivers/timer/TimerFactory.hpp"
#include "../drivers/timer/ITimer.hpp"

#include <memory>
#include <cstdint>
#include <vector>

namespace application {

struct WatermeterConfig {
    uint32_t pingIntervalUs = 30000000;  // 30 seconds
    uint32_t dataIntervalUs = 60000000;  // 60 seconds
};

class WatermeterApp {
public:
    WatermeterApp();
    ~WatermeterApp() = default;

    void init(std::shared_ptr<drivers::uart::IUartDriver> uart, const WatermeterConfig& config);
    void process();

private:
    void initTransport(std::shared_ptr<drivers::uart::IUartDriver> uart);
    void initCommands();
    void initQueue();
    void initTimers();

    std::shared_ptr<drivers::watermeter::kamstrup::transport::IPhysicalLayer> kmpPhysical_;
    std::shared_ptr<drivers::watermeter::kamstrup::transport::IDataLinkLayer> kmpDataLink_;
    std::shared_ptr<drivers::watermeter::kamstrup::transport::IApplicationLayer> kmpApplication_;

    std::shared_ptr<drivers::watermeter::kamstrup::transport::GetSerialNumberCommand> pingCmd_;
    std::vector<std::shared_ptr<drivers::watermeter::kamstrup::transport::GetRegisterCommand>> registerCmds_;

    std::shared_ptr<drivers::watermeter::kamstrup::transport::CommandQueue> kmpQueue_;
    std::shared_ptr<drivers::timer::ITimer> pingTimer_;
    std::shared_ptr<drivers::timer::ITimer> dataTimer_;

    std::shared_ptr<drivers::uart::IUartDriver> kmpUart_;
    WatermeterConfig config_;
};

}

#endif // _WATERMETER_APP_HPP_
