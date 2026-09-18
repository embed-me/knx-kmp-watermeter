#include <gtest/gtest.h>
#include <csignal>
#include <thread>
int main(int argc, char **argv) {
    signal(SIGHUP, SIG_IGN);
    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    return result;
}
