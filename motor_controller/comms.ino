#include "motor_driver.h"
#include "encoder.h"
#include "comms.h"

void Comms::initComms()
{
    // init message array to all 0s
    char message[max_message_length]{0};
    int8_t msg_index{0};

    // Serial.begin(9600);

    while (true)
    {
        // message received from pi
        if (Serial.available())
        {
            // auto msg{Serial.readStringUntil('\n')};
            char c{Serial.read()};
            message[msg_index++] = c;

            // terminator found, end of message
            if (c == '\n')
            {
                switch (message[0])
                {
                case read_encoders:
                    _printEncoders();
                    break;
                case reset_encoders:
                    Encoder::resetEncoders();
                    break;
                case stop_motors:
                    MotorDriver::stopMotors();
                    break;
                case set_motor_speeds:
                    _setMotorSpeeds(message, msg_index);
                    break;
                case get_motor_speeds:
                    _printMotorSpeeds();
                    break;
                }

                msg_index = 0;
            }
            // message length maxed out before receiving terminator (\n)
            else if (msg_index >= max_message_length)
            {
                msg_index = 0;
                Serial.println("ERROR, TERMINATOR EXPECTED");
            }
        }
    }
}

void _printEncoders()
{
    Serial.print(Encoder::getLeftCount());
    Serial.print(' ');
    Serial.println(Encoder::getRightCount());
}

void _printMotorSpeeds()
{
    Serial.print(MotorDriver::getMotorSpeed(MotorDriver::LEFT));
    Serial.print(' ');
    Serial.println(MotorDriver::getMotorSpeed(MotorDriver::RIGHT));
}

int8_t _setMotorSpeeds(const char *msg, int8_t end_index)
{

    if (msg[1] != ' ' || msg[6] != ' ' || end_index < 11)
    {
        return -1;
    }

    // TODO: allow for ommiting 100s and 10s place if 0 from message
    int16_t left_speed{(msg[3] - 48) * 100 + (msg[4] - 48) * 10 + (msg[5] - 48)};
    int16_t right_speed{(msg[8] - 48) * 100 + (msg[9] - 48) * 10 + (msg[10] - 48)};

    if (left_speed > 255 || right_speed > 255)
    {
        return -2;
    }

    if (msg[2] == '-')
    {
        left_speed *= -1;
    }
    else if (msg[2] != '+')
    {
        return -3;
    }

    if (msg[7] == '-')
    {
        right_speed *= -1;
    }
    else if (msg[7] != '+')
    {
        return -3;
    }

    MotorDriver::setMotorSpeeds(left_speed, right_speed);
    return 0;
}

void _printMessage(const char *msg, int8_t end_index)
{
    for (int8_t i{0}; i < end_index; ++i)
    {
        Serial.print(msg[i]);
    }
    Serial.println();
}
