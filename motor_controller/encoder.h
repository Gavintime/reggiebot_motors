#ifndef ENCODER_H
#define ENCODER_H

namespace Encoder
{
    // TODO: 75:1 gear ratio, 11 ppr (spec sheet), 75*11=825
    constexpr int encoder_ppr{814};

    // A pins must support interrupts (pins 2 and 3 on nano)
    constexpr uint8_t left_encoder_pin_a{2}; // yellow
    constexpr uint8_t left_encoder_pin_b{4}; // green
    constexpr uint8_t right_encoder_pin_a{3}; // yellow
    constexpr uint8_t right_encoder_pin_b{5}; // green

    void initEncoders();
    int getLeftPos();
    int getRightPos();
}
#endif
