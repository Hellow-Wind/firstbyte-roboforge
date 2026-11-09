#ifndef SERIAL_BRIDGE_HPP
#define SERIAL_BRIDGE_HPP

#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <std_msgs/msg/int32_multi_array.hpp>
#include <serial/serial.h>

class SerialNode : public rclcpp::Node
{
public:
    SerialNode(const std::string& node_name = "serial_bridge");

private:
    // 两个回调函数，分别处理速度和坐标
    void CmdVelCallback(const geometry_msgs::msg::Twist::SharedPtr msg);
    void TargetCallback(const std_msgs::msg::Int32MultiArray::SharedPtr msg);

    // 发送速度的底层函数（带锁，防止多线程冲突）
    void SendSpeedPacket(int16_t vx, int16_t vz);
    void SendTargetPacket(int16_t tx, int16_t ty);

    serial::Serial serial_port_;
    std::mutex serial_mutex_; // 极其重要：串口是共享资源，必须加互斥锁！

    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr sub_cmd_vel_;
    rclcpp::Subscription<std_msgs::msg::Int32MultiArray>::SharedPtr sub_target_;
};

#endif