#include "motor_driver.h"
#include "encoder.h"
#include "comms.h"
#include "motor_controller.h"

static int16_t _target_left_speed{};
static int16_t _target_right_speed{};
static bool _closed_loop_control{false};
static double _left_integral{};
static double _right_integral{};
static unsigned long _previous_time{};
static int _prev_left_rpm{};
static int _prev_right_rpm{};
static double _left_rpm_filtered{};
static double _right_rpm_filtered{};

void setup()
{
    Serial.begin(9600);
    Encoder::initEncoders();
    MotorDriver::initMotors();
}

void loop()
{

    Comms::runCommsIteration();

    Encoder::calculateSpeeds();

    if (_closed_loop_control)
    {
        // TODO: add watchdog to closed and open loop control
        runPIDIteration();
        Serial.print(_left_rpm_filtered);
        Serial.print(' ');
        Serial.println(_right_rpm_filtered);
    }
    else
    {
        // doubles to match output of closed loop info above
        Serial.print((double)Encoder::getLeftRPM());
        Serial.print(' ');
        Serial.println((double)Encoder::getRightRPM());
    }
}

int8_t MotorController::setMotorSpeeds(int16_t left_speed, int16_t right_speed)
{

    // if starting closed loop control, not just updating speeds
    // when the pid loop is started, some values need set to defaults
    if (!_closed_loop_control)
    {
        _closed_loop_control = true;
        _previous_time = micros();
        _prev_left_rpm = Encoder::getLeftRPM();
        _prev_right_rpm = Encoder::getRightRPM();
        _left_rpm_filtered = _prev_left_rpm;
        _right_rpm_filtered = _prev_right_rpm;
        _left_integral = 0;
        _right_integral = 0;
    }

    // TODO: set these to measured limits
    _target_left_speed = min(max(left_speed, 255), -255);
    _target_right_speed = min(max(right_speed, 255), -255);
    return 0;
}

void MotorController::disableClosedLoopControl()
{
    _closed_loop_control = false;
}

void runPIDIteration()
{
    // pid weights
    constexpr double kp{5};
    constexpr double ki{600};

    auto current_time{micros()};

    auto left_rpm{Encoder::getLeftRPM()};
    auto right_rpm{Encoder::getRightRPM()};

    // low pass filter (25hz cutoff), credit Curio Res on Youtube
    _left_rpm_filtered = 0.854 * _left_rpm_filtered + 0.0728 * left_rpm + 0.0728 * _prev_left_rpm;
    _right_rpm_filtered = 0.854 * _right_rpm_filtered + 0.0728 * right_rpm + 0.0728 * _prev_right_rpm;

    // calculate deltas
    auto current_left_error{_target_left_speed - _left_rpm_filtered};
    auto current_right_error{_target_right_speed - _right_rpm_filtered};
    auto dt{((current_time - _previous_time) / 1.0e6) / 60};

    // calculate derivative components
    _left_integral += current_left_error * dt;
    _right_integral += current_right_error * dt;

    // calculate control values
    auto left_control{kp * current_left_error + ki * _left_integral};
    left_control = max(min(left_control, 255), -255);

    auto right_control{kp * current_right_error + ki * _right_integral};
    right_control = max(min(right_control, 255), -255);

    MotorDriver::setMotorPowers(left_control, right_control);

    _previous_time = current_time;
    _prev_left_rpm = left_rpm;
    _prev_right_rpm = right_rpm;
}

void MotorController::printSpeedInfo()
{

    if (_closed_loop_control)
    {
        Serial.print(_left_rpm_filtered);
        Serial.print(' ');
        Serial.println(_right_rpm_filtered);
    }
    else
    {
        // doubles to match output of closed loop info above
        Serial.print((double)Encoder::getLeftRPM());
        Serial.print(' ');
        Serial.println((double)Encoder::getRightRPM());
    }
}
