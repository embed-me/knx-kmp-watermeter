#include <gtest/gtest.h>
#include "src/drivers/rtc/IRtcDriver.hpp"

#include <memory>

using namespace drivers::rtc;

struct MockRtcDriver : public IRtcDriver {
    DateTime stored;

    void setDateTime(const DateTime& dt) override {
        stored = dt;
    }

    DateTime getDateTime() override {
        return stored;
    }
};

TEST(RtcTest, SetAndGetRoundTrip) {
    auto rtc = std::make_shared<MockRtcDriver>();
    DateTime input = {2024, 6, 15, 10, 30, 45};
    rtc->setDateTime(input);
    auto output = rtc->getDateTime();
    EXPECT_EQ(output.year, input.year);
    EXPECT_EQ(output.month, input.month);
    EXPECT_EQ(output.day, input.day);
    EXPECT_EQ(output.hour, input.hour);
    EXPECT_EQ(output.minute, input.minute);
    EXPECT_EQ(output.second, input.second);
}

TEST(RtcTest, DefaultConstructedDateTime) {
    auto rtc = std::make_shared<MockRtcDriver>();
    auto dt = rtc->getDateTime();
    EXPECT_EQ(dt.year, 0u);
    EXPECT_EQ(dt.month, 0u);
}
