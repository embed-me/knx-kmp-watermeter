#include <gtest/gtest.h>

// CommandQueue tests cannot compile on native because:
// - CommandQueue constructor creates TimerFactory internally (CommandQueue.cpp:18)
// - TimerFactory::getTimer() returns ArduinoTimer, which requires <Arduino.h>
//
// Test cases (once timers are mockable):
// 1. enqueue single command — executed, CID handler registered
// 2. enqueue while busy — queued, executed after first completes
// 3. onCommandDone via listener — next queued command starts
// 4. timeout fires before response — TIMEOUT delivered, wakeupDriver->isAwake() checked for cleanup
// 5. timeout while awake with remaining commands — sends next, isAwake stays true (in wakeup driver)
// 6. timeout while awake with empty queue — sleep called, isAwake transitions to false (in wakeup driver)
// 7. stale timeout (sequence mismatch) — ignored
// 8. stale listener (sequence mismatch) — ignored
// 9. enqueue with wakeup driver — wakeup called, settle timer starts, then sendNext
// 10. enqueue without wakeup driver — sendNext called immediately
// 11. onCommandDone empty queue while awake — sleep called, isAwake clears (in wakeup driver)
// 12. enqueue with null command — no-op, no crash
// 13. enqueue while awake — no duplicate wakeup call, sendNext immediately
// 14. onCommandDone empty queue with no wakeup driver — no crash

TEST(CommandQueue, Placeholder) {
    SUCCEED();
}
