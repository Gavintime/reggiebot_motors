#include "motor_driver.h"

// between -255 and 255
static int16_t _left_power{0};
static int16_t _right_power{0};

// initialize motor pins and turn motors off
void MotorDriver::initMotors()
{
    // Set all the motor control pins to outputs
    pinMode(left_motor_pwm_pin, OUTPUT);
    pinMode(left_motor_forward_pin, OUTPUT);
    pinMode(left_motor_backward_pin, OUTPUT);

    pinMode(right_motor_pwm_pin, OUTPUT);
    pinMode(right_motor_forward_pin, OUTPUT);
    pinMode(right_motor_backward_pin, OUTPUT);

    // default motors to off
    stopMotors();
}

// takes in a motor power between -255 and 255, and sets motor pins accordingly
// error code -1: a motor power was outside the range of -255 to 255
// error code -2: invalid motor selection
// TODO: how to deal with minimum real power (before stalling)
int8_t MotorDriver::setMotorPower(MotorSelection motor, int16_t motor_power)
{
    // power requests must be between -255 and 255
    if (motor_power > 255 || motor_power < -255)
    {
        return -1;
    }

    // get pins of selected motor and set global power
    uint8_t pwm_pin;
    uint8_t forward_pin;
    uint8_t backward_pin;
    switch (motor)
    {
    case LEFT:
        pwm_pin = left_motor_pwm_pin;
        forward_pin = left_motor_forward_pin;
        backward_pin = left_motor_backward_pin;
        _left_power = motor_power;
        break;
    case RIGHT:
        pwm_pin = right_motor_pwm_pin;
        forward_pin = right_motor_forward_pin;
        backward_pin = right_motor_backward_pin;
        _right_power = motor_power;
        break;
    // impossible motor selection
    default:
        return -2;
    }

    // if reverse, swap pins and make power absolute
    if (motor_power < 0)
    {
        motor_power *= -1;
        auto temp{forward_pin};
        forward_pin = backward_pin;
        backward_pin = temp;
    }

    analogWrite(pwm_pin, motor_power);
    digitalWrite(forward_pin, HIGH);
    digitalWrite(backward_pin, LOW);

    return 0;
}

int8_t MotorDriver::setMotorPower(int16_t left_motor_power, int16_t right_motor_power)
{
    setMotorPower(LEFT, left_motor_power);
    setMotorPower(RIGHT, right_motor_power);
}

int16_t MotorDriver::getMotorPower(MotorSelection motor)
{
    switch (motor)
    {
        case LEFT:
            return _left_power;
        case RIGHT:
            return _right_power;
        default:
            return -2;
    }
}

void MotorDriver::stopMotors()
{
    analogWrite(left_motor_pwm_pin, 0);
    analogWrite(right_motor_pwm_pin, 0);
    digitalWrite(left_motor_forward_pin, LOW);
    digitalWrite(left_motor_backward_pin, LOW);
    digitalWrite(right_motor_forward_pin, LOW);
    digitalWrite(right_motor_backward_pin, LOW);
    _left_power = 0;
    _right_power = 0;
}
