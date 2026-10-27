#ifndef VISION_NODE_HPP
#define VISION_NODE_HPP

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <geometry_msgs/msg/point.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>

class VisionNode:public rclcpp::Node
{
public:
    VisionNode();
private:
    void ImageCallback(const sensor_msgs::msg::Image::SharedPtr msg);

    //HSV上下限制
    int iLowH_ = 0;
    int iHighH_ = 255;
    int iLowS_ = 0;
    int iHighS_ = 255;
    int iLowV_ = 0;
    int iHighV_ = 255;

    //面积区间
    double min_area = 500.0;
    double max_area = 5000.0;

    //距离死区
    int dis_deadzone = 50;

    //
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr sub_image_;
    rclcpp::Publisher<geometry_msgs::msg::Point>::SharedPtr pub_point_;
};


#endif