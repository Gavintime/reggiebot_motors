#include "motor_driver.h"
#include "encoder.h"
#include "comms.h"
#include "motor_controller.h"

void setup()
{
    Serial.begin(9600);
    Encoder::initEncoders();
    MotorDriver::initMotors();
}

void loop()
{
    static auto previous_time{millis()};
    static constexpr auto odom_interval{(1 / (double)60) * 1000};

    Comms::runCommsIteration();

    const auto current_time{millis()};

    // run odom at 60hz
    if (current_time - previous_time > odom_interval)
    {
        previous_time = current_time;
        Encoder::calculateSpeeds();
    }
}
