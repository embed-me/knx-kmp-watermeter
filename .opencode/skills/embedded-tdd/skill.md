# Embedded TDD (Red-Green-Refactor)

Test-Driven Development for embedded C++ firmware using GoogleTest on native host.

## Workflow

### 1. RED — Write a failing test first
- Add a new test suite under `firmware/test/test_<component>/`
- Each suite needs a `test_main.cpp` and `test_<component>.cpp`
- Include the unit-under-test's `.cpp` directly (native compilation needs it)
- Use `TEST()` or `TEST_F()` from GoogleTest

```cpp
// test/test_foo/test_foo.cpp
#include <gtest/gtest.h>
#include "src/path/to/Foo.cpp"

TEST(FooTest, DoesThing) {
    Foo foo;
    EXPECT_EQ(foo.doThing(), expected);
}
```

### 2. GREEN — Make it compile and pass
- Implement the minimum code in the production source
- For hardware dependencies, use mocks from `test/mocks/`:
  ```cpp
  auto mockTimer = std::make_shared<MockTimer>();
  auto driver = WatermeterWakeupDriver(motion, cfg, mockTimer);
  ```
- For hardware headers unavailable on host, use stubs in `test/stubs/`
- Run: `pio test -e test`

### 3. REFACTOR — Clean up while keeping green
- Apply SOLID and OOP principles, remove duplication, rename for clarity
- Do NOT change behavior
- Re-run tests to confirm

## Test infrastructure conventions

| Resource | Location | Purpose |
|---|---|---|
| Tests | `firmware/test/test_*/` | One directory per component |
| Mocks | `firmware/test/mocks/` | Hand-rolled mock classes (no mocking framework) |
| Stubs | `firmware/test/stubs/` | Platform headers unavailable on host (e.g. `pico/time.h`) |
| Main | `firmware/test/test_*/test_main.cpp` | GoogleTest entry point (`RUN_ALL_TESTS`) |

## Writing mocks

Follow the existing pattern in `test/mocks/`:

```cpp
struct MockFoo : public IFoo {
    bool doThingCalled = false;
    int doThingResult = 42;

    int doThing() override {
        doThingCalled = true;
        return doThingResult;
    }
};
```

Build commands are in [firmware/Readme.md](../../../firmware/Readme.md).
