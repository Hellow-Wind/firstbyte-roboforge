#include<rclcpp/rclcpp.hpp>
#include<std_msgs/msg/int32.hpp>

class NumberPublisherNode:public rclcpp::Node
{
public:
    NumberPublisherNode():Node("number_publisher_node")
    {
        pub_num_ = create_publisher<std_msgs::msg::Int32>("/num_pub",10);
        timer_ = create_wall_timer(std::chrono::milliseconds(1000),std::bind(&NumberPublisherNode::PubNum,this));
    }

private:
    void PubNum()
    {
        num_.data = 666;
        pub_num_->publish(num_);
    }
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr pub_num_;
    std_msgs::msg::Int32 num_;
    rclcpp::TimerBase::SharedPtr timer_;
    
};

int main(int argc,char**argv)
{
    rclcpp::init(argc,argv);
    auto node = std::make_shared<NumberPublisherNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}