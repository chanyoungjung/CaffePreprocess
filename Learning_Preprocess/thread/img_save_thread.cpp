#include "img_save_thread.h"

void ImgSaveThread::Slot_Main_2_ImgSaveThread(bool save_status,std::string save_text_path,std::string save_image_path,std::string final_image_path,cv::Mat save_img,double str_angle)
{
    SaveBtnStatus = save_status;
    this->save_text_path= save_text_path;
    this->save_image_path= save_image_path;
    this->save_image = save_img;
    this->steer_angle = str_angle;
    this->final_image_path = final_image_path;

    if(this->SaveBtnStatus == false)
    {
        while(this->isRunning()) {}
        this->exit();
    }
    else
    {
        if(!this->isRunning())
        {
            this->start();
        }
    }
}

void ImgSaveThread::Slot_Main_2_ImgSaveThread_SaveOption(bool flg_SaveCrop, bool flg_ToGray, cv::Point lefttop, cv::Point rightbottom)
{
    save_option.SaveCrop = flg_SaveCrop;
    save_option.SaveToGray = flg_ToGray;
    save_option.lefttop = lefttop;
    save_option.rightbottom = rightbottom;
}

cv::Mat ImgSaveThread::CropImage()
{
    cv::Mat crop_img;
    cv::Rect roi(save_option.lefttop.x,save_option.lefttop.y,
                 (save_option.rightbottom.x - save_option.lefttop.x), // width
                 (save_option.rightbottom.y - save_option.lefttop.y)); // height

    if(save_option.rightbottom.x - save_option.lefttop.x <= 0 || save_option.rightbottom.x - save_option.lefttop.x > save_image.cols
            ||(save_option.rightbottom.y - save_option.lefttop.y) <= 0 || (save_option.rightbottom.y - save_option.lefttop.y) > save_image.rows )
    {
        cout << " crop size error" << endl;
        return crop_img;
    }

    if(roi.width > 0 && roi.height > 0 && save_image.rows != 0)
        crop_img = save_image(roi);
    return crop_img;
}

cv::Mat ImgSaveThread::ToGrayImage()
{
    cv::Mat gray_img;
    cv::cvtColor(save_image,gray_img,CV_RGB2GRAY);
    return gray_img;
}

void ImgSaveThread::SaveData()
{
    if(!TextFile->isOpen())
    {
        QString filename = QString::fromStdString(save_text_path);
        TextFile->setFileName(filename);
        TextFile->open(QIODevice::WriteOnly);
    }

    QTextStream out(TextFile);

    if(save_image_path != save_image_path_past)
        out << QString::fromStdString(save_image_path) << " " << int(steer_angle) << "\n";

    cv::Mat processed_img = save_image;

    if(this->save_option.SaveCrop == true) // crop image
    {
        processed_img = CropImage();
        if(this->save_option.SaveToGray == true) // cropped image to gray scale and save
        {
            cv::cvtColor(processed_img,processed_img,CV_RGB2GRAY);
        }
    }
    else // raw image
    {
        if(this->save_option.SaveToGray == true) // image to gray scale and save
        {
            cv::cvtColor(processed_img,processed_img,CV_RGB2GRAY);
        }
    }

    save_image_path_past = save_image_path;
    if(processed_img.rows !=0 && this->save_image_path.length() != 0)
        imwrite(this->save_image_path,processed_img);
}

void ImgSaveThread::run()
{
    while(SaveBtnStatus == true)
    {
        SaveData();
        QThread::msleep(1);
    }
}
