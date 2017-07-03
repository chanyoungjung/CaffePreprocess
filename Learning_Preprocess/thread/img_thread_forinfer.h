#ifndef IMG_THREAD_FORINFER_H
#define IMG_THREAD_FORINFER_H

// lcm header
#include <lcm/lcm.h>
#include <lcm/lcm-cpp.hpp>
#include "lcm_handler.h"

// qt header
#include <QtCore>
#include <QThread>
#include <QMutex>
#include <QImage>

// opencv header
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// stl
#include <iostream>
#include <string>

using namespace std;

class ImgThreadforinfer : public QThread {
    Q_OBJECT

public:
    ImgThreadforinfer() {}
    ~ImgThreadforinfer() {}

private:
    void run();
    void LoadImg();

    // image
    cv::Mat load_img;

    // flag
    bool pause_status = false;

    // lcm
    Handler handler_obj;
    lcm::LCM lcm_obj;

    // path
    string img_path = "";
    string final_img_path = "";
    string image_save_str;
    string timestamp;

    // var
    double logged_steer_angle;

public slots:

signals:

};

#endif // IMG_THREAD_FORINFER_H
