#include "Wheel.h"

#include <string>
#include <gtest/gtest.h>

class WheelTest : public ::testing::Test
{

protected:
    WheelTest() 
    { 
        wheel = new Wheel(15, "Alloy"); 
    }

    virtual ~WheelTest()
    {
        delete wheel;
        wheel = NULL;
    }


    Wheel* wheel;
};

TEST_F(WheelTest, ConstructorTest) {
    EXPECT_EQ(wheel->GetDiameter(), 15);
    EXPECT_EQ(wheel->GetMaterial(), "Alloy");
    EXPECT_THROW(Wheel(0, "Test Material"), std::out_of_range);
}