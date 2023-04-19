#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

namespace MotorDriver
{
    // L298N motor driver connections
    // Motor A
    constexpr uint8_t left_motor_pwm_pin{11}; // white
    constexpr uint8_t left_motor_forward_pin{9}; // red
    constexpr uint8_t left_motor_backward_pin{10}; // brown
    // Motor B
    constexpr uint8_t right_motor_pwm_pin{6}; // brown
    constexpr uint8_t right_motor_forward_pin{7}; // yellow
    constexpr uint8_t right_motor_backward_pin{8}; // orange

    enum MotorSelection
    {
        LEFT,
        RIGHT
    };

    void initMotors();
    // motors may stall below 20% power, confirmed stalling at 10% power
    int8_t setMotorPower(MotorSelection motor, int16_t motor_power);
    int8_t setMotorPower(int16_t left_motor_power, int16_t right_motor_power);
    int16_t getMotorPower(MotorSelection motor);
    void stopMotors();

}
#endif
