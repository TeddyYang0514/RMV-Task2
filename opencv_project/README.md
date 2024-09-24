# 使用C++和OpenCV进行图像处理

## 配置环境以及构建项目

### 配置C++开发环境
在系统终端中进行配置
```
    sudo apt update 
    sudo apt build essential 
    sudo apt install cmake
    sudo apt install libgtk2.0-dev pkg-config
    sudo apt install libopencv-dev
```
### 安装OpenCV库
在终端中安装
```
    sudo apt update\n
    sudo apt install libgtk2.0-dev pkg-config
    sudo apt install libopencv-dev
```

### 构建项目结构
```
    yangchen@yangchen-HP-Pavilion-Plus-Laptop-16-ab0xxx:~/opencv/opencv_project$ ls
    build  CMakeLists.txt  README.md  resources  src
    yangchen@yangchen-HP-Pavilion-Plus-Laptop-16-ab0xxx:~/opencv/opencv_project$ 
    yangchen@yangchen-HP-Pavilion-Plus-Laptop-16-ab0xxx:~/opencv/opencv_project$ cd build
    yangchen@yangchen-HP-Pavilion-Plus-Laptop-16-ab0xxx:~/opencv/opencv_project/build$ ls
    CMakeCache.txt  CMakeFiles  cmake_install.cmake  Makefile  opencv_project
```

## 核心实现

### CMakeLists.txt 编写
#### 文本内容如下

```
    cmake_minimum_required(VERSION 2.8)
    project(opencv_project)
    find_package(OpenCV REQUIRED)
    include_directories(${OpenCV_INCLUDE_DIRS})
    add_executable(opencv_project src/main.cpp)
    target_link_libraries(opencv_project ${OpenCV_LIBS})
```
#### 遇到的困难

在配置CmakeLists时对文件名称的大小写有严格的限定，查了好久查不出错误，结果问题出在文件名大小写上QAQ。

### 主程序开发
#### 实现思路
**-<big>图像颜色空间转换</big>**
	-**转化为灰度图**
	转化灰度图一共有两种操作方法，一是直接调用OpenCV中的转换函数，第二种是自己使用公式根据人眼亮度感知调节编写一个函数，这里选择第一种直接调用转换函数。`cvtColor(src, cvt_gray_image,COLOR_BGR2GRAY)`
	<https://blog.csdn.net/yangSHU21/article/details/130237669>                    
	-**转化为HSV图像**
	调用OpenCV中的转换函数`cvtColor(image,hsv_image,COLOR_BGR2HSV)`
	<https://blog.csdn.net/weixin_46999174/article/details/140574320>



**-<big>应用各种滤波操作</big>**
	-**应用均值滤波**
	均值滤波的原理为用当前像素点周围nxn个像素值的均值来代替当前像素值。
	<https://blog.csdn.net/weixin_51571728/article/details/121455266>
	而每一个像素的像素值则可以由如果对于一张本身就是灰度图像（8位灰度图像）来说，他的像素值就是它的灰度值，如果是一张彩色图像，则它的灰度值需要经过函数映射来得到。灰度图像是由纯黑和纯白来过渡得到的，在黑色中加入白色就得到灰色，纯黑和纯白按不同的比例来混合就得到不同的灰度值。R=G=B=255为白色，R=G=B=0为黑色，R=G=B=小于255的某个整数时，此时就为某个灰度值。
	<https://blog.csdn.net/Strive_0902/article/details/78023080>
	完成均值滤波的调用OpenCV中的均值滤波函数`blur(src, srcOpencv, Size(N, N))`，这里给出滤波核边长为3和5两种图像。
	<https://blog.csdn.net/jgj123321/article/details/95078503>

***	遇到的问题以及解决方法***	而在应用均值滤波之后，还有一些边缘像素点没有被处理（原因是图片的以像素单位的长宽不一定能够被滤波核单位的边长整除），因此理论上还要进行*边缘像素处理*，但是由于本身的图片较大，这些边缘效应可以被忽略，若要处理，可以通过OpenCV中的copyMakeBorder函数对图像边界像素进行扩充,使之成为像素单元的整数倍。

​	-**应用高斯滤波**
​	高斯滤波的原理为：
​	要模糊一张图像，可以这么做：对于每个像素点，以它为中心，取其3x3区域内所有像素灰度值的平均作为中心点的灰度值。可是，如果仅使用简单平均，显然不是很合理，因为图像都是连续的，越靠近的点关系越密切，越远离的点关系越疏远。因此，加权平均更合理，距离越近的点权重越大，距离越远的点权重越小。而正态分布显然是一种可取的权重分配模式。由于图像是二维的，所以需要使用二维的高斯函数。

​	实现我们通过OpenCV自带的函数`GaussianBlur`来处理。

​	<https://blog.csdn.net/a435262767/article/details/107115249>
​	

​	***遇到的问题以及解决方法***	接下来又遇到一个问题：高斯滤波的标准差应该选取多少。调整高斯滤波整S实际是在调整周围像素对当前像素的影响程度，调大S即提高了远处像素对中心像素的影响程度，滤波结果也就越平滑。经过资料的查阅，发现大多数教程的标准差选取0。

​	同均值滤波，边缘像素处理也是需要的。高斯模板在对图像边缘像素进行卷积时，会有一部分权重没有对应像素，因此我们需要在图像的边缘补0。这种方法称作Zero Padding。并且权值（卷积核）要进行归一化操作。*
<https://blog.csdn.net/a435262767/article/details/107115249>





**-<big>特征提取 </big>**

​	-**提取红色颜色区域**
​	Step 1:将图像转化为HSV空间。
​	Step 2:定义高范围的红色和低范围的红色阈值。使用OpenCV自带的`inRange`函数
​	Step 3:合并两个红色区域的阈值。使用`AddWeight`函数
​	Step 4:提取红色区域。
​	Step 5:显示红色区域提取结果。
​	<https://blog.csdn.net/one_cup_of_pepsi/article/details/132287326>
​	在完成上述步骤之后，我们需要确定红色的HSV范围，经过查阅，网上所说的红色HSV范围众说纷纭，最后选择了这个表(https://i-blog.csdnimg.cn/blog_migrate/9ab4a1561056e987e48d01f9078ad3cb.png)
​	-**寻找红色外轮廓并展示之**
​	我使用的方法思路是先用上述方法提取出红色区域，然后再绘制出图像的轮廓线（因为这样能体现出两个子任务的关联性哈哈），寻找轮廓用的是绘制轮廓的话用的是`findCounters`函数，绘制轮廓的话用的是`drawContours`函数。
​	要寻找HSV图像中的颜色轮廓并绘制它们，可以按照以下步骤进行：

>1. **转换颜色空间**：将图像从BGR转换为HSV。
>2. **使用`inRange`**：根据指定的HSV范围创建掩模。
>3. **查找轮廓**：使用`findContours`来获取轮廓。
>4. **绘制轮廓**：使用`drawContours`绘制找到的轮廓。

>以下是一个完整的示例代码，展示如何实现这些步骤：

```cpp
#include <opencv2/opencv.hpp>
#include <vector>

int main() {
    // 读取图像
    cv::Mat image = cv::imread("image.jpg");
    if (image.empty()) return -1;

    // 转换到HSV颜色空间
    cv::Mat hsvimage;
    cv::cvtColor(image, hsvimage, cv::COLOR_BGR2HSV);

    // 定义HSV范围
    cv::Scalar lower(0, 100, 100); // 下界
    cv::Scalar upper(10, 255, 255); // 上界

    // 创建掩模
    cv::Mat mask;
    cv::inRange(hsvimage, lower, upper, mask);

    // 查找轮廓
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // 绘制轮廓
    cv::Mat output = image.clone();
    for (size_t i = 0; i < contours.size(); i++) {
        cv::drawContours(output, contours, static_cast<int>(i), cv::Scalar(0, 255, 0), 2);
    }

    return 0;
}
```
-**寻找并绘制红色的boundingbox，然后计算面积**
	在上一节的基础之上，使用找到的红色部分，运用`boudingRect`函数来计算外接矩形,并用`rectangle`在图像上绘制方框。最后再用`boudingBox.width`以及`boundingBox.height`函数来计算方框的面积。
<	https://blog.csdn.net/jndingxin/article/details/124569885>

**提取高亮颜色区域并进行图形学处理**
	提取高亮颜色区域我的思路是限制HSV中亮度（VALUE）的值来提取亮度较高颜色的区域。

​	这里的“高亮”定义比较模糊*我选取的HSV分别为0-180，0-255，0-255；
​	<https://blog.csdn.net/zsc201825/article/details/89919496>\n
<u>**-灰度化**</u>

​	同第一节——转化为灰度图*
**-<u>二值化</u>**
​	首先，二值化操作可以通过OpenCV自带的`threshold`来实现。同时膨胀结构元素使用`getStructuringElement`来实现。
​	但是还存一个问题，就是二值化的阈值怎么选取。
​	<u>经过查阅，确定二制法的阈值有多种方法，比如*P参数法*，*双峰法*，*OTSU法*。这里选取OTSU法。</u>

​	OTSU法: 日本工程师大津展之为这个波谷找到了一个合适的数学表达，并于1979年发表。这个二值化方法称为大津算法（Otsu’s method）。从大津法的原理上来讲，该方法又称作最大类间方差法，因为按照大津法求得的阈值进行图像二值化分割后，前景与背景图像的类间方差最大。它被认为是图像分割中阈值选取的最佳算法，计算简单，不受图像亮度和对比度的影响，因此在数字图像处理上得到了广泛的应用。它是按图像的灰度特性，将图像分成背景和前景两部分。因方差是灰度分布均匀性的一种度量,背景和前景之间的类间方差越大,说明构成图像的两部分的差别越大,当部分前景错分为背景或部分背景错分为前景都会导致类间差别变小。因此,使类间方差最大的分割意味着错分概率最小。
<	https://blog.csdn.net/IYXUAN/article/details/124458001>

​	同时OpenCV也自带OSTU法的函数`threshold(src1, src2, A, B, THRESH_OTSU);` 返回值也包括阈值`retval`。
<	https://blog.csdn.net/youcans/article/details/121451915>

**<u>-膨胀</u>**

​	运用函数`dialate`进行膨胀操作并使用函数`getStructuringElement`来提取膨胀元素。

**<u>-腐蚀</u>**

​	调用函数`erode`进行膨胀操作并使用函数`getStructuringElement`来提取膨胀元素。

​	<https://blog.csdn.net/qq_33287871/article/details/112700635>

**<u>-漫水处理</u>**

​	调用函数`floodFill`进行漫水操作

​	<https://zhuanlan.zhihu.com/p/493492176>

### 图像绘制

**-绘制任意圆形方形和文字**

​	使用OpenCV内置的`circle`,  `rectangle`,  `putText`操作

**-绘制红色的外轮廓**

​	见上一章

**-绘制红色的 bounding box**

​	见上一章

### 对图像进行处理

**<u>-旋转</u>**

​	运用`getRotationMatrix2D`内置函数算出旋转矩阵，然后用`warpAffine`进行旋转操作。

​	<https://blog.csdn.net/qq_22734027/article/details/134482760>

**<u>-裁剪</u>**

​	直接对原图进行裁减

​	<https://blog.csdn.net/zhuoqingjoking97298/article/details/122727680>

### 总结

​	这次任务调用了OpenCV中的许多函数并对图形进行了空间转换、滤波、特征处理、绘制以及旋转和裁剪等操作，除了学习这些函数之外，我还在进行均值滤波和高斯滤波时学到了对图形边界的补充处理和优化、二值化中的OTSU方法等比较前沿的方法。

​	*Jack of all trades and master of none.* 

​	后续还要在实战中对这些操作进行精通。
