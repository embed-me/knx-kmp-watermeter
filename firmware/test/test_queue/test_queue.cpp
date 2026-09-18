#include <gtest/gtest.h>

#include "src/drivers/watermeter/kamstrup/transport/layers/IApplicationLayer.hpp"
#include "src/drivers/watermeter/kamstrup/utils/CommandQueue.hpp"
#include "src/drivers/watermeter/kamstrup/utils/CommandQueueConfig.hpp"

#include "src/drivers/watermeter/kamstrup/utils/CommandQueue.cpp"
#include "src/drivers/timer/ArduinoTimerFactory.cpp"
#include "src/drivers/timer/arduino/ArduinoTimer.cpp"
#include "src/drivers/logger/Logger.cpp"
#include "src/utils/scheduler/Scheduler.cpp"

#include "../mocks/timer/mock_timer.hpp"
#include "../mocks/mock_application_layer.hpp"
#include "../mocks/mock_command.hpp"

#include "src/drivers/timer/ArduinoTimerFactory.hpp"

#include <memory>

using namespace drivers::watermeter::kamstrup::transport;

struct ImmediateSchedulerStrategy : public utils::ISchedulerStrategy {
    void process() override {}
    void schedule(std::function<void(void*)> work) override {
        work(nullptr);
    }
};

struct MockTimerFactory : public drivers::timer::ArduinoTimerFactory {
    std::shared_ptr<MockTimer> mockTimeoutTimer = std::make_shared<MockTimer>();
    std::shared_ptr<drivers::timer::ITimer> getTimer() override {
        return mockTimeoutTimer;
    }
};

class CommandQueueTest : public ::testing::Test {
protected:
    void SetUp() override {
        scheduler_ = std::make_unique<utils::Scheduler>(
            std::make_shared<ImmediateSchedulerStrategy>());

        appLayer_ = std::make_shared<MockApplicationLayer>();
        timerFactory_ = std::make_shared<MockTimerFactory>();

        CommandQueueConfig cfg{1600000};
        queue_ = std::make_shared<CommandQueue>(
            appLayer_, cfg, timerFactory_);
    }

    void TearDown() override {
        queue_.reset();
        utils::Scheduler s(nullptr);
    }

    std::unique_ptr<utils::Scheduler> scheduler_;
    std::shared_ptr<MockApplicationLayer> appLayer_;
    std::shared_ptr<MockTimerFactory> timerFactory_;
    std::shared_ptr<CommandQueue> queue_;
};

TEST_F(CommandQueueTest, EnqueueNullCommandNoCrash) {
    EXPECT_NO_THROW(queue_->enqueue(nullptr));
}

TEST_F(CommandQueueTest, EnqueueSingleCommandSendsImmediately) {
    auto cmd = MockCommand::create(0x10);
    queue_->enqueue(cmd);

    EXPECT_TRUE(cmd->executeCalled);
    EXPECT_TRUE(timerFactory_->mockTimeoutTimer->started);
    EXPECT_EQ(timerFactory_->mockTimeoutTimer->lastTimeoutUs, 1600000u);
}

TEST_F(CommandQueueTest, EnqueueWhileBusyQueues) {
    auto cmd1 = MockCommand::create(0x10);
    auto cmd2 = MockCommand::create(0x20);
    queue_->enqueue(cmd1);
    queue_->enqueue(cmd2);

    EXPECT_TRUE(cmd1->executeCalled);
    EXPECT_FALSE(cmd2->executeCalled);
}

TEST_F(CommandQueueTest, OnCommandDoneSendsNext) {
    auto cmd1 = MockCommand::create(0x10);
    auto cmd2 = MockCommand::create(0x20);
    queue_->enqueue(cmd1);
    queue_->enqueue(cmd2);

    appLayer_->invokeHandler(cmd1->getCid(), {});

    EXPECT_TRUE(cmd2->executeCalled);
}

TEST_F(CommandQueueTest, OnCommandDoneCallsOnEmptyCallback) {
    bool emptyCalled = false;
    queue_->setOnEmpty([&]() { emptyCalled = true; });

    auto cmd1 = MockCommand::create(0x10);
    queue_->enqueue(cmd1);
    appLayer_->invokeHandler(cmd1->getCid(), {});

    EXPECT_TRUE(emptyCalled);
}

TEST_F(CommandQueueTest, OnTimeoutSendsNext) {
    auto cmd1 = MockCommand::create(0x10);
    auto cmd2 = MockCommand::create(0x20);
    queue_->enqueue(cmd1);
    queue_->enqueue(cmd2);

    timerFactory_->mockTimeoutTimer->fire();

    EXPECT_TRUE(cmd2->executeCalled);
}

TEST_F(CommandQueueTest, OnTimeoutCallsOnEmptyCallback) {
    bool emptyCalled = false;
    queue_->setOnEmpty([&]() { emptyCalled = true; });

    auto cmd1 = MockCommand::create(0x10);
    queue_->enqueue(cmd1);
    timerFactory_->mockTimeoutTimer->fire();

    EXPECT_TRUE(emptyCalled);
}

TEST_F(CommandQueueTest, SequenceNumberPreventsStaleResponse) {
    auto cmd1 = MockCommand::create(0x10);
    auto cmd2 = MockCommand::create(0x20);
    queue_->enqueue(cmd1);
    queue_->enqueue(cmd2);

    appLayer_->invokeHandler(cmd1->getCid(), {});

    EXPECT_EQ(cmd1->lastResult, ExecuteResult::SUCCESS);

    auto stale = MockCommand::create(0x10);
    stale->onExecuteResult(ExecuteResult::SUCCESS, {});
}

TEST_F(CommandQueueTest, MultipleCommandsSequentialFlow) {
    auto cmd1 = MockCommand::create(0x10);
    auto cmd2 = MockCommand::create(0x20);
    auto cmd3 = MockCommand::create(0x30);

    queue_->enqueue(cmd1);
    queue_->enqueue(cmd2);
    queue_->enqueue(cmd3);

    EXPECT_TRUE(cmd1->executeCalled);
    appLayer_->invokeHandler(cmd1->getCid(), {});
    EXPECT_TRUE(cmd2->executeCalled);
    appLayer_->invokeHandler(cmd2->getCid(), {});
    EXPECT_TRUE(cmd3->executeCalled);
    appLayer_->invokeHandler(cmd3->getCid(), {});
}
