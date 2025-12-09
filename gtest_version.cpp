#include <gtest/gtest.h>
#include "lib.h"

TEST(VersionTest, ValidRelease) {
    EXPECT_LT(0, version());
}