#include "encoder.h"

static int left_pos{0};
static int right_pos{0};

void Encoder::initEncoders()
{
    pinMode(left_encoder_pin_a, INPUT);
    pinMode(left_encoder_pin_b, INPUT);
    pinMode(right_encoder_pin_a, INPUT);
    pinMode(right_encoder_pin_b, INPUT);

    // setup left encoder interrupt callback
    attachInterrupt(
        digitalPinToInterrupt(left_encoder_pin_a),
        [] { readEncoder(left_encoder_pin_b, left_pos); },
        RISING);

    // setup right encoder interrupt callback
    attachInterrupt(
        digitalPinToInterrupt(right_encoder_pin_a),
        [] { readEncoder(right_encoder_pin_b, right_pos); },
        RISING);
}

// update encoder rotation value based on direction (if b pin is high or not)
void readEncoder(uint8_t encoderBPin, int &pos)
{
    bool b{digitalRead(encoderBPin)};

    if (b)
    {
        ++pos;
    }
    else
    {
        --pos;
    }
}

int Encoder::getLeftPos() { return left_pos; }
int Encoder::getRightPos() { return right_pos; }
