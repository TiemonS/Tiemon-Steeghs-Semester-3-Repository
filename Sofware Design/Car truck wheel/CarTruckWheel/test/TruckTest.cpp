#include "Truck.h"

#include <string>
#include <gtest/gtest.h>

class TruckTest : public ::testing::Test
{

protected:
    TruckTest() 
    { 
        truck = new Truck("Trucker truck truck", "Alloy", 15, 7, 5); 
    }

    virtual ~TruckTest()
    {
        delete truck;
        truck = NULL;
    }


    Truck* truck;
};

TEST_F(TruckTest, TruckCopyConstructorTest) 
{
    truck->SetLicencePlate("AB-1234-CD");
    Truck* CopyTruck = truck;
    EXPECT_EQ(CopyTruck->GetModel(), truck->GetModel());
    EXPECT_EQ(CopyTruck->GetNrWheels(), truck->GetNrWheels());
    EXPECT_EQ(CopyTruck->GetLicencePlate(), truck->GetLicencePlate());
    EXPECT_EQ(CopyTruck->GetPower(), truck->GetPower());
    EXPECT_EQ(CopyTruck->GetWheel(0)->GetDiameter(), truck->GetWheel(0)->GetDiameter());
}

TEST_F(TruckTest, TruckAssignmentOperatorTest) 
{
    Truck* CopyTruck;
    CopyTruck = truck;

    EXPECT_EQ(CopyTruck->GetModel(), truck->GetModel());
    EXPECT_EQ(CopyTruck->GetNrWheels(), truck->GetNrWheels());
    EXPECT_EQ(CopyTruck->GetLicencePlate(), truck->GetLicencePlate());
    EXPECT_EQ(CopyTruck->GetPower(), truck->GetPower());
    EXPECT_EQ(CopyTruck->GetWheel(0)->GetDiameter(), truck->GetWheel(0)->GetDiameter());
}