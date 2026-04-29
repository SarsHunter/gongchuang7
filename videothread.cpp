#include "videothread.h"
#include <QDebug>


videothread::videothread(int camera, QMutex *lock)
{
    running=false;

    cameraID=camera;

    data_lock=lock;

    redThreshold ={170,180,43,255,46,255};

    greenThreshold ={35,85,43,255,46,255};
    blueThreshold  ={100,130,43,255,46,255};


    currentColorType=1;

    blockDetecting=false;
    circleDetecting=false;
    qrDetecting=false;

    scanner.set_config(zbar::ZBAR_NONE,zbar::ZBAR_CFG_ENABLE,1);
}

videothread::~videothread()
{
    stop();
}


//stop 函数实现
void videothread::stop()
{
    // 1. 判断线程是否正在运行
    if (isRunning()) {
        // 2. 修改标志位，让 run() 函数里的 while 循环退出
        running = false;

        // 3. 等待线程安全退出
        // 这一步非常重要！如果不 wait，直接 delete 后续可能会崩溃
        // wait() 会阻塞，直到 run() 函数执行完毕
        wait();
    }
}


void videothread::run()
{
    running=true;

    cv::VideoCapture cap(cameraID,cv::CAP_V4L2);

    if(!cap.isOpened())
    {
        qDebug()<<"camera open failed";
        return;
    }

    cap.set(cv::CAP_PROP_FRAME_WIDTH,640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT,480);
    cap.set(cv::CAP_PROP_FPS,60);

    cv::Mat frame;

    while(running)
    {
        cap>>frame;

        if(frame.empty())
            break;

        cv::Mat processFrame=frame;

        cv::Mat mask=createMask(processFrame);

        if(qrDetecting)
            detectQRCode(processFrame);

        if(blockDetecting)
            detectColorBlock(processFrame,mask);

        if(circleDetecting)
            detectCircle(processFrame,mask);



        emit frameCaptured(matToQimage(processFrame));

        emit maskCaptured(matToQimage(mask));
    }

    cap.release();

    running=false;
}




void videothread::detectCircle(cv::Mat &frame, cv::Mat &mask)
{

    cv::Point2f focus;
    if(hasLastCenter)
        focus = lastCenter;
    else
        focus = cv::Point2f(frame.cols/2.0f, frame.rows/2.0f);

    int roiWidth;
    int roiHeight;

    if(hasLastCenter)
    {
        roiWidth = 200;
        roiHeight = 200;
    }
    else
    {
        roiWidth = frame.cols;
        roiHeight = frame.rows;
    }

//    int roiWidth = 200;
//    int roiHeight = 200;

    int x = std::max(int(focus.x - roiWidth/2), 0);
    int y = std::max(int(focus.y - roiHeight/2), 0);
    int w = std::min(roiWidth, frame.cols - x);
    int h = std::min(roiHeight, frame.rows - y);

    cv::Rect roiRect(x, y, w, h);
    cv::Mat roiMask = mask(roiRect);


    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(7,7));
    cv::Mat processed;
    cv::dilate(roiMask, processed, kernel, cv::Point(-1,-1), 2);
    cv::morphologyEx(processed, processed, cv::MORPH_CLOSE, kernel, cv::Point(-1,-1), 1);


    cv::Mat gray;
    if(processed.channels() == 3)
        cv::cvtColor(processed, gray, cv::COLOR_BGR2GRAY);
    else
        gray = processed.clone();


    cv::GaussianBlur(gray, gray, cv::Size(7,7), 2,2);


    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(gray, circles, cv::HOUGH_GRADIENT,
                     1.2, 100, 50, 30, 20, 180);

    if(circles.empty())
    {
        lostFrames++;
        if(lostFrames >= maxLostFrames)
            hasLastCenter = false;
        // 连续丢帧超过30帧 → 通知上位机目标丢失，停车
        if(lostFrames >= 30)
            emit circleError(0, 0, 2, 2);
        return;
    }


    lostFrames = 0;


    cv::Point2f centerMax;
    float radiusMax = 0;
    for(size_t i=0; i<circles.size(); i++)
    {
        float radius = circles[i][2];
        if(radius > radiusMax)
        {
            radiusMax = radius;
            centerMax = cv::Point2f(circles[i][0], circles[i][1]);
        }
    }

//    if(radiusMax < 20 || radiusMax > 200)
//        return;


    centerMax.x += x;
    centerMax.y += y;


    lastCenter = centerMax;
    hasLastCenter = true;


    cv::circle(frame, centerMax, radiusMax, cv::Scalar(0,255,0), 2);
    cv::circle(frame, centerMax, 5, cv::Scalar(0,0,255), -1);


    cv::Point2f screenCenter(frame.cols/2.0f, frame.rows/2.0f);
    cv::circle(frame, screenCenter, 5, cv::Scalar(255,0,0), -1);


    cv::line(frame, screenCenter, centerMax, cv::Scalar(255,255,0), 2);


    int dx = centerMax.x - screenCenter.x;
    int dy = centerMax.y - screenCenter.y;

    uint8_t x_dir = (dx >= 0) ? 1 : 0;
    uint8_t y_dir = (dy >= 0) ? 1 : 0;

    emit circleError(dx, dy, x_dir, y_dir);
//    float distance = cv::norm(centerMax - screenCenter);
//    char buf[50];
//    snprintf(buf, sizeof(buf), "Dist: %.1f px", distance);
//    cv::putText(frame, buf, cv::Point(10,30),
//                cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0,0,255), 2);


//    qDebug() << "Max Hough Circle Center:" << centerMax.x
//             << centerMax.y << " radius:" << radiusMax
//             << " distance to screen center:" << distance;
}


// circleDetect，接收 mask 作为参数
//void videothread::detectCircle(cv::Mat &frame, cv::Mat &mask)
//{

//    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(7, 7));
//    cv::Mat processed;


//    cv::dilate(mask, processed, kernel, cv::Point(-1,-1), 2);


//    cv::morphologyEx(processed, processed, cv::MORPH_CLOSE, kernel, cv::Point(-1,-1), 1);


//    cv::Mat gray;
//    if(processed.channels() == 3)
//        cv::cvtColor(processed, gray, cv::COLOR_BGR2GRAY);
//    else
//        gray = processed.clone();


//    cv::GaussianBlur(gray, gray, cv::Size(7,7), 2,2);


//    std::vector<cv::Vec3f> circles;
//    cv::HoughCircles(gray, circles, cv::HOUGH_GRADIENT,
//                     1.2, 100, 50, 30, 20, 180);

//    if(circles.empty())
//        return;

//    // 5.
//    cv::Point2f centerMax;
//    float radiusMax = 0;
//    for(size_t i=0; i<circles.size(); i++)
//    {
//        float radius = circles[i][2];
//        if(radius > radiusMax)
//        {
//            radiusMax = radius;
//            centerMax = cv::Point2f(circles[i][0], circles[i][1]);
//        }
//    }

//    if(radiusMax < 20 || radiusMax > 200){
//        return;
//    }

//    cv::circle(frame, centerMax, radiusMax, cv::Scalar(0,255,0), 2);
//    cv::circle(frame, centerMax, 5, cv::Scalar(0,0,255), -1);


//    cv::Point2f screenCenter(frame.cols/2.0f, frame.rows/2.0f);
//    cv::circle(frame, screenCenter, 5, cv::Scalar(255,0,0), -1);


//    cv::line(frame, screenCenter, centerMax, cv::Scalar(255,255,0), 2);


////    float distance = cv::norm(centerMax - screenCenter);
////    char buf[50];
////    snprintf(buf, sizeof(buf), "Dist: %.1f px", distance);
////    cv::putText(frame, buf, cv::Point(10,30),
////                cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0,0,255), 2);


////    qDebug() << "Max Hough Circle Center:" << centerMax.x
////             << centerMax.y << " radius:" << radiusMax
////             << " distance to screen center:" << distance;
//}

void videothread::detectColorBlock(cv::Mat &frame, cv::Mat &mask)
{
    // 1?? ÐÎÌ¬Ñ§´¦Àí£ºÈ¥Ôëµã + Ìî³äÐ¡¿×¶´
    cv::Mat cleanMask;
    cv::morphologyEx(mask, cleanMask, cv::MORPH_OPEN, cv::Mat(), cv::Point(-1,-1), 1);
    cv::morphologyEx(cleanMask, cleanMask, cv::MORPH_CLOSE, cv::Mat(), cv::Point(-1,-1), 1);

    // 2?? ÕÒÂÖÀª
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(cleanMask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    if (contours.empty())
        return;

    double maxArea = 0;
    std::vector<cv::Point> bestContour;

    // 3?? ÂÖÀªÉ¸Ñ¡
    for (auto &c : contours)
    {
        double area = cv::contourArea(c);

        // 3??1 Ãæ»ý¹ýÂË£ºÌÞ³ýÌ«Ð¡
        if (area < 2000)  // ¿É¸ù¾ÝÊµ¼ÊÄ¿±êµ÷½ÚãÐÖµ
            continue;

        // 3??2 ¼ÆËãÍ¹°ü solidity
        std::vector<cv::Point> hull;
        cv::convexHull(c, hull);
        double hullArea = cv::contourArea(hull);
        double solidity = (hullArea > 0) ? (area / hullArea) : 0;
        if (solidity < 0.8)
            continue;

        // 3??3 ¼ÆËã³¤¿í±È
        cv::Rect bounding = cv::boundingRect(c);
        double aspect = (double)bounding.width / bounding.height;
        if (aspect < 0.3 || aspect > 3.0)
            continue;

        // Ñ¡ÔñÃæ»ý×î´óµÄ·ûºÏÌõ¼þÂÖÀª
        if (area > maxArea)
        {
            maxArea = area;
            bestContour = c;
        }
    }

    if (bestContour.empty())
        return;

    // 4?? ¼ÆËãÖÊÐÄ
    cv::Moments m = cv::moments(bestContour);
    int cx = int(m.m10 / m.m00);
    int cy = int(m.m01 / m.m00);

    // 5?? »æÖÆÂÖÀªºÍÖÐÐÄµã
    cv::drawContours(frame, std::vector<std::vector<cv::Point>>{bestContour}, -1, cv::Scalar(0,255,0), 2);
    cv::circle(frame, cv::Point(cx, cy), 5, cv::Scalar(0,0,255), -1);
}

void videothread::setHSVThreshold(int hMin,int hMax,
                                  int sMin,int sMax,
                                  int vMin,int vMax)
{
    QMutexLocker locker(data_lock);

    HSVThreshold *th=nullptr;

    if(currentColorType==1) th=&redThreshold;
    if(currentColorType==2) th=&greenThreshold;
    if(currentColorType==3) th=&blueThreshold;

    if(th)
    {
        th->h_min=hMin;
        th->h_max=hMax;
        th->s_min=sMin;
        th->s_max=sMax;
        th->v_min=vMin;
        th->v_max=vMax;
    }
}


void videothread::setColorType(int type)
{
    QMutexLocker locker(data_lock);

    if (type < 1 || type > 3) return;

    currentColorType = type;
}

HSVThreshold videothread::getThreshold(int type)
{
    QMutexLocker locker(data_lock);

    switch(type)
    {
    case 1: return redThreshold;
    case 2: return greenThreshold;
    case 3: return blueThreshold;
    }

    return redThreshold;
}


cv::Mat videothread::createMask(cv::Mat &frame)
{
    cv::Mat hsv;
    cv::Mat mask;

    cv::cvtColor(frame,hsv,cv::COLOR_BGR2HSV);

    HSVThreshold th;

    {
        QMutexLocker locker(data_lock);

        if(currentColorType==1) th = redThreshold;
        if(currentColorType==2) th = greenThreshold;
        if(currentColorType==3) th = blueThreshold;
    }

    cv::inRange(hsv,
                cv::Scalar(th.h_min,th.s_min,th.v_min),
                cv::Scalar(th.h_max,th.s_max,th.v_max),
                mask);

    cv::Mat kernel=cv::getStructuringElement(
        cv::MORPH_ELLIPSE,
        cv::Size(5,5));

    cv::morphologyEx(mask,mask,cv::MORPH_OPEN,kernel);
    cv::morphologyEx(mask,mask,cv::MORPH_CLOSE,kernel);

    return mask;
}
QImage videothread::matToQimage(cv::Mat &frame)
{
    cv::Mat rgb;

    if(frame.channels()==1)
        cv::cvtColor(frame, rgb, cv::COLOR_GRAY2RGB);
    else
        cv::cvtColor(frame, rgb, cv::COLOR_BGR2RGB);

//    return QImage(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888).copy();
    QImage img(rgb.data,
               rgb.cols,
               rgb.rows,
               rgb.step,
               QImage::Format_RGB888);

    // 320x240
    return img.scaled(320, 240, Qt::KeepAspectRatio, Qt::SmoothTransformation).copy();
}
void videothread::detectQRCode(cv::Mat &frame)
{
    // ZBar 需要 Y800 格式 (即灰度图)
    cv::Mat grayFrame;
    // 注意：传入的 frame 此时还是 BGR 格式 (在 run() 里转 RGB 之前)
    cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);

    // 3. 封装为 ZBar Image 对象
    // 参数：宽度，高度，格式("Y800"表示灰度)，数据首地址，数据总长度
    zbar::Image image(grayFrame.cols, grayFrame.rows, "Y800", grayFrame.data, grayFrame.cols * grayFrame.rows);

    // 4. 扫描图像 (返回识别到的码的数量)
    int n = scanner.scan(image);

    // 5. 处理结果
    if (n > 0) {
        // 遍历所有识别到的结果
        for (zbar::Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol) {

            //如果只想识别二维码，可以加个类型判断 (可选)
            if (symbol->get_type() != zbar::ZBAR_QRCODE) continue;

            // 获取解码内容
            std::string data = symbol->get_data();

            if (!data.empty()) {
                // 转为 QString 并发送信号
                QString result = QString::fromStdString(data);
                emit qrCodeResult(result);

                // 一般一帧只需要处理第一个二维码，节省资源
                break;
            }
        }
    }
}
void videothread::enableBlockDetect(bool flag)
{
    blockDetecting = flag;
}

void videothread::enableCircleDetect(bool flag)
{
    circleDetecting = flag;
    if (flag) {
        lostFrames = 0;        // 开启检测时重置丢帧计数
        hasLastCenter = false; // 重新进入全图搜索模式
    }
}

void videothread::enableQRDetect(bool flag)
{
    qrDetecting = flag;
}
