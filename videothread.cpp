   #include "videothread.h"

videothread::videothread(int camera,QMutex *lock):
    running(false),cameraID(camera),data_lock(lock)
{
}

videothread::~videothread()
{

}
// 新增 stop 函数实现
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


void videothread::run(){
    running = true;
    cv::VideoCapture cap(0, cv::CAP_V4L2);
    if (!cap.isOpened()) {
        std::cerr << "无法打开摄像头" << std::endl;
        return ;
    }
    cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    cap.set(cv::CAP_PROP_FPS, 60);

    cv::Mat tmp_frame;
    cv::Mat hsv;

    while(running){
        cap >> tmp_frame;
        if (tmp_frame.empty()){
            break;
        }
        cv::Mat blur;
        cv::GaussianBlur(tmp_frame,blur,cv::Size(7, 7),1.5);
        // ==================== 1. 颜色转换 (始终执行) ====================
        cv::cvtColor(blur, hsv, cv::COLOR_RGB2HSV);

        // ==================== 2. 生成 Mask (始终执行，用于预览) ====================
        cv::Mat mask;
        {
            // 加锁读取阈值，防止冲突
            QMutexLocker locker(data_lock);
            cv::Scalar lower_green(h_min, s_min, v_min);
            cv::Scalar upper_green(h_max, s_max, v_max);
            cv::inRange(hsv, lower_green, upper_green, mask);
        }

        // 发送掩膜给 UI 线程显示 (Qt 会自动复制 Mat 数据，所以这里是安全的)
        emit previewReady(&mask);

        // ==================== 3. 二维码识别 (始终执行) ====================
        if (qrdetecing){
            qrCode(tmp_frame);
        }

        // ==================== 4. 圆检测 (仅在开启时执行) ====================
        if (detecting){
            // 将生成的 mask 传给检测函数，避免重复计算
            circleDetect(tmp_frame, mask);
        }

        // ==================== 5. 发送原始帧 (带绘制结果) ====================
        // tmp_frame 此时可能被 circleDraw 画上了圆
        data_lock->lock();
        frame = tmp_frame;
        data_lock->unlock();
        emit frameCaptured(&frame);
    }

    cap.release();
    running = false;
}

// 修改 circleDetect，接收 mask 作为参数
void videothread::circleDetect(cv::Mat &frame, const cv::Mat &mask) {

    // 膨胀
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(7, 7));
//    cv::Mat dilated_mask;
//    cv::dilate(mask, dilated_mask, kernel);
    // 闭运算
    cv::Mat closed_mask;
    cv::morphologyEx(mask, closed_mask, cv::MORPH_CLOSE, kernel);
//    cv::morphologyEx(closed_mask, closed_mask, cv::MORPH_CLOSE, kernel);

    // 霍夫圆变换
    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(
        closed_mask,
        circles,
        cv::HOUGH_GRADIENT,
        1.2,
        closed_mask.rows / 8,
                100,             // param1: Canny 高阈值
                50,              // param2: 累加器阈值，从30提高到45，减少误检
                20,              // minRadius: 最小半径，根据实际情况调整
                180              // maxRadius: 最大半径
    );

    // 筛选并绘制
    if (!circles.empty()) {
        int max_idx = 0;
        float max_r = 0;
        for(size_t i = 0; i < circles.size(); i++) {
            if (circles[i][2] > max_r) {
                max_r = circles[i][2];
                max_idx = i;
            }
        }

        cv::Vec3i c = circles[max_idx];
        cv::Point center = cv::Point(c[0], c[1]);
        int radius = c[2];

        // 绘制圆心和轮廓
        cv::circle(frame, center, 3, cv::Scalar(0, 255, 255), -1, cv::LINE_AA);
        cv::circle(frame, center, radius, cv::Scalar(255, 0, 0), 3, cv::LINE_AA);
    }
}


void videothread::setHSVThreshold(int hMin, int hMax, int sMin, int sMax, int vMin, int vMax)
{
    data_lock->lock();
    h_min = hMin;
    h_max = hMax;
    s_min = sMin;
    s_max = sMax;
    v_min = vMin;
    v_max = vMax;
    data_lock->unlock();
}





void videothread::qrCode(cv::Mat &frame) {
    // 1. 创建 ZBar 扫描器
    zbar::ImageScanner scanner;

    // 配置：启用所有类型的码 (二维码、条形码等)
    scanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);

    // 2. 图像预处理
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

    // 6. 数据清理 (zbar::Image 析构时会自动清理，无需手动释放 grayFrame 数据)
}

