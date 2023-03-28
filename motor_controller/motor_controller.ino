#include "motor_driver.h"
#include "encoder.h"
#include "motor_controller.h"

void setup()
{
    Serial.begin(9600);
    Encoder::initEncoders();
    MotorDriver::initMotors();
    MotorDriver::setMotorSpeeds(50, 50);
    delay(2000);
}

void loop()
{
    int val{Encoder::getLeftPos()};
    // 810 822
    // if (val < -814)
    // {
    //     MotorDriver::stopMotors();
    //     Serial.println("STOPPED");
    // }
    // Serial.println(val);


    // static bool flag{false};
    // if (val < -814 - 10 && !flag)
    // {
    //     MotorDriver::setMotorSpeeds(0, -20);
    //     flag = true;
    // }

    // if (val > 0 && flag)
    // {
    //     MotorDriver::stopMotors();
    // }

    if (val < 0)
    {
        MotorDriver::setMotorSpeeds(0, -20);
    }
    else if (val > 0)
    {
        MotorDriver::setMotorSpeeds(0, 20);
    }


    Serial.println(val);
}
