#include "img_infer_thread.h"
#include "classification.hpp"

ImgInferThread::ImgInferThread()
{
    classifier_obj.net_initialize(model_file,weight_file,label_file,mean_file);
}


void ImgInferThread::Slot_ImgThread_2_ImgInterThread(cv::Mat load_img, double logged_str_angle, string timestamp_str)
{
    this->InferImg = load_img;
    this->logged_str_angle = logged_str_angle;
    this->timestamp = timestamp_str;

}

void ImgInferThread::Slot_ImgThread_2_ImgInterThread_InferStatus(bool infer_status)
{
    this->inferstatus = infer_status;

    if(!inferstatus)
    {
        while(this->isRunning()) {}
        this->exit();
    }
    else
    {
        if(!this->isRunning())
        {\
            this->start();
        }
    }
}

void ImgInferThread::run()
{
    while(inferstatus)
    {
        if(InferImg.size().width == 0)
        {
            continue;
        }
        Inference();
        QThread::msleep(1);
    }
}

void ImgInferThread::Inference()
{
    cv::Mat CropImg;
    cv::Point lefttop;

    // crop option for inje
//    lefttop.x = 100;
//    lefttop.y = 150;
//    int width = 400;
//    int height = 150;

    // crop option for kaist
//    lefttop.x = 180;
//    lefttop.y = 200;
//    int width = 320;
//    int height = 150;

//    // crop option for kaist_small image
//    lefttop.x = 230;
//    lefttop.y = 230;
//    int width = 220;
//    int height = 150;

    // /home/jay/Downloads/caffe-master/data/E2E_kaist/170420_with_augment
//    lefttop.x = 230;
//    lefttop.y = 240;
//    int width = 250;
//    int height = 110;

////     if no needs to crop...
    lefttop.x = 0;
    lefttop.y = 0;
    int width = InferImg.size().width;
    int height = InferImg.size().height;

//     /home/jay/Downloads/caffe-master/data/E2E_kaist/170420_with_augment
//    lefttop.x = 100;
//    lefttop.y = 300;
//    int width = 600;
//    int height = 200;


    cv::Rect roi(lefttop.x,lefttop.y,width,height);
    CropImg = InferImg(roi);

    vector<string> classification_result = angle_estimation(classifier_obj, CropImg);

    infered_str_angle = atof(classification_result[1].c_str());

    emit Sig_ImgInferThread_2_Main(infered_str_angle,logged_str_angle);

//    cout << "infer : " << infered_str_angle << "        " << "logged : " << logged_str_angle << endl;




}
