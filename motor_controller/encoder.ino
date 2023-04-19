#include <math.h> // fmod and M_PI
#include "encoder.h"

static int _left_count{0};
static int _right_count{0};
// static double _yaw{0};
// in mm per second
// static double _Vx{0};
// static double _Vy{0};
static int _Vleft{0};
static int _Vright{0};

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
int Encoder::getLeftSpeed() { return _Vleft; }
int Encoder::getRightSpeed() { return _Vright; }

void Encoder::resetEncoders()
{
    _left_count = 0;
    _right_count = 0;
}

// Sends linear velocities(x,y) and orientation(yaw) over serial
void Encoder::calculateSpeeds()
{
    // TODO: disable interrupts, or copy left/right pos at start etc
    static auto previous_time{millis()};
    static auto previous_left_count{_left_count};
    static auto previous_right_count{_right_count};

    auto current_time{millis()};

    // calculate deltas
    // time is in secs, distance is in mm
    auto dt{(current_time - previous_time) / (double)1000};
    int dleft{((_left_count - previous_left_count) / (double)counts_per_meter) * 1000};
    int dright{((_right_count - previous_right_count) / (double)counts_per_meter) * 1000};

    // calculate wheel velocities
    _Vleft = dleft / dt;
    _Vright = dright / dt;

    // prevent encoder counters from overflowing
    // TODO: review this
    // _left_count %= encoder_ppr;
    // _right_count %= encoder_ppr;
    // update info for next iteration
    previous_time = current_time;
    previous_left_count = _left_count;
    previous_right_count = _right_count;
}

void Encoder::printSpeedInfo()
{
    Serial.print(_Vleft);
    Serial.print(' ');
    Serial.println(_Vright);

}
