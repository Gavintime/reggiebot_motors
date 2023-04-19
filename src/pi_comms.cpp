#include <libserial/SerialPort.h>
#include <libserial/SerialPortConstants.h>
#include <iostream>
#include <string>

std::string getCommandFromUser()
{
    std::string str;
    std::cout << "> ";
    getline(std::cin, str);
    return str;
}

int main()
{
    // connect to arduino over serial port and set serial parameters
    LibSerial::SerialPort arduino_port("/dev/arduino-motor");
    arduino_port.SetBaudRate(LibSerial::BaudRate::BAUD_9600);
    arduino_port.SetCharacterSize(LibSerial::CharacterSize::CHAR_SIZE_8);
    arduino_port.SetFlowControl(LibSerial::FlowControl::FLOW_CONTROL_NONE);
    arduino_port.SetParity(LibSerial::Parity::PARITY_NONE);
    arduino_port.SetStopBits(LibSerial::StopBits::STOP_BITS_1);

    while (true)
    {
        std::string command{getCommandFromUser()};
        arduino_port.Write(command + '\n');

        // get response if get encoder count or get motor speed command
        if (command[0] == 'r' || command[0] == 'g' || command[0] == 'o')
        {
            std::string response;
            arduino_port.ReadLine(response, '\n');
            std::cout << response;
        }
    }
}
