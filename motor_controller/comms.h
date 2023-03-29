#ifndef COMMS_H
#define COMMS_H

namespace Comms
{
    constexpr int8_t max_message_length{12};

    // encoder commands
    constexpr char read_encoders{'r'};
    constexpr char reset_encoders{'b'};

    // motor commands
    constexpr char stop_motors{'e'};
    constexpr char set_motor_speeds{'s'}; // s +255 +255
    constexpr char get_motor_speeds{'g'};

    void initComms();
}
#endif
