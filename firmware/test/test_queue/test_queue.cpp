#include <gtest/gtest.h>

// CommandQueue tests cannot compile on native because:
// - CommandQueue constructor creates TimerFactory internally (CommandQueue.cpp:15)
// - TimerFactory::getTimer() returns ArduinoTimer, which requires <Arduino.h>
// - To enable testing: CommandQueue should accept ITimerDriverFactory interface
// See test plan issues I5 and test plan section 6 for details.
//
// Test cases (once timers are mockable):
// 1. enqueue single command → executed, CID handler registered
// 2. enqueue while busy → queued, executed after first completes
// 3. onCommandDone via listener → next queued command starts
// 4. timeout fires before response → TIMEOUT delivered, retry delay starts
// 5. retry delay expires → next queued command starts
// 6. stale timeout (sequence mismatch) → ignored
// 7. stale listener (sequence mismatch) → ignored
// 8. enqueue with null command → no-op, no crash

TEST(CommandQueue, Placeholder) {
    SUCCEED(); // placeholder so test binary doesn't fail with no tests
}
