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
    Comms::initComms();
}
