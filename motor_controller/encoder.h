#ifndef ENCODER_H
#define ENCODER_H

namespace Encoder
{
    // all lengths are in millimeters except counts per meter

    // TODO: 75:1 gear ratio, 11 ppr (spec sheet), 75*11=825
    constexpr auto encoder_ppr{814};
    // TODO: measure this
    constexpr auto wheel_gap{200}; // guesstimated 8 inches for now
    constexpr double wheel_circumference{204.2}; // 65mm diameter * pi
    constexpr int counts_per_meter{encoder_ppr/wheel_circumference * 1000}; // 3986

    // A pins must support interrupts (pins 2 and 3 on nano)
    constexpr uint8_t left_encoder_a_pin{2}; // yellow
    constexpr uint8_t left_encoder_b_pin{4}; // green
    constexpr uint8_t right_encoder_a_pin{3}; // yellow
    constexpr uint8_t right_encoder_b_pin{5}; // green

    void initEncoders();
    int getLeftCount();
    int getRightCount();
    void sendOdomOverSerial();

}
#endif
