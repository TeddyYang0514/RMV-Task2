#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
using namespace std;
using namespace cv;
int main(){
    string imagePath = "/home/yangchen/opencv/opencv_project/resources/image"; // 将 "image.jpg" 替换为图片路径
    Mat image = imread(imagePath);
//转化为灰度图
    Mat grayimage;

    // 将图像转换为灰度
    cvtColor(image, grayimage, COLOR_BGR2GRAY);
    
    imshow("Gray image", grayimage);
    waitKey(0);
    
//转化为HsV图像   
    Mat hsvimage;

    // 将图像转换为HsV格式
    cvtColor(image, hsvimage, COLOR_BGR2HSV);

    imshow("HsV image", hsvimage);
    
    waitKey(0);
    
//应用均值滤波
    Mat blurredimage;
    
    // 使用均值滤波，滤波窗口大小为 5*5
    blur(image, blurredimage, Size(5, 5));

    imshow("Blurred image 5*5", blurredimage);

    waitKey(0);
    
    // 使用均值滤波，滤波窗口大小为 3*3
    blur(image, blurredimage, Size(3, 3));

    imshow("Blurred image 3*3", blurredimage);

    waitKey(0);
//应用高斯滤波
    Mat gaussyblurredimage;

    // 使用高斯滤波，滤波核大小为 3x3，标准差为 0
    GaussianBlur(image, gaussyblurredimage, Size(3, 3), 0, 0);

    imshow("Gaussy Blurred image", gaussyblurredimage);

    waitKey(0);
    
//提取红色区域
    // 将图像转换为HsV颜色空间
    cvtColor(image, hsvimage, COLOR_BGR2HSV);

    Mat lowerredMask, upperredMask, redMask;

    // 低范围的红色 (H: 0 ~ 10)
    inRange(hsvimage, Scalar(0, 100, 100), Scalar(10, 255, 255), lowerredMask);

    // 高范围的红色 (H: 156~ 180)
    inRange(hsvimage, Scalar(156, 100, 100), Scalar(180, 255, 255), upperredMask);

    // 合并两个红色区域的阈值
    addWeighted(lowerredMask, 1.0, upperredMask, 1.0, 0.0, redMask);

    // 提取红色区域
    Mat redregion;
    bitwise_and(image, image, redregion, redMask);

    // 显示红色区域提取结果
    imshow("red region", redregion);

    waitKey(0);

//寻找红色外轮廓
    // 将图像转换为HsV颜色空间
    cvtColor(image, hsvimage, COLOR_BGR2HSV);

    // 低范围的红色 (H: 0 ~ 10)
    inRange(hsvimage, Scalar(0, 100, 100), Scalar(10, 255, 255), lowerredMask);

    // 高范围的红色 (H: 156 ~ 180)
    inRange(hsvimage, Scalar(156, 100, 100), Scalar(180, 255, 255), upperredMask);

    // 合并两个红色区域
    addWeighted(lowerredMask, 1.0, upperredMask, 1.0, 0.0, redMask);

    // 寻找轮廓
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(redMask, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    // 绘制轮廓
    Mat contourimage = image.clone();
    for (int i = 0; i < contours.size(); ++i) {
        drawContours(contourimage, contours, (int)i, Scalar(0, 255, 0), 2);
    }

    // 显示轮廓图像
    imshow("red Contours", contourimage);

    waitKey(0);
    
//寻找红色的boudingbox并计算面积

    // 绘制轮廓并计算外接矩形 (bounding box)
    Mat boundingBoximage = image.clone(); 
    double Total_area;
    for (int i = 0; i < contours.size(); ++i) {
        // 计算外接矩形 
        Rect boundingBox = boundingRect(contours[i]);

        // 在图像上绘制外接矩形
        rectangle(boundingBoximage, boundingBox, Scalar(255, 0, 0), 2);

        // 计算外接矩形的面积
        double area;
        area = boundingBox.width * boundingBox.height;
        Total_area+=area;
    }

    // 显示外接矩形图像
    imshow("Bounding Boxes", boundingBoximage);
    printf("Area = %f",Total_area);
    waitKey(0);

// 提取高亮颜色区域并进行图形学处理

    cvtColor(image, hsvimage, COLOR_BGR2HSV);

    // 创建阈值
    Mat mask;
    inRange(hsvimage, Scalar(0,0,200), Scalar(180,255,255), mask);

    // 提取高亮部分
    Mat highlightedregions;

    image.copyTo(highlightedregions, mask);

    imshow("Highlighted regions", highlightedregions);

    waitKey(0);

    //灰度化
    cvtColor(highlightedregions, grayimage, COLOR_BGR2GRAY);

    imshow("Highlighted regions -gray", grayimage);

    waitKey(0);

    //二值化
    Mat threshimage;

    threshold(grayimage, threshimage, 63, 255, THRESH_OTSU);

    imshow("Thresh image", threshimage);

    waitKey(0);

    //膨胀
    Mat dilateimage;

        //提取膨胀结构元素
    Mat struct1 = getStructuringElement(0, Size(3,3));

    dilate(highlightedregions, dilateimage, struct1);

    imshow("Dilate image", dilateimage);

    waitKey(0);

    //腐蚀
    Mat Erodeimage;

    Mat struct2 = getStructuringElement(0, Size(3,3));

    erode(highlightedregions, Erodeimage, struct2);

    imshow("Erode image", Erodeimage);

    waitKey(0);

    //漫水处理
    Mat floodfillimage = threshimage.clone();

    floodFill(floodfillimage, Point(400,400), Scalar(156, 100, 100), 0, Scalar(20,20,20), Scalar(100,100,100), cv::FLOODFILL_FIXED_RANGE);

    imshow("Floodfill image", floodfillimage);

    waitKey(0);

//图像绘制

    //绘制圆


    Mat circleimage = image.clone();

    circle(circleimage, Point(circleimage.rows/3, circleimage.cols/3), 100, Scalar(0,0,255), 2, LINE_8);

    imshow("circle", circleimage);

    waitKey(0);

    //绘制矩形
    Mat rectimage = image.clone();

    rectangle(rectimage, Rect(circleimage.rows/3, circleimage.cols/3,circleimage.rows/2.5, circleimage.cols/2.5), Scalar(0,0,255), 2, LINE_8);

    imshow("rectangle", rectimage);

    waitKey(0);

    //绘制文字
    Mat textimage = image.clone();

    putText(textimage, "Ionia is in high spirits!!", Point(100, 100), FONT_HERSHEY_COMPLEX, 0.8, Scalar(0, 0, 255), 2, LINE_8);

    imshow("text", textimage);

    waitKey(0);

//图像操作

    //旋转35度
    int w = image.size().width/2;
    int h = image.size().height/2;

    Mat spinimage;

    Mat Matrix;

    Matrix=getRotationMatrix2D(Point(w,h), -35, 1.0);

    warpAffine(image, spinimage, Matrix, spinimage.size());

    imshow("spin", spinimage);

    waitKey(0);

    //裁减
    
    Mat cropped_image = image(Range(0,1213), Range(855,1709));

    imshow("cropped image", cropped_image);

    waitKey(0);

    return 0;

}
