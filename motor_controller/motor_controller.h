#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

namespace MotorController
{
    // set motor speeds in rpm
    int8_t setMotorSpeeds(int16_t left_speed, int16_t right_speed);
    void disableClosedLoopControl();
}
#endif
