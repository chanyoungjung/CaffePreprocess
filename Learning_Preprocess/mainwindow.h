#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QImage>
#include <QtGui>
#include <QThread>
#include <QPixmap>
#include <QGraphicsScene>
#include <qvector.h>

// opencv header
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

// thread
#include "thread/img_thread.h"
#include "thread/img_save_thread.h"
#include "thread/img_infer_thread.h"
#include "qcustomplot.h"

// stl
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>

// lcm header
#include "lcm_handler.h"

// opencv header

using namespace std;

#define QuantizeSize 5

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QImage Mat2QImage(cv::Mat src);
    void setupRealtimeDataPlot(QCustomPlot *customPlot);
    void realtimeDataPlot();

    void SetTab2DataPlot(QCustomPlot *customPlot);
    void Tab2_Data_assign();


private slots:
    void on_pushButton_log2textdata_clicked();
    void on_pushButton_set_imgpath_clicked();
    void on_pushButton_testimgpath_clicked();
    void on_pushButton_testimgshow_clicked();
    void on_pushButton_updatesaveoption_clicked();
    void on_pushButton_clicked();
    void on_pushButton_SaveData_clicked();
    void on_pushButton_imagesavepath_clicked();
    void on_pushButton_InferImgPath_clicked();
    void on_pushButton_Infer_clicked();
    void on_pushButton_setlabelpath_fornorm_clicked();
    void on_pushButton_setoutputtxt_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_setoutputnormpath_clicked();
    void on_pushButton_analyze_hist_clicked();
    void on_pushButton_labeltext_norm_clicked();
    void on_pushButton_tab2_imgpath_clicked();
    void on_pushButton_tab2_imgcrop_clicked();
    void on_pushButton_norm2_clicked();
    void on_pushButton_tab2setaugdirectory_clicked();

public slots:
    void Slot_ImgThread_2_Main(cv::Mat img,double str_angle,std::string final_image_path);
    void Slot_ImgInferThread_2_Main(double infered,double logged);

signals:
    // Main to image display thread
    void Sig_Main_2_ImgThread(bool);
    void Sig_Main_2_ImgThread_str(std::string,std::string);

    // Main to image save thread
    void Sig_Main_2_ImgSaveThread(bool,std::string,std::string,std::string,cv::Mat,double);
    void Sig_Main_2_ImgSaveThread_SaveOption(bool,bool,cv::Point,cv::Point);

    // Main to inference thread
    void Sig_Main_2_ImgInferThread(bool infer_pause_status);



private:
    Ui::MainWindow *ui;

    // thread
    ImgThread* img_thread = new ImgThread;
    ImgSaveThread* img_save_thread = new ImgSaveThread;
    ImgInferThread* img_infer_thread = new ImgInferThread;

    // img
    cv::Mat disp_img;
    QImage disp_qimg;
    QPixmap disp_pimg;

    cv::Mat disp_testimg;
    QImage disp_qtestimg;
    QPixmap disp_ptestimg;

    QGraphicsScene *disp_img_scene = new QGraphicsScene;

    cv::Point test_lefttop;
    cv::Point test_rightbottom;
    cv::Size testimg_size;

    // flag
    bool LogStatus = false;
    bool SaveStatus = false;
    bool InferPauseStatus = false;

    bool flg_img_crop = false;
    bool flg_img_augmentation = false;
    bool flg_img_togray = false;
    bool flg_from_logged_data = true;
    bool flg_from_directory = !flg_from_logged_data;

    // path
    string loadimgpath_str;
    string tab2_imgpath_str;
    string final_image_path;
    string imagesavepath_str;
    string jpgaddedsavepath_str;
    string savetextlabelpath_str;
    string testimgpath_str;
    string inferimgpath_str;

    string tab2_loadlabelpath_str;
    string tab2_savelabelpath_str;
    string tab2_augimgpath_str;

    // var
    double logged_str_angle =0;
    double inferenced_str_angle;

    int lefttop_row;
    int lefttop_col;
    int rightbottom_row;
    int rightbottom_col;

    int maxhisto_cnt=0;
    int maxhisto_index=0;

    int hist_arr[1000/QuantizeSize] = {0,};
    int hist_indexmin = 10000;
    int hist_indexmax = -10000;
    double write_ratio_arr[1000/QuantizeSize] = {0.0,};
    int write_cnt_arr[1000/QuantizeSize] = {0,};

    QStringList filelist;
    QDir path_img;

    // prepare data:
    QVector<double> x1;
    QVector<double> y1;

    std::string timestamp_str;

    // timer
    QTimer dataTimer;



};

#endif // MAINWINDOW_H
