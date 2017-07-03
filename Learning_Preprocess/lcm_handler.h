#ifndef LCM_HANDLER_H
#define LCM_HANDLER_H

// std header
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>

// lcm header
#include <lcm/lcm.h>
#include <lcm/lcm-cpp.hpp>
#include "exlcm/example_t.hpp"
#include "eurecar/can_t.hpp"
#include "eurecar/avm.hpp"
#include "eurecar/CAM.hpp"

using namespace std;

class Handler
{
    public:
    ~Handler() {}

    long timestamp;
    double str_ang;

    ofstream logfile;

    //specify save path in absolute directory
    const char *savepath = "/home/jay/caffe-segnet/data/steer_zed/data/label_int.txt";

    bool fileIsOpen=0;
    bool SteerTurn=0;
    bool TimeStampTurn=1;

    void handleMessage_can(const lcm::ReceiveBuffer* rbuf,const std::string& chan,const eurecar::can_t* can_msg)//,const eurecar::avm* avm_msg)
    {
        str_ang = can_msg->mdps_str_ang;

//        if(fileIsOpen==0)
//        {
//            logfile.open(savepath);
//                fileIsOpen =1;
//        }
//        else if(fileIsOpen ==1 && SteerTurn==1)
//        {
//                logfile << (int)str_ang << endl;
//            SteerTurn=0;
//            TimeStampTurn=1;
//        }
    }

    void handleMessage_cam(const lcm::ReceiveBuffer* rbuf,const std::string& chan,const eurecar::avm* CAM_msg)
    {
        timestamp = CAM_msg->timestamp;

//        if(fileIsOpen==0)
//        {
//            logfile.open(savepath);
//                fileIsOpen =1;
//        }
//        else if(fileIsOpen ==1 && TimeStampTurn ==1)
//        {
//           timestamp = CAM_msg->timestamp;
//           //cout << "      time : " << timestamp << endl;

//           logfile << timestamp << ".jpg ";
//           TimeStampTurn =0;
//           SteerTurn =1;
//        }
    }

};

#endif // LCM_HANDLER_H
