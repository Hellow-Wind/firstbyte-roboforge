#include<cv_pkg/vision_node.hpp>
#include<algorithm>

//构造函数
VisionNode::VisionNode() : Node("vision_node")
{
    pub_targets_ = create_publisher<std_msgs::msg::Int32MultiArray>("/target_points", 10);

    sub_image_ = create_subscription<sensor_msgs::msg::Image>("/image_raw",10,
        std::bind(&VisionNode::ImageCallback,this,std::placeholders::_1));

}

//图像处理+寻找坐标+筛选
std::vector<cv::Point> VisionNode::FindTargets(const cv::Mat& imgHSV)
{

    cv::Mat mask,mask_high,mask_low;
    cv::inRange(imgHSV,cv::Scalar(iLowH_m1_,iLowS_m1_,iLowV_m1_),cv::Scalar(iHighH_m1_,iHighS_m1_,iHighV_m1_),mask_high);
    cv::inRange(imgHSV,cv::Scalar(iLowH_m2_,iLowS_m2_,iLowV_m2_),cv::Scalar(iHighH_m2_,iHighS_m2_,iHighV_m2_),mask_low);
    cv::bitwise_or(mask_low,mask_high,mask);

    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(5,5));
    cv::morphologyEx(mask,mask,cv::MORPH_OPEN,element);
    cv::morphologyEx(mask,mask,cv::MORPH_CLOSE,element);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask,contours,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_SIMPLE);

    std::vector<cv::Point> all_targets;
    
    for(size_t i=0;i<contours.size();i++)
    {
        double area = cv::contourArea(contours[i]);

        if(area>min_area&&area<max_area)
        {
            cv::Rect rect = cv::boundingRect(contours[i]);

            int cx = rect.x + rect.width/2;
            int cy = rect.y + rect.height/2;

            all_targets.push_back(cv::Point(cx, cy));
        }


    }

    if(all_targets.empty())return {};

    std::sort(all_targets.begin(), all_targets.end(), 
        [](const cv::Point& a, const cv::Point& b)
        {
            return a.x < b.x;
        });    
    
    
    cv::imshow("Mask", mask); 
    cv::waitKey(1);

    return all_targets;
}

//筛选坐标
std::vector<cv::Point> VisionNode::FilterTargets(const std::vector<cv::Point>& all_targets)
{
    std::vector<cv::Point> final_targets;
    for (auto& p : all_targets)
    {
        if (final_targets.empty())
        {
            final_targets.push_back(p);
        }
        else
        {
            cv::Point last_p = final_targets.back();
        
            int dx = std::abs(p.x - last_p.x);
        
            if (dx < dis_deadzone)
            {
                continue; 
            }
            else
            {
                final_targets.push_back(p);
            }
        }
    }

    RCLCPP_INFO(get_logger(), "原始碎块有 %zu 个，合并后确认 %zu 个真靶子：", all_targets.size(), final_targets.size());



    return final_targets;
}


//画十字
void VisionNode::DrawDebug(cv::Mat& imgOriginal,const std::vector<cv::Point>& targets)
{
    for (size_t i=0; i<targets.size(); i++) {
        cv::drawMarker(imgOriginal,targets[i],cv::Scalar(0,255,0),cv::MARKER_CROSS,20,2);
    }
}


void VisionNode::ImageCallback(const sensor_msgs::msg::Image::SharedPtr msg)
{
    //传图
    cv::Mat imgOriginal = cv_bridge::toCvCopy(msg,"bgr8")->image;

    cv::Mat imgHSV;
    cv::cvtColor(imgOriginal,imgHSV,cv::COLOR_BGR2HSV);

    //图像处理
    std::vector<cv::Point> all_targets = FindTargets(imgHSV);
    std::vector<cv::Point> final_targets = FilterTargets(all_targets);

    //发送坐标
    if (!final_targets.empty())
    {


        auto msg_array = std_msgs::msg::Int32MultiArray();

        for (size_t i=0;i<final_targets.size();i++)
        {



            RCLCPP_INFO(get_logger(), "靶子 %zu: X=%d, Y=%d", i+1, final_targets[i].x, final_targets[i].y);

            msg_array.data.push_back(final_targets[i].x);
            msg_array.data.push_back(final_targets[i].y);
        }


        pub_targets_->publish(msg_array);

        DrawDebug(imgOriginal,final_targets);
    }
    else
    {
        RCLCPP_INFO(get_logger(), "未找到目标");
    }
    cv::imshow("RGB", imgOriginal);
    cv::waitKey(1);



}


