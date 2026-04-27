#include "videodetector.h".h"

videodetector::videodetecing(QObject *parent)
    : QObject{parent}
{

}
void videothread::circleDetect(cv::Mat &frame) {


        Mat hsv, mask, result;
        frame.copyTo(result);

        // 1. 转换HSV
        cvtColor(frame, hsv, COLOR_BGR2HSV);

        // 2.创建掩码
        if (colorMode == 1) {
            // === 红色双阈值 ===
            Mat mask1, mask2;

            // 红1: H: 0 ~ red_h1_max
            inRange(hsv, Scalar(0, minSat, minVal), Scalar(red_h1_max, 255, 255), mask1);

            // 红2: H: red_h2_min ~ 180
            inRange(hsv, Scalar(red_h2_min, minSat, minVal), Scalar(180, 255, 255), mask2);

            // 合并
            bitwise_or(mask1, mask2, mask);

        } else if (colorMode == 2) {
            // === 绿色 ===
            // 绿色范围: H: 35 ~ 85
            inRange(hsv, Scalar(35, minSat, minVal), Scalar(85, 255, 255), mask);

        } else if (colorMode == 3) {
            // === 蓝色范围 ===
            // 蓝色范围: H: 100 ~ 130
            inRange(hsv, Scalar(100, minSat, minVal), Scalar(130, 255, 255), mask);
        }

        // 3. 形态学操作
        Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
        morphologyEx(mask, mask, MORPH_OPEN, kernel);
        morphologyEx(mask, mask, MORPH_CLOSE, kernel);

        // 显示mask
        imshow(maskWindowName, mask);

        // 4. 霍夫元检测
        vector<Vec3f> circles;

        double dp = dp_val / 10.0;
        if (dp < 0.1) dp = 1.0;

        HoughCircles(mask, circles, HOUGH_GRADIENT, dp, min_dist,
                     param1_val, param2_val, min_radius, max_radius);

        // 5.draw reasult
        for (size_t i = 0; i < circles.size(); i++) {
            Vec3i c = circles[i];
            Point center(c[0], c[1]);
            int radius = c[2];

            // center
            circle(result, center, 3, Scalar(0, 255, 0), -1, LINE_AA);
            // circle
            circle(result, center, radius, Scalar(0, 255, 0), 2, LINE_AA);

            // 半径
//            putText(result, to_string(radius), Point(c[0]-20, c[1]-radius-10),
//                    FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255,255,255), 1);
        }



        return result;

}
