#ifndef ENCODER_H
#define ENCODER_H
#include <math.h> // for M_PI

namespace Encoder
{

    // 75:1 gear ratio, 11 ppr (spec sheet), 75*11=825
    constexpr auto encoder_ppr{75 * 11};

    // A pins must support interrupts (pins 2 and 3 on nano)
    constexpr uint8_t left_encoder_a_pin{2};  // green
    constexpr uint8_t left_encoder_b_pin{4};  // yellow
    constexpr uint8_t right_encoder_a_pin{3}; // yellow
    constexpr uint8_t right_encoder_b_pin{5}; // green

    void initEncoders();
    int getLeftCount();
    int getRightCount();
    int getLeftRPM();
    int getRightRPM();
    void resetEncoders();
    void calculateSpeeds();
    void printSpeedInfo();

}
#endif
