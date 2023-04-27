#ifndef REGGIEBOT_WHEEL_INTERFACE_HPP
#define REGGIEBOT_WHEEL_INTERFACE_HPP

#include <libserial/SerialPort.h>
#include "hardware_interface/system_interface.hpp"
#include "rclcpp_lifecycle/state.hpp"
#include "hardware_interface/hardware_info.hpp"
#include "hardware_interface/handle.hpp"
#include "hardware_interface/types/hardware_interface_return_values.hpp"
#include <memory>
#include <array>

namespace reggiebot_motors
{
    class ReggiebotWheelInterface : public hardware_interface::SystemInterface
    {
    private:
        std::shared_ptr<LibSerial::SerialPort> m_arduino_port;
        std::array<double, 2> m_hw_rpm_commands;   // rpm commands to controller (closed loop/PID)
        // std::array<int, 2> m_hw_power_commands; // pwm commands to controller (open loop)
        std::array<double, 2> m_hw_rpms;           // read wheel rpm
        // std::array<int, 2> m_hw_powers;         // read wheel power as pwm
        std::string m_arduino_file;

    public:
        ReggiebotWheelInterface() = default;

        // LifecycleNodeInterface
        CallbackReturn on_configure(const rclcpp_lifecycle::State &previous_state) override;
        CallbackReturn on_cleanup(const rclcpp_lifecycle::State &previous_state) override;
        CallbackReturn on_shutdown(const rclcpp_lifecycle::State &previous_state) override;
        CallbackReturn on_activate(const rclcpp_lifecycle::State &previous_state) override;
        CallbackReturn on_deactivate(const rclcpp_lifecycle::State &previous_state) override;
        CallbackReturn on_error(const rclcpp_lifecycle::State &previous_state) override;

        // SystemInterface
        CallbackReturn on_init(const hardware_interface::HardwareInfo &hardware_info) override;
        std::vector<hardware_interface::StateInterface> export_state_interfaces() override;
        std::vector<hardware_interface::CommandInterface> export_command_interfaces() override;
        hardware_interface::return_type prepare_command_mode_switch(
            const std::vector<std::string> &start_interfaces,
            const std::vector<std::string> &stop_interfaces) override;
        hardware_interface::return_type perform_command_mode_switch(
            const std::vector<std::string> &start_interfaces,
            const std::vector<std::string> &stop_interfaces) override;
        hardware_interface::return_type read(
            const rclcpp::Time &time, const rclcpp::Duration &period) override;
        hardware_interface::return_type write(
            const rclcpp::Time &time, const rclcpp::Duration &period) override;
    };
}

#endif
