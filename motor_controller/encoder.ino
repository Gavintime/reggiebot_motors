#include <math.h> // fmod and M_PI
#include "encoder.h"

static int _left_count{0};
static int _right_count{0};
static double _yaw{0};
static double _Vx{0};
static double _Vy{0};

void Encoder::initEncoders()
{
    pinMode(left_encoder_a_pin, INPUT);
    pinMode(left_encoder_b_pin, INPUT);
    pinMode(right_encoder_a_pin, INPUT);
    pinMode(right_encoder_b_pin, INPUT);

    // setup left encoder interrupt callback
    attachInterrupt(
        digitalPinToInterrupt(left_encoder_a_pin),
        []
        { _readEncoder(left_encoder_b_pin, _left_count); },
        RISING);

    // setup right encoder interrupt callback
    attachInterrupt(
        digitalPinToInterrupt(right_encoder_a_pin),
        []
        { _readEncoder(right_encoder_b_pin, _right_count); },
        RISING);

    return;
}

// ISR to update encoder count value based on direction (if b pin is high or not)
void _readEncoder(uint8_t encoder_b_pin, int &encoder_counter)
{
    if (digitalRead(encoder_b_pin))
    {
        ++encoder_counter;
    }
    else
    {
        --encoder_counter;
    }

    return;
}

int Encoder::getLeftCount() { return _left_count; }
int Encoder::getRightCount() { return _right_count; }

void Encoder::resetEncoders()
{
    _left_count = 0;
    _right_count = 0;
}

// Sends linear velocities(x,y) and orientation(yaw) over serial
void Encoder::runOdomIteration()
{
    // TODO: disable interrupts, or copy left/right pos at start etc
    static auto previous_time{millis()};
    // Serial.print("previous_time: ");
    // Serial.print(previous_time);
    static auto previous_left_count{_left_count};
    // Serial.print(" previous_left_count: ");
    // Serial.print(previous_left_count);
    static auto previous_right_count{_right_count};
    // Serial.print(" previous_right_count: ");
    // Serial.print(previous_right_count);
    // static int X{0};
    // static int Y{0};
    // static double yaw{0};
    // Serial.print(" yaw: ");
    // Serial.print(yaw);

    auto current_time{millis()};
    // Serial.print(" current_time: ");
    // Serial.print(current_time);

    // calculate deltas in terms of time
    // time is in secs, distance is in mm
    auto dt{(current_time - previous_time) / (double)1000};
    // Serial.print(" dt: ");
    // Serial.print(dt);
    int dleft{((_left_count - previous_left_count) / (double)counts_per_meter) * 1000};
    // Serial.print(" dleft: ");
    // Serial.print(dleft);
    int dright{((_right_count - previous_right_count) / (double)counts_per_meter) * 1000};
    // Serial.print(" dright: ");
    // Serial.print(dright);
    auto dforward{(dleft + dright) / 2};
    // Serial.print(" dforward: ");
    // Serial.print(dforward);
    int dx{dforward * sin(_yaw)};
    // Serial.print(" dx: ");
    // Serial.print(dx);
    int dy{dforward * cos(_yaw)};
    // Serial.print(" dy: ");
    // Serial.print(dy);
    auto dyaw{(dright - dleft) / (double)wheel_gap};
    // Serial.print(" dyaw: ");
    // Serial.print(dyaw);

    // // calculate robot frame velocities in mm/s
    // // velocity of each wheel
    // int Vleft{dleft / dt};
    // int Vright{dright / dt};
    // // Vforward is velocity in x direction in robot frame
    // int Vforward{(Vleft + Vright) / 2};
    // int Vyaw{(Vright - Vleft) / wheel_gap};

    // // calculate position in odom frame
    // double dyaw{Vyaw * dt};
    // // At startup, X is forward and Y is left
    // X += dx;
    // Y += dy;
    _yaw += dyaw;
    _yaw = fmod(_yaw, 2 * M_PI);
    // Serial.print(" yaw: ");
    // Serial.print(yaw);

    // calculate velocity in odom frame
    _Vx = dx / dt;
    // Serial.print(" Vx: ");
    // Serial.print(Vx);
    _Vy = dy / dt;
    // Serial.print(" Vy: ");
    // Serial.print(Vy);

    // Serial.print(Vx);
    // Serial.print(' ');
    // Serial.print(Vy);
    // Serial.print(' ');
    // Serial.print(yaw);
    // Serial.print('\n');

    // prevent encoder counters from overflowing
    // _left_count %= encoder_ppr;
    // _right_count %= encoder_ppr;
    // update info for next iteration
    previous_time = current_time;
    previous_left_count = _left_count;
    previous_right_count = _right_count;
}

void Encoder::SendOdomInfo()
{
    Serial.print(_Vx);
    Serial.print(' ');
    Serial.print(_Vy);
    Serial.print(' ');
    Serial.print(_yaw);
    Serial.print('\n');
}
