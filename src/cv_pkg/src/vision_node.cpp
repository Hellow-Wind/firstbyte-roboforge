#include<cv_pkg/vision_node.hpp>
#include<algorithm>

//构造函数
VisionNode::VisionNode(const std::string& node_name) : Node(node_name)
{
    pub_target_ = create_publisher<std_msgs::msg::Int32>("/target_points", 10);

    sub_image_ = create_subscription<sensor_msgs::msg::Image>("/image_raw",10,
        std::bind(&VisionNode::ImageCallback,this,std::placeholders::_1));
}


//图像处理+寻找坐标
cv::Point VisionNode::FindTargets(const cv::Mat& imgHSV)
{
    //颜色滤波
    cv::Mat mask,mask_high,mask_low;
    cv::inRange(imgHSV,cv::Scalar(iLowH_m1_,iLowS_m1_,iLowV_m1_),cv::Scalar(iHighH_m1_,iHighS_m1_,iHighV_m1_),mask_high);
    cv::inRange(imgHSV,cv::Scalar(iLowH_m2_,iLowS_m2_,iLowV_m2_),cv::Scalar(iHighH_m2_,iHighS_m2_,iHighV_m2_),mask_low);
    cv::bitwise_or(mask_low,mask_high,mask);

    //形态学
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(11,11));
    cv::morphologyEx(mask,mask,cv::MORPH_OPEN,element);
    cv::morphologyEx(mask,mask,cv::MORPH_CLOSE,element);

    //轮廓
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask,contours,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_SIMPLE);

    //目标（最大面积）
    double target_area = 0;
    cv::Point target = {-1,-1}; 

    for(size_t i=0;i<contours.size();++i)
    {
        double area = cv::contourArea(contours[i]);

        if(area>min_area && area<max_area)
        {
            cv::Rect rect = cv::boundingRect(contours[i]);

            int width = rect.width;
            int height = rect.height;
            double rate = 1.0*height/width;
            if(rate<=aspect_ratio_max_ && rate>=aspect_ratio_min_)
            {
                int cx = rect.x + width/2;
                int cy = rect.y + height/2;

                if(target_area<area)
                {
                    target_area = area;
                    target = {cx,cy};
                }
            }
        }
    }

    if(debug_mode_){
        cv::imshow("Mask", mask); 
        cv::waitKey(1);
    }

    return target;
}


//画十字
void VisionNode::DrawDebug(cv::Mat& imgOriginal,const cv::Point target)
{
    cv::drawMarker(imgOriginal,target,cv::Scalar(0,255,0),cv::MARKER_CROSS,20,2);
}

//视觉识别操作
void VisionNode::ImageCallback(const sensor_msgs::msg::Image::SharedPtr msg)
{
    //传图
    cv::Mat imgOriginal = cv_bridge::toCvCopy(msg,"bgr8")->image;

    //转hsv
    cv::Mat imgHSV;
    cv::cvtColor(imgOriginal,imgHSV,cv::COLOR_BGR2HSV);

    //图像处理
    cv::Point target = FindTargets(imgHSV);
    
    if(target.x!=-1)
    {
        auto msg = std_msgs::msg::Int32();

        msg.data = (target.x < imgOriginal.cols / 3) ? 1 : ((target.x < imgOriginal.cols * 2 / 3) ? 2 : 3);

        pub_target_->publish(msg);

        if(debug_mode_){
            DrawDebug(imgOriginal,target);
            RCLCPP_INFO(get_logger(), "板子在位置编号: %d", msg.data);
        }
    }
    else
    {
        RCLCPP_INFO(get_logger(),"没有找到目标");
    }



    if(debug_mode_){
        cv::imshow("RGB", imgOriginal);
        cv::waitKey(1);
    }
}