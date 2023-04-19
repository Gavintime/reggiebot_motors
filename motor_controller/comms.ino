#include "motor_driver.h"
#include "encoder.h"
#include "motor_controller.h"
#include "comms.h"

void Comms::runCommsIteration()
{
    // init message array to all 0s (\0)
    // +1 is so that there is always a \0 at the end
    static char message[max_message_length + 1]{0};
    static int8_t msg_index{0};

    // message received from pi
    if (Serial.available())
    {
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
                MotorController::disableClosedLoopControl();
                MotorDriver::stopMotors();
                break;
            case set_motor_powers:
                _setMotorPowers(message, msg_index);
                break;
            case get_motor_powers:
                _printMotorPowers();
                break;
            case get_motor_speeds:
                Encoder::printSpeedInfo();
                break;
            case set_motor_speeds:
                _setMotorSpeeds(message, msg_index);
                break;
            }

            msg_index = 0;
        }
        // message was maxed out before receiving terminator (\n)
        else if (msg_index >= max_message_length)
        {
            msg_index = 0;
            Serial.println("ERROR, TERMINATOR EXPECTED");
        }
    }
}

void _printEncoders()
{
    Serial.print(Encoder::getLeftCount());
    Serial.print(' ');
    Serial.println(Encoder::getRightCount());
}

void _printMotorPowers()
{
    Serial.print(MotorDriver::getMotorPower(MotorDriver::LEFT));
    Serial.print(' ');
    Serial.println(MotorDriver::getMotorPower(MotorDriver::RIGHT));
}

int8_t _setMotorPowers(const char *msg, int8_t end_index)
{

    // verify msg has required space and is large enough
    if (msg[1] != ' ' || end_index < 5)
    {
        return -1;
    }

    int16_t left_power(atoi(msg + 2));
    // get pointer to first ' ' char after the left power,
    // which will be just before the right power
    int16_t right_power(atoi(strchr(msg + 3, ' ') + 1));

    MotorController::disableClosedLoopControl();
    MotorDriver::setMotorPowers(left_power, right_power);
    return 0;
}

int8_t _setMotorSpeeds(const char *msg, int8_t end_index)
{
    // verify msg has required space and is large enough
    if (msg[1] != ' ' || end_index < 5)
    {
        return -1;
    }

    int16_t left_speed(atoi(msg + 2));
    // get pointer to first ' ' char after the left speed,
    // which will be just before the right speed
    int16_t right_speed(atoi(strchr(msg + 3, ' ') + 1));

    MotorController::setMotorSpeeds(left_speed, right_speed);
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
