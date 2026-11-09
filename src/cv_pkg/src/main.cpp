#include <cv_pkg/vision_node.hpp>

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);

    auto node = std::make_shared<VisionNode>();

    rclcpp::spin(node);

    rclcpp::shutdown();
    
    return 0;
}