#include<cv_pkg/vision_node.hpp>

VisionNode::VisionNode() : Node("vision_node")
{
    pub_point_ = create_publisher<geometry_msgs::msg::Point>("/target_point",10);

    sub_image_ = create_subscription<sensor_msgs::msg::Image>("/image_raw",10,
        std::bind(&VisionNode::ImageCallback,this,std::placeholders::_1));

}
void VisionNode::ImageCallback(const sensor_msgs::msg::Image::SharedPtr msg)
{
    cv::Mat imgOriginal = cv_bridge::toCvCopy(msg,"bgr8")->image;

    cv::Mat imgHSV;
    cv::cvtColor(imgOriginal,imgHSV,cv::COLOR_BGR2HSV);

    cv::Mat mask,mask_high,mask_low;
    cv::inRange(imgHSV,cv::Scalar(iLowH_,iLowS_,iLowV_),cv::Scalar(iHighH_,iHighS_,iHighV_),mask_high);
    cv::inRange(imgHSV,cv::Scalar(iLowH_,iLowS_,iLowV_),cv::Scalar(iHighH_,iHighS_,iHighV_),mask_low);
    cv::bitwise_and(mask,mask_high,mask_low);

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

        if (!all_targets.empty())
        {
            std::sort(all_targets.begin(), all_targets.end(), 
                [](const cv::Point& a, const cv::Point& b)
                {
                    return a.x < b.x;
                });
            std::vector<cv::Point> final_targets;
        
            for (auto& p : all_targets) {
                if (final_targets.empty())
                {
                    final_targets.push_back(p);
                }
                else
                {
                    cv::Point last_p = final_targets.back();
                
                    int dx = std::abs(p.x - last_p.x);
                    int dy = std::abs(p.y - last_p.y);
                
                    if (dx < dis_deadzone && dy < dis_deadzone)
                    {
                        continue; 
                    }
                    else
                    {
                        final_targets.push_back(p);
                    }
                }
            }

            //RCLCPP_INFO(get_logger(), "原始碎块有 %zu 个，合并后确认 %zu 个真靶子：", all_targets.size(), final_targets.size());
            for (size_t i=0;i<final_targets.size();i++)
            {
                RCLCPP_INFO(get_logger(), "靶子 %zu: X=%d, Y=%d", i+1, final_targets[i].x, final_targets[i].y);
            }
        }
        else
        {
            RCLCPP_INFO(get_logger(), "未找到目标");
        }
    }
    //cv::imshow("RGB", imgOriginal);
    //cv::imshow("Mask", mask); 
    //cv::waitKey(1);



}
