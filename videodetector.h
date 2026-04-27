#ifndef VIDEODETECTOR_H
#define VIDEODETECTOR_H

#include <QObject>

class videodetector : public QObject
{
    Q_OBJECT
public:
    explicit videodetector(QObject *parent = nullptr);



private:

    //1:red 2:green 3:blue
    int colorMode = 1;


    struct HSVThreshold {
        int h_min, h_max;
        int s_min, s_max;
        int v_min, v_max;
    };

    HSVThreshold redThreshold1;
    HSVThreshold redThreshold2;
    HSVThreshold greenThreshold;
    HSVThreshold blueThreshold;




    // 霍夫圆形检测参数
    int dp_val = 12;       // 分辨率比值 (实际值 = dp_val / 10.0)
    int min_dist = 100;    // 圆心最小距
    int param1_val = 50;   // Canny
    int param2_val = 30;   // 累加器
    int min_radius = 10;   // 最小半径
    int max_radius = 300;  // 最大半径




signals:

};

#endif // VIDEODETECTOR_H
