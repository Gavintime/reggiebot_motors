#ifndef ENCODER_H
#define ENCODER_H

namespace Encoder
{
    // all lengths are in millimeters except counts per meter

    // 75:1 gear ratio, 11 ppr (spec sheet), 75*11=825
    constexpr auto encoder_ppr{825};
    // TODO: measure this
    constexpr auto wheel_gap{200}; // guesstimated 8 inches for now
    constexpr double wheel_circumference{204.2}; // 65mm diameter * pi
    constexpr int counts_per_meter{encoder_ppr/wheel_circumference * 1000};

    // A pins must support interrupts (pins 2 and 3 on nano)
    constexpr uint8_t left_encoder_a_pin{2}; // green
    constexpr uint8_t left_encoder_b_pin{4}; // yellow
    constexpr uint8_t right_encoder_a_pin{3}; // yellow
    constexpr uint8_t right_encoder_b_pin{5}; // green

    void initEncoders();
    int getLeftCount();
    int getRightCount();
    void resetEncoders();
    void runOdomIteration();
    void SendOdomInfo();

}
#endif
