#include "encoder.h"

volatile static int _left_count{0};
volatile static int _right_count{0};
static int _left_rpm{0};
static int _right_rpm{0};

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
void _readEncoder(uint8_t encoder_b_pin, volatile int &encoder_counter)
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
int Encoder::getLeftRPM() { return _left_rpm; }
int Encoder::getRightRPM() { return _right_rpm; }

void Encoder::resetEncoders()
{
    _left_count = 0;
    _right_count = 0;
}

// Sends linear velocities(x,y) and orientation(yaw) over serial
void Encoder::calculateSpeeds()
{
    // TODO: disable interrupts, or copy left/right pos at start etc
    static auto previous_time{micros()};
    static auto previous_left_count{_left_count};
    static auto previous_right_count{_right_count};

    const auto current_time{micros()};

    // calculate deltas
    auto dt{((current_time - previous_time) / 1.0e6) / 60};
    auto dleft{(_left_count - previous_left_count) / (double)encoder_ppr};
    auto dright{(_right_count - previous_right_count) / (double)encoder_ppr};

    // calculate wheel velocities in rpm
    _left_rpm = round(dleft / dt);
    _right_rpm = round(dright / dt);

    // update info for next iteration
    previous_time = current_time;
    previous_left_count = _left_count;
    previous_right_count = _right_count;
}

