#ifndef IMG_INFER_THREAD_H
#define IMG_INFER_THREAD_H

// qt header
#include <QtCore>
#include <QThread>
#include <QMutex>
#include <QImage>
#include <QFile>
#include <QMainWindow>
#include <QFileDialog>
#include <QtGui>
#include <QPixmap>
#include <QGraphicsScene>

// stl
#include <math.h>
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <dirent.h>
#include <algorithm>
#include <fstream>
#include <sys/time.h>
#include <ctime>
#include <cstdlib>
#include <sstream>

// opencv header
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// caffe header
#include <caffe/caffe.hpp>

// local path
#include "classification.hpp"

using namespace std;

extern vector<string> angle_estimation(Classifier classifier, cv::Mat inputimg);

class ImgInferThread : public QThread {
    Q_OBJECT

public:
    ImgInferThread();
    ~ImgInferThread() {}

private:
    void run();
    void Inference();

private:
    cv::Mat InferImg;
    std::string timestamp;
    double logged_str_angle;
    double infered_str_angle;

    Classifier classifier_obj;

//    // for inje speedium
//    string model_file = "/home/jay/Qt_workspace/build-AVM_gen_infersteer-Desktop_Qt_5_8_0_GCC_64bit-Debug/deploy_inje.prototxt";
//    string weight_file = "/media/jay/Data/E2E_inje_28_30_tatal_weight/weight_file_iter_65494.caffemodel";
//    string label_file = "/home/jay/Qt_workspace/build-AVM_gen_infersteer-Desktop_Qt_5_8_0_GCC_64bit-Debug/label.txt";
//    string mean_file = "/media/jay/Data/train_mean2.binaryproto";

//    // for kaist
//    string model_file = "/home/jay/Downloads/caffe-master/data/E2E_kaist/model_weight_bias_0/deploy2.prototxt";
//    string weight_file = "/home/jay/Downloads/caffe-master/data/E2E_kaist/model_weight_bias_0/weight/weight_iter_12091.caffemodel";
//    string label_file = "/home/jay/Qt_workspace/build-AVM_gen_infersteer-Desktop_Qt_5_8_0_GCC_64bit-Debug/label.txt";
//    string mean_file = "/media/jay/Data/kaist_data/norm_train_mean_227_small.binaryproto";

//    // /media/jay/Data/TRAINNING_DATA/KAIST/total_until_20
//    string model_file = "/home/jay/Downloads/caffe-master/data/E2E_kaist/170420_with_augment/deploy2.prototxt";
//    string weight_file = "/media/jay/Data/TRAINNING_DATA/KAIST/total_until_20/weights/weight__iter_63940.caffemodel";
//    string label_file = "/home/jay/Qt_workspace/build-AVM_gen_infersteer-Desktop_Qt_5_8_0_GCC_64bit-Debug/label.txt";
//    string mean_file = "/media/jay/Data/TRAINNING_DATA/KAIST/total_until_20/mean/train_mean_1.binaryproto";

    // /media/jay/Data/TRAINNING_DATA/KAIST/total_until_20
    string model_file = "/media/jay/Data/training_data_veloster/KAIST/model/deploy2.prototxt";
    string weight_file = "/media/jay/Data/training_data_veloster/KAIST/weights/weights_iter_18419.caffemodel";
    string label_file = "/home/jay/Qt_workspace/build-AVM_gen_infersteer-Desktop_Qt_5_8_0_GCC_64bit-Debug/label.txt";
    string mean_file = "/media/jay/Data/training_data_veloster/KAIST/mean/train_mean_aug.binaryproto";


//    string model_file = "/media/jay/Data/training_data_veloster/KAIST/model/deploy2.prototxt";
//    string weight_file = "/media/jay/Data/training_data_veloster/KAIST/weights/weights_TEST_iter_6261.caffemodel";
//    string label_file = "/home/jay/Qt_workspace/build-AVM_gen_infersteer-Desktop_Qt_5_8_0_GCC_64bit-Debug/label.txt";
//    string mean_file = "/media/jay/Data/training_data_veloster/KAIST/mean/train_mean_TEST.binaryproto";


    bool inferstatus;

public slots:
    void Slot_ImgThread_2_ImgInterThread(cv::Mat load_img,double logged_str_angle,std::string timestamp_str);
    void Slot_ImgThread_2_ImgInterThread_InferStatus(bool infer_status);

signals:
    void Sig_ImgInferThread_2_Main(double,double);
};


#endif // IMG_INFER_THREAD_H
