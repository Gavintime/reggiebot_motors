#include "motor_driver.h"

// between -255 and 255
static int16_t _left_speed{0};
static int16_t _right_speed{0};

// initialize motor pins and turn motors off
void MotorDriver::initMotors()
{
    // Set all the motor control pins to outputs
    pinMode(left_motor_speed_pin, OUTPUT);
    pinMode(left_motor_forward_pin, OUTPUT);
    pinMode(left_motor_backward_pin, OUTPUT);

    pinMode(right_motor_speed_pin, OUTPUT);
    pinMode(right_motor_forward_pin, OUTPUT);
    pinMode(right_motor_backward_pin, OUTPUT);

    // default motors to off
    stopMotors();
}

// takes in a motor speed between -255 and 255, and sets motor pins accordingly
// error code -1: a motor speed was outside the range of -255 to 255
// error code -2: invalid motor selection
// TODO: how to deal with minimum real speeds (before stalling)
int8_t MotorDriver::setMotorSpeed(MotorSelection motor, int16_t motor_speed)
{
    // speed requests must be between -255 and 255
    if (motor_speed > 255 || motor_speed < -255)
    {
        return -1;
    }

    // get pins of selected motor and set global speed
    uint8_t speed_pin;
    uint8_t forward_pin;
    uint8_t backward_pin;
    switch (motor)
    {
    case LEFT:
        speed_pin = left_motor_speed_pin;
        forward_pin = left_motor_forward_pin;
        backward_pin = left_motor_backward_pin;
        _left_speed = motor_speed;
        break;
    case RIGHT:
        speed_pin = right_motor_speed_pin;
        forward_pin = right_motor_forward_pin;
        backward_pin = right_motor_backward_pin;
        _right_speed = motor_speed;
        break;
    // impossible motor selection
    default:
        return -2;
    }

    // if reverse speed, swap pins and make speed absolute
    if (motor_speed < 0)
    {
        motor_speed *= -1;
        auto temp{forward_pin};
        forward_pin = backward_pin;
        backward_pin = temp;
    }

    analogWrite(speed_pin, motor_speed);
    digitalWrite(forward_pin, HIGH);
    digitalWrite(backward_pin, LOW);

    return 0;
}

int8_t MotorDriver::setMotorSpeeds(int16_t left_motor_speed, int16_t right_motor_speed)
{
    setMotorSpeed(LEFT, left_motor_speed);
    setMotorSpeed(RIGHT, right_motor_speed);
}

int16_t MotorDriver::getMotorSpeed(MotorSelection motor)
{
    switch (motor)
    {
        case LEFT:
            return _left_speed;
        case RIGHT:
            return _right_speed;
        default:
            return -2;
    }
}

void MotorDriver::stopMotors()
{
    analogWrite(left_motor_speed_pin, 0);
    analogWrite(right_motor_speed_pin, 0);
    digitalWrite(left_motor_forward_pin, LOW);
    digitalWrite(left_motor_backward_pin, LOW);
    digitalWrite(right_motor_forward_pin, LOW);
    digitalWrite(right_motor_backward_pin, LOW);
    _left_speed = 0;
    _right_speed = 0;
}
