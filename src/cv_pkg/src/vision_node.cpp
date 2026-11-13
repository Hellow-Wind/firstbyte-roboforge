#include<cv_pkg/vision_node.hpp>
#include<algorithm>

//构造函数
VisionNode::VisionNode(const std::string& node_name) : Node(node_name)
{

    // 预分配 RGB 原图内存
    //imgOriginal_ = cv::Mat(480, 640, CV_8UC3);
    // 预分配 HSV 图内存
    imgHSV_ = cv::Mat(480, 640, CV_8UC3);
    // 预分配 Mask 内存
    mask_ = cv::Mat(480, 640, CV_8UC1);
    mask_high_ = cv::Mat(480, 640, CV_8UC1);
    mask_low_ = cv::Mat(480, 640, CV_8UC1);

    pub_target_ = create_publisher<std_msgs::msg::Int32>("/target_points", 10);

    sub_image_ = create_subscription<sensor_msgs::msg::Image>("/image_raw",10,
        std::bind(&VisionNode::ImageCallback,this,std::placeholders::_1));

}


//图像处理+寻找坐标
cv::Point VisionNode::FindTargets(const cv::Mat& imgHSV)
{
    //颜色滤波
    cv::inRange(imgHSV,cv::Scalar(iLowH_m1_,iLowS_m1_,iLowV_m1_),cv::Scalar(iHighH_m1_,iHighS_m1_,iHighV_m1_),mask_high_);
    cv::inRange(imgHSV,cv::Scalar(iLowH_m2_,iLowS_m2_,iLowV_m2_),cv::Scalar(iHighH_m2_,iHighS_m2_,iHighV_m2_),mask_low_);
    cv::bitwise_or(mask_low_,mask_high_,mask_);

    //形态学
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(11,11));
    cv::morphologyEx(mask_,mask_,cv::MORPH_OPEN,element);
    cv::morphologyEx(mask_,mask_,cv::MORPH_CLOSE,element);

    //轮廓
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask_,contours,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_SIMPLE);

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
    /*
    if(debug_mode_){
        cv::imshow("Mask", mask_); 
        cv::waitKey(1);
    }
    */
    return target;
}

/*
//画十字
void VisionNode::DrawDebug(cv::Mat& imgOriginal,const cv::Point target)
{
    cv::drawMarker(imgOriginal,target,cv::Scalar(0,255,0),cv::MARKER_CROSS,20,2);
}
*/
//视觉识别操作
void VisionNode::ImageCallback(const sensor_msgs::msg::Image::SharedPtr msg)
{
    //传图片指针
    cv_bridge::CvImageConstPtr cv_ptr = cv_bridge::toCvShare(msg, "bgr8");
    const cv::Mat& img = cv_ptr->image;

    //转hsv
    imgHSV_.create(img.rows, img.cols, CV_8UC3);
    cv::cvtColor(img, imgHSV_, cv::COLOR_BGR2HSV);
    //cv::cvtColor(imgOriginal_,imgHSV_,cv::COLOR_BGR2HSV);

    //图像处理
    cv::Point target = FindTargets(imgHSV_);
    
    

    if(target.x!=-1)
    {

        msg_.data = (target.x < imgHSV_.cols / 3) ? 0 : ((target.x < imgHSV_.cols * 2 / 3) ? 1 : 2);

        pub_target_->publish(msg_);

    }
    else
    {
        RCLCPP_INFO(get_logger(),"没有找到目标");
    }


    /*暂时弃用
    if(debug_mode_)
    {
        if (target.x != -1) {
            RCLCPP_INFO(get_logger(), "板子在位置编号: %d", current_dir);
        }

        cv::Mat imgDisplay = img.clone(); // 深拷贝一份专门用来画图
        if(target.x != -1) {
            DrawDebug(imgDisplay, target); // 先画十字
        }
        
        cv::imshow("RGB", imgDisplay); // 再显示画面
        cv::waitKey(1);                // 最后刷新一次
    }
    */
}