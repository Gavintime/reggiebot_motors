#include <libserial/SerialPort.h>
#include <libserial/SerialPortConstants.h>
#include "pluginlib/class_list_macros.hpp"
#include "reggiebot_wheel_interface.hpp"
#include "rclcpp/rclcpp.hpp"
#include "hardware_interface/types/hardware_interface_type_values.hpp"
#include <array>
#include <vector>
#include <cmath>

namespace reggiebot_motors
{
    using CallbackReturn = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;

    /// Initialization of the hardware interface from data parsed from the robot's URDF.
    CallbackReturn ReggiebotWheelInterface::on_init(
        const hardware_interface::HardwareInfo &hardware_info)
    {
        // call parent init, return if not successful
        if (hardware_interface::SystemInterface::on_init(hardware_info) !=
            CallbackReturn::SUCCESS)
        {
            return CallbackReturn::ERROR;
        }

        // verify all joint information is correct
        // 2 joints, each joint has 1 velocity command/state interface
        if (info_.joints.size() != 2)
        {
            RCLCPP_FATAL(rclcpp::get_logger("ReggiebotWheelInterface"),
                         "%zu joints found, 2 expected",
                         info_.joints.size());
            return CallbackReturn::ERROR;
        }

        for (const auto &joint : info_.joints)
        {
            // TODO: support pwm command interfaces
            if (joint.command_interfaces.size() != 1)
            {
                RCLCPP_FATAL(rclcpp::get_logger("ReggiebotWheelInterface"),
                             "Joint '%s' has %zu command interfaces, 1 expected.",
                             joint.name.c_str(), joint.command_interfaces.size());
                return CallbackReturn::ERROR;
            }

            if (joint.command_interfaces[0].name != hardware_interface::HW_IF_VELOCITY)
            {
                RCLCPP_FATAL(rclcpp::get_logger("ReggiebotWheelInterface"),
                             "Joint '%s' has command interface with name '%s', '%s' expected.",
                             joint.name.c_str(), joint.command_interfaces[0].name.c_str(),
                             hardware_interface::HW_IF_VELOCITY);
                return CallbackReturn::ERROR;
            }

            // TODO: support pwm state interfaces
            if (joint.state_interfaces.size() != 1)
            {
                RCLCPP_FATAL(rclcpp::get_logger("ReggiebotWheelInterface"),
                             "Joint '%s' has %zu state interfaces, 1 expected.",
                             joint.name.c_str(), joint.state_interfaces.size());
                return CallbackReturn::ERROR;
            }

            if (joint.state_interfaces[0].name != hardware_interface::HW_IF_VELOCITY)
            {
                RCLCPP_FATAL(rclcpp::get_logger("ReggiebotWheelInterface"),
                             "Joint '%s' has state interface with name '%s', '%s' expected.",
                             joint.name.c_str(), joint.state_interfaces[0].name.c_str(),
                             hardware_interface::HW_IF_VELOCITY);
                return CallbackReturn::ERROR;
            }
        }

        // init member variables
        m_arduino_port = std::shared_ptr<LibSerial::SerialPort>(new LibSerial::SerialPort());
        m_hw_rpm_commands = std::array<double, 2>{0, 0};
        // m_hw_power_commands = std::array<double, 2>{0, 0};
        m_hw_rpms = std::array<double, 2>{0, 0};
        // m_hw_powers = std::array<double, 2>{0, 0};
        // TODO: set this from params
        m_arduino_file = "/dev/arduino-motor";

        return CallbackReturn::SUCCESS;
    }

    CallbackReturn ReggiebotWheelInterface::on_configure(
        const rclcpp_lifecycle::State & /*previous_state*/)
    {
        // setup communication to hardware so it can be activated
        return CallbackReturn::SUCCESS;
    }

    CallbackReturn ReggiebotWheelInterface::on_cleanup(
        const rclcpp_lifecycle::State & /*previous_state*/)
    {
        // do the opposite of on_configure()
        return CallbackReturn::SUCCESS;
    }

    CallbackReturn ReggiebotWheelInterface::on_activate(
        const rclcpp_lifecycle::State & /*previous_state*/)
    {
        // TODO: catch exception and return error
        m_arduino_port->Open(m_arduino_file);
        // these must happen after the port is opened
        m_arduino_port->SetBaudRate(LibSerial::BaudRate::BAUD_9600);
        m_arduino_port->SetCharacterSize(LibSerial::CharacterSize::CHAR_SIZE_8);
        m_arduino_port->SetFlowControl(LibSerial::FlowControl::FLOW_CONTROL_NONE);
        m_arduino_port->SetParity(LibSerial::Parity::PARITY_NONE);
        m_arduino_port->SetStopBits(LibSerial::StopBits::STOP_BITS_1);

        // allow the arduino enough time to start
        sleep(2);

        RCLCPP_INFO(rclcpp::get_logger("ReggiebotWheelInterface"),
                    "Wheel controller successfully activated!");

        return CallbackReturn::SUCCESS;
    }

    CallbackReturn ReggiebotWheelInterface::on_deactivate(
        const rclcpp_lifecycle::State & /*previous_state*/)
    {
        // TODO: Close() settings are lost on close, is this okay?
        m_arduino_port->Close();
        RCLCPP_INFO(rclcpp::get_logger("ReggiebotWheelInterface"),
                    "Wheel controller successfully deactivated.");
        return CallbackReturn::SUCCESS;
    }

    CallbackReturn ReggiebotWheelInterface::on_shutdown(
        const rclcpp_lifecycle::State & /*previous_state*/)
    {
        // TODO: difference between on_deactivate() and on_shutdown()?
        // TODO: Close() settings are lost on close, is this okay?
        m_arduino_port->Close();
        return CallbackReturn::SUCCESS;
    }

    CallbackReturn ReggiebotWheelInterface::on_error(
        const rclcpp_lifecycle::State & /*previous_state*/)
    {
        // TODO: handle errors from all states
        return CallbackReturn::SUCCESS;
    }

    std::vector<hardware_interface::StateInterface> ReggiebotWheelInterface::export_state_interfaces()
    {
        // full interface names have structure <joint_name>/<interface_type>
        std::vector<hardware_interface::StateInterface> state_interfaces{};

        // add both state interfaces
        for (uint i{0}; i < info_.joints.size(); ++i)
        {
            state_interfaces.emplace_back(hardware_interface::StateInterface(
                info_.joints[i].name, hardware_interface::HW_IF_VELOCITY, &m_hw_rpms[i]));
        }

        return state_interfaces;
    }

    std::vector<hardware_interface::CommandInterface> ReggiebotWheelInterface::export_command_interfaces()
    {
        // full interface names have structure <joint_name>/<interface_type>
        std::vector<hardware_interface::CommandInterface> command_interfaces{};

        // add both command interfaces
        for (uint i{0}; i < info_.joints.size(); ++i)
        {
            command_interfaces.emplace_back(hardware_interface::CommandInterface(
                info_.joints[i].name, hardware_interface::HW_IF_VELOCITY, &m_hw_rpm_commands[i]));
        }

        return command_interfaces;
    }

    hardware_interface::return_type ReggiebotWheelInterface::prepare_command_mode_switch(
        const std::vector<std::string> & /*start_interfaces*/,
        const std::vector<std::string> & /*stop_interfaces*/)
    {
        // TODO: for switching between open and closed loop control
        return hardware_interface::return_type::OK;
    }

    hardware_interface::return_type ReggiebotWheelInterface::perform_command_mode_switch(
        const std::vector<std::string> & /*start_interfaces*/,
        const std::vector<std::string> & /*stop_interfaces*/)
    {
        // TODO: for switching between open and closed loop control
        return hardware_interface::return_type::OK;
    }

    hardware_interface::return_type ReggiebotWheelInterface::read(
        const rclcpp::Time & /*time*/, const rclcpp::Duration & /*period*/)
    {
        // request current motor speeds
        m_arduino_port->Write("o\n");
        std::string response;
        m_arduino_port->ReadLine(response, '\n');

        // parse response
        int left_rpm{atoi(response.c_str())};
        // get pointer to first ' ' char after the left rpm,
        // which will be just before the right rpm
        int right_rpm{atoi(strchr(response.c_str(), ' ') + 1)};

        m_hw_rpms[0] = left_rpm;
        m_hw_rpms[1] = right_rpm;

        return hardware_interface::return_type::OK;
    }

    hardware_interface::return_type ReggiebotWheelInterface::write(
        const rclcpp::Time &/*time*/, const rclcpp::Duration & /*period*/)
    {

        std::string rpm_command{"c "};
        rpm_command.append(std::to_string(static_cast<int>(std::round(m_hw_rpm_commands[0]))));
        rpm_command.append(" ");
        rpm_command.append(std::to_string(static_cast<int>(std::round(m_hw_rpm_commands[1]))));

        // TODO: handle cases where arduino port is closed/disconnected etc
        m_arduino_port->Write(rpm_command + '\n');

        return hardware_interface::return_type::OK;
    }
}

PLUGINLIB_EXPORT_CLASS(reggiebot_motors::ReggiebotWheelInterface,
                       hardware_interface::SystemInterface)
