#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

namespace MotorDriver
{
    // L298N motor driver connections
    // Motor A
    constexpr uint8_t left_motor_speed_pin{6};
    constexpr uint8_t left_motor_forward_pin{7};
    constexpr uint8_t left_motor_backward_pin{8};
    // Motor B
    constexpr uint8_t right_motor_speed_pin{11};
    constexpr uint8_t right_motor_forward_pin{9};
    constexpr uint8_t right_motor_backward_pin{10};

    enum MotorSelection
    {
        LEFT,
        RIGHT
    };

    void initMotors();
    // motors may stall below 20% power, confirmed stalling at 10% power
    int8_t setMotorSpeed(MotorSelection motor, int8_t motor_speed);
    int8_t setMotorSpeeds(int8_t left_motor_speed, int8_t right_motor_speed);
    int8_t getMotorSpeed(MotorSelection motor);
    void stopMotors();

}
#endif
