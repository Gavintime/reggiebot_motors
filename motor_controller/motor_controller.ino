#include "motor_driver.h"
#include "encoder.h"
#include "comms.h"
#include "motor_controller.h"

static int16_t _target_left_speed{0};
static int16_t _target_right_speed{0};
static bool _closed_loop_control{false};

void setup()
{
    Serial.begin(9600);
    Encoder::initEncoders();
    MotorDriver::initMotors();
    MotorController::setMotorSpeeds(60, 60);
}

void loop()
{
    Comms::runCommsIteration();

    Encoder::calculateSpeeds();

    if (_closed_loop_control)
    {
        runPIDIteration();
    }
    Serial.print(Encoder::getLeftRPM());
    Serial.print(' ');
    Serial.println(MotorDriver::getMotorPower(MotorDriver::LEFT));
}

int8_t MotorController::setMotorSpeeds(int16_t left_speed, int16_t right_speed)
{
    _target_left_speed = left_speed;
    _target_right_speed = right_speed;
    _closed_loop_control = true;
    return 0;
}

void MotorController::disableClosedLoopControl()
{
    _closed_loop_control = false;
}

void runPIDIteration()
{
    // pid weights
    constexpr double kp{7};
    constexpr double ki{10};
    constexpr double kd{0};

    static auto previous_time{micros()};
    static int previous_left_error{0};
    static int previous_right_error{0};
    static double left_integral{0};
    static double right_integral{0};

    auto current_time{micros()};

    // in rpm
    auto current_left_error{_target_left_speed - Encoder::getLeftRPM()};
    auto current_right_error{_target_right_speed - Encoder::getRightRPM()};
    auto dt{((current_time - previous_time) / 1.0e6) / 60};

    // calculate integral and derivative components
    left_integral += current_left_error * dt;
    right_integral += current_right_error * dt;
    auto left_derivative{(current_left_error - previous_left_error) / dt};
    auto right_derivative{(current_right_error - previous_right_error) / dt};

    // calculate control values
    auto left_control{(kp * current_left_error) + (ki * left_integral) + (kd * left_derivative)};
    left_control = max(min(left_control, 255), -255);

    auto right_control{(kp * current_right_error) + (ki * right_integral) + (kd * right_derivative)};
    right_control = max(min(right_control, 255), -255);

    MotorDriver::setMotorPowers(left_control, right_control);

    previous_time = current_time;
    previous_left_error = current_left_error;
    previous_right_error = current_right_error;
}
