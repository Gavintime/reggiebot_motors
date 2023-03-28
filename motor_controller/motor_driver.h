#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

namespace MotorDriver
{
    // L298N motor driver connections
    // Motor A
    constexpr uint8_t left_motor_pin_speed{6};
    constexpr uint8_t left_motor_pin_forward{7};
    constexpr uint8_t left_motor_pin_backward{8};
    // Motor B
    constexpr uint8_t right_motor_pin_speed{11};
    constexpr uint8_t right_motor_pin_forward{9};
    constexpr uint8_t right_motor_pin_backward{10};

    void initMotors();
    // motors may stall below 20% power, confirmed stalling at 10% power
    int8_t setMotorSpeeds(int8_t leftMotorSpeed, int8_t rightMotorSpeed);
    void stopMotors();
}
#endif
