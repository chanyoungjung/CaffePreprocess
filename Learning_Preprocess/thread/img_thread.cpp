#include "img_thread.h"


void ImgThread::Slot_Main_2_ImgThread(bool _pause_status)
{
    pause_status = _pause_status;

    if(pause_status)
    {
        while(this->isRunning()) {}
        this->exit();
    }
    else
    {
        if(!this->isRunning())
        {
            lcm_obj.subscribe("CAM",&Handler::handleMessage_cam, &handler_obj);
            lcm_obj.subscribe("CAN_T", &Handler::handleMessage_can, &handler_obj);

            this->start();
        }
    }
}

void ImgThread::Slot_Main_2_ImgThread_str(std::string _path_str,std::string image_save_str)
{
    img_path = _path_str;
//    this->image_save_str= image_save_str;
}


void ImgThread::LoadImg()
{
    lcm_obj.handleTimeout(10);

    if(!handler_obj.timestamp == 0)
    {
        final_img_path = this->img_path + "/" + to_string(handler_obj.timestamp) + ".jpg";
        this->timestamp = "/" + to_string(handler_obj.timestamp) + ".jpg";
    }

    if(final_img_path.length() != 0)
    {
        this->load_img = cv::imread(final_img_path);
        logged_steer_angle = handler_obj.str_ang;
    }
}

void ImgThread::run()
{
    while(!pause_status)
    {
        LoadImg();
        emit Sig_ImgThread_2_Main(load_img,logged_steer_angle,timestamp);
        emit Sig_ImgThread_2_ImgInferThread(load_img,logged_steer_angle,timestamp);
        QThread::msleep(1);
    }
}
