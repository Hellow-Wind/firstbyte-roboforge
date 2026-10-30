#ifndef VISION_NODE_HPP
#define VISION_NODE_HPP

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <geometry_msgs/msg/point.hpp>
#include <std_msgs/msg/int32.hpp>
#include <std_msgs/msg/int32_multi_array.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>

class VisionNode:public rclcpp::Node
{
public:
    static constexpr int MODE_3_TARGETS = 1;
    static constexpr int MODE_1_TARGET = 2; 

    VisionNode(const std::string& node_name = "vision_node");

private:
    void ImageCallback(const sensor_msgs::msg::Image::SharedPtr msg);

    void ModeCallback(const std_msgs::msg::Int32::SharedPtr msg);

    std::vector<cv::Point> FindTargets(const cv::Mat& imgHSV);
    std::vector<cv::Point> FilterTargets(const std::vector<cv::Point>& all_targets);
    void DrawDebug(cv::Mat& imgOriginal, const std::vector<cv::Point>& targets);

    //HSV上下限制
    //mask1
    int iLowH_m1_ = 0;
    int iHighH_m1_ = 10;
    int iLowS_m1_ = 100;
    int iHighS_m1_ = 255;
    int iLowV_m1_ = 100;
    int iHighV_m1_ = 255;
    //mask2
    int iLowH_m2_ = 170;
    int iHighH_m2_ = 180;
    int iLowS_m2_ = 100;
    int iHighS_m2_ = 255;
    int iLowV_m2_ = 100;
    int iHighV_m2_ = 255;

    //面积区间
    double min_area = 500.0;
    double max_area = 50000.0;

    //距离死区
    int dis_deadzone = 50;

    //
    int current_mode_ = MODE_3_TARGETS;

    //
    rclcpp::Publisher<std_msgs::msg::Int32MultiArray>::SharedPtr pub_targets_;
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr sub_image_;
    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr sub_mode_;

};

#endif