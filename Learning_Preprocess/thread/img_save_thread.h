#ifndef IMG_SAVE_THREAD_H
#define IMG_SAVE_THREAD_H

// qt header
#include <QtCore>
#include <QThread>
#include <QMutex>
#include <QImage>
#include <QFile>

// stl
#include <iostream>
#include <string>
#include <fstream>

// opencv header
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


using namespace std;

class ImgSaveThread : public QThread {
    Q_OBJECT

public:
    ImgSaveThread() {}
    ~ImgSaveThread() {}

private:
    void run();
    void SaveData();

    // flag
    bool SaveBtnStatus = false;


    // image
    cv::Mat save_image;
    cv::Mat CropImage();
    cv::Mat ToGrayImage();

    // text file
    QFile *TextFile = new QFile;

    // var
    double steer_angle;

    // path
    std::string final_image_path;
    std::string save_text_path;
    std::string save_image_path_past;
    std::string save_image_path;

    struct option
    {
        bool SaveCrop = false;
        bool SaveToGray = false;
        cv::Point lefttop;
        cv::Point rightbottom;
    }save_option;

public slots:
    void Slot_Main_2_ImgSaveThread(bool save_status,std::string save_text_path,std::string save_image_path,std::string final_image_path,cv::Mat save_img,double str_angle);
    void Slot_Main_2_ImgSaveThread_SaveOption(bool flg_SaveCrop,bool flg_ToGray,cv::Point lefttop,cv::Point rightbottom);

};
#endif // IMG_SAVE_THREAD_H
