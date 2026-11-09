#include<rclcpp/rclcpp.hpp>
#include<sensor_msgs/msg/image.hpp>
#include<cv_bridge/cv_bridge.h>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>

std::shared_ptr<rclcpp::Node>node;

static int iLowH = 10;
static int iHighH = 40;

static int iLowS = 90;
static int iHighS = 90;

static int iLowV = 1;
static int iHighV = 255;

void CamRGBCallback(const sensor_msgs::msg::Image::SharedPtr msg)
{
    cv_bridge::CvImagePtr cv_ptr;
    cv_ptr = cv_bridge::toCvCopy(msg,sensor_msgs::image_encodings::BGR8);

    cv::Mat imgOriginal = cv_ptr->image;

    cv::Mat imgHSV;
    cv::cvtColor(imgOriginal,imgHSV,cv::COLOR_BGR2HSV);

    std::vector<cv::Mat> hsvSplit;
    cv::split(imgHSV,hsvSplit);
    cv::equalizeHist(hsvSplit[2],hsvSplit[2]);
    cv::merge(hsvSplit,imgHSV);

    cv::Mat imgThresholded;
    cv::inRange(imgHSV,cv::Scalar(iLowH,iLowS,iLowV),cv::Scalar(iHighH,iHighS,iHighV),imgThresholded);

    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(5,5));
    cv::morphologyEx(imgThresholded,imgThresholded,cv::MORPH_OPEN,element);
    cv::morphologyEx(imgThresholded,imgThresholded,cv::MORPH_CLOSE,element);

    int nTargetX = 0;
    int nTargetY = 0;
    int nPixCount = 0;
    int nImgWidth = imgThresholded.cols;
    int nImgHeight = imgThresholded.rows;

    for(int y=0;y<nImgHeight;y++)
    {
        for(int x=0;x<nImgWidth;x++)
        {
            if(imgThresholded.data[y*nImgWidth+x]==255)
            {
                nTargetX += x;
                nTargetY += y;
                nPixCount++;
            }
        }

    }
    if(nPixCount>0)
    {
        nTargetX /= nPixCount;
        nTargetY /= nPixCount;
        printf("Target(%d,%d)PixelCount=%d\n",nTargetX,nTargetY,nPixCount);
        cv::Point line_begin = cv::Point(nTargetX-10,nTargetY);
        cv::Point line_end = cv::Point(nTargetX+10,nTargetY);
        cv::line(imgOriginal,line_begin,line_end,cv::Scalar(255,0,0));
        line_begin.x = nTargetX;line_begin.y = nTargetY-10;
        line_end.x = nTargetX;line_end.y = nTargetY+10;
        cv::line(imgOriginal,line_begin,line_end,cv::Scalar(255,0,0));
    }
    else
    {
        printf("Target disappeared...\n");
    }

    cv::imshow("RGB",imgOriginal);
    cv::imshow("HSV",imgHSV);
    cv::imshow("Result",imgThresholded);
    cv::waitKey(5);
}

int main(int argc,char**argv)
{
    rclcpp::init(argc,argv);

    node = std::make_shared<rclcpp::Node>("cv_hsv_node");

    auto rgb_sub = node->create_subscription<sensor_msgs::msg::Image>("/kinect2/qhd/image_raw",1,CamRGBCallback);

    cv::namedWindow("Threshold",cv::WINDOW_AUTOSIZE);

    cv::createTrackbar("LowH","Threshold",&iLowH,179);
    cv::createTrackbar("HighH","Threshold",&iHighH,179);

    cv::createTrackbar("LowS","Threshold",&iLowS,255);
    cv::createTrackbar("HighS","Threshold",&iHighS,255);

    cv::createTrackbar("LowV","Threshold",&iLowV,255);
    cv::createTrackbar("HighV","Threshold",&iHighV,255);

    cv::namedWindow("RGB");
    cv::namedWindow("HSV");
    cv::namedWindow("Result");

    rclcpp::spin(node);

    cv::destroyAllWindows();

    rclcpp::shutdown();
    
    return 0;
}