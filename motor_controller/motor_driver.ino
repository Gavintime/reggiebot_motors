#include "motor_driver.h"

// initialize motor pins and turn motors off
void MotorDriver::initMotors()
{
    // Set all the motor control pins to outputs
    pinMode(left_motor_pin_speed, OUTPUT);
    pinMode(left_motor_pin_forward, OUTPUT);
    pinMode(left_motor_pin_backward, OUTPUT);

    pinMode(right_motor_pin_speed, OUTPUT);
    pinMode(right_motor_pin_forward, OUTPUT);
    pinMode(right_motor_pin_backward, OUTPUT);

    // default motors to off
    stopMotors();
}

// takes in 2 motor speeds between -100 and 100, and sets motor pins accordingly
// error code -1 = a motor speed was outside the range of -100 to 100
// TODO: how to deal with minimum real speeds (before stalling)
int8_t MotorDriver::setMotorSpeeds(int8_t leftMotorSpeed, int8_t rightMotorSpeed)
{
    bool leftReverese{false};
    bool rightReverse{false};

    if (leftMotorSpeed < 0)
    {
        leftMotorSpeed *= -1;
        leftReverese = true;
    }

    if (rightMotorSpeed < 0)
    {
        rightMotorSpeed *= -1;
        rightReverse = true;
    }

    // speed requests must be between -100 and 100
    if (leftMotorSpeed > 100 || rightMotorSpeed > 100)
    {
        return -1;
    }

    // map speeds from 0-100 to 0-255
    leftMotorSpeed = map(leftMotorSpeed, 0, 100, 0, 255);
    rightMotorSpeed = map(rightMotorSpeed, 0, 100, 0, 255);

    // set motor speed and direction
    analogWrite(left_motor_pin_speed, leftMotorSpeed);
    analogWrite(right_motor_pin_speed, rightMotorSpeed);

    if (!leftReverese)
    {
        digitalWrite(left_motor_pin_forward, HIGH);
        digitalWrite(left_motor_pin_backward, LOW);
    }
    else
    {
        digitalWrite(left_motor_pin_forward, LOW);
        digitalWrite(left_motor_pin_backward, HIGH);
    }

    if (!rightReverse)
    {
        digitalWrite(right_motor_pin_forward, HIGH);
        digitalWrite(right_motor_pin_backward, LOW);
    }
    else
    {
        digitalWrite(right_motor_pin_forward, LOW);
        digitalWrite(right_motor_pin_backward, HIGH);
    }

    return 0;
}

void MotorDriver::stopMotors()
{
    analogWrite(left_motor_pin_speed, 0);
    analogWrite(right_motor_pin_speed, 0);
    digitalWrite(left_motor_pin_forward, LOW);
    digitalWrite(left_motor_pin_backward, LOW);
    digitalWrite(right_motor_pin_forward, LOW);
    digitalWrite(right_motor_pin_backward, LOW);
}
