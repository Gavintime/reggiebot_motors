#include "encoder.h"

// these are moduloed by encoder ppr in sendOdomOverSerial() to prevent overflows
static int _left_count{0};
static int _right_count{0};

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

// update encoder rotation value based on direction (if b pin is high or not)
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

// Sends linear velocities(x,y) and orientation(yaw) over serial
void Encoder::sendOdomOverSerial()
{
    // TODO: disable interrupts, or copy left/right pos at start etc
    static auto previous_time{millis()};
    static auto previous_left_count{_left_count};
    static auto previous_right_count{_right_count};
    // static int X{0};
    // static int Y{0};
    static double yaw{0};

    auto current_time{millis()};

    // calculate deltas in terms of time
    // time is in msecs, distance is in mm
    int dt{current_time - previous_time};
    int dleft{((_left_count - previous_left_count) / (double)counts_per_meter) * 1000};
    int dright{((_right_count - previous_right_count) / (double)counts_per_meter) * 1000};
    auto dforward{(dleft + dright) / 2};
    int dx{dforward * sin(yaw)};
    int dy{dforward * cos(yaw)};
    auto dyaw{(dright - dleft) / (double)wheel_gap};

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
    yaw += dyaw;

    // calculate velocity in odom frame
    auto Vx{dx / dt};
    auto Vy{dy / dt};

    Serial.print(Vx);
    Serial.print(' ');
    Serial.print(Vy);
    Serial.print(' ');
    Serial.print(yaw);
    Serial.print('\n');

    // prevent encoder counters from overflowing
    _left_count %= encoder_ppr;
    _right_count %= encoder_ppr;
    // update info for next iteration
    previous_time = current_time;
    previous_left_count = _left_count;
    previous_right_count = _right_count;
}
