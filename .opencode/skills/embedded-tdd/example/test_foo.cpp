#include <gtest/gtest.h>
#include "src/path/to/Foo.cpp"

TEST(FooTest, DoesThing) {
    Foo foo;
    EXPECT_EQ(foo.doThing(), 42);
}
