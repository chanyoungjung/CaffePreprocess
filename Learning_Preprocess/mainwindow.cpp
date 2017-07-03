#include "mainwindow.h"
#include "ui_mainwindow.h"



using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->checkBox_whileplayloggeddata->setChecked(flg_from_logged_data);
    ui->checkBox_fromdirectory->setChecked(flg_from_directory);
    ui->lineEdit_quantize_size->setText(QString::number(QuantizeSize));
    ui->checkBox_upsample->setChecked(false);
    ui->checkBox_downsample->setChecked(false);

    // register type using for singal
    qRegisterMetaType<cv::Mat>("cv::Mat");
    qRegisterMetaType<std::string>("std::string");
    qRegisterMetaType<cv::Point>("cv::Point");

    // connect thread, start signal with which function
    connect(this,SIGNAL(Sig_Main_2_ImgThread(bool)),img_thread,SLOT(Slot_Main_2_ImgThread(bool)));

    // connect thread, img signal with which function
    connect(img_thread,SIGNAL(Sig_ImgThread_2_Main(cv::Mat,double,std::string)),this,SLOT(Slot_ImgThread_2_Main(cv::Mat,double,std::string)));

    // connect thread, string img path signal with which function
    connect(this,SIGNAL(Sig_Main_2_ImgThread_str(std::string,std::string)),img_thread,SLOT(Slot_Main_2_ImgThread_str(std::string,std::string)));

    // connect save thread, start signal with which function
    connect(this,SIGNAL(Sig_Main_2_ImgSaveThread(bool,std::string,std::string,std::string,cv::Mat,double)),img_save_thread,SLOT(Slot_Main_2_ImgSaveThread(bool,std::string,std::string,std::string,cv::Mat,double)));

    // connect save thread, start signal with which function
    connect(this,SIGNAL(Sig_Main_2_ImgSaveThread_SaveOption(bool,bool,cv::Point,cv::Point)),img_save_thread,SLOT(Slot_Main_2_ImgSaveThread_SaveOption(bool,bool,cv::Point,cv::Point)));

    connect(img_thread,SIGNAL(Sig_ImgThread_2_ImgInferThread(cv::Mat,double,std::string)),img_infer_thread,SLOT(Slot_ImgThread_2_ImgInterThread(cv::Mat,double,std::string)));

    connect(this,SIGNAL(Sig_Main_2_ImgInferThread(bool)),img_infer_thread,SLOT(Slot_ImgThread_2_ImgInterThread_InferStatus(bool)));

    connect(img_infer_thread,SIGNAL(Sig_ImgInferThread_2_Main(double,double)),this,SLOT(Slot_ImgInferThread_2_Main(double,double)));

    setupRealtimeDataPlot(ui->widget_customplot);
//    SetTab2DataPlot(ui->widget_tab2);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SetTab2DataPlot(QCustomPlot *customPlot)
{
    // prepare data:
    QVector<double> x1(20), y1(20);

    for (int i=0; i<x1.size(); ++i)
    {
      x1[i] = i/(double)(x1.size()-1)*10;
      y1[i] = qCos(x1[i]*0.8+qSin(x1[i]*0.16+1.0))*qSin(x1[i]*0.54)+1.4;
    }

    // create and configure plottables:
    QCPGraph *graph1 = customPlot->addGraph();
    graph1->setData(x1, y1);
    graph1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1.5), QBrush(Qt::white), 9));
    graph1->setPen(QPen(QColor(120, 120, 120), 2));

    // move bars above graphs and grid below bars:
    customPlot->addLayer("abovemain", customPlot->layer("main"), QCustomPlot::limAbove);
    customPlot->addLayer("belowmain", customPlot->layer("main"), QCustomPlot::limBelow);
    graph1->setLayer("abovemain");
    customPlot->xAxis->grid()->setLayer("belowmain");
    customPlot->yAxis->grid()->setLayer("belowmain");

    // set some pens, brushes and backgrounds:
    customPlot->xAxis->setBasePen(QPen(Qt::white, 1));
    customPlot->yAxis->setBasePen(QPen(Qt::white, 1));
    customPlot->xAxis->setTickPen(QPen(Qt::white, 1));
    customPlot->yAxis->setTickPen(QPen(Qt::white, 1));
    customPlot->xAxis->setSubTickPen(QPen(Qt::white, 1));
    customPlot->yAxis->setSubTickPen(QPen(Qt::white, 1));
    customPlot->xAxis->setTickLabelColor(Qt::white);
    customPlot->yAxis->setTickLabelColor(Qt::white);
    customPlot->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    customPlot->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    customPlot->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    customPlot->xAxis->grid()->setSubGridVisible(true);
    customPlot->yAxis->grid()->setSubGridVisible(true);
    customPlot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    customPlot->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    customPlot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    customPlot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(80, 80, 80));
    plotGradient.setColorAt(1, QColor(50, 50, 50));
    customPlot->setBackground(plotGradient);
    QLinearGradient axisRectGradient;
    axisRectGradient.setStart(0, 0);
    axisRectGradient.setFinalStop(0, 350);
    axisRectGradient.setColorAt(0, QColor(80, 80, 80));
    axisRectGradient.setColorAt(1, QColor(30, 30, 30));
    customPlot->axisRect()->setBackground(axisRectGradient);

    customPlot->rescaleAxes();
    customPlot->yAxis->setRange(0, 100);
}

void MainWindow::setupRealtimeDataPlot(QCustomPlot *customPlot)
{
    customPlot->addGraph(); // blue line
    customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    customPlot->addGraph(); // red line
    customPlot->graph(1)->setPen(QPen(QColor(255, 110, 40)));

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    customPlot->xAxis->setTicker(timeTicker);
    customPlot->axisRect()->setupFullAxesBox();
    customPlot->yAxis->setRange(-150, 150);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
    dataTimer.start(0); // Interval 0 means to refresh as fast as possible
}

void MainWindow::realtimeDataPlot()
{
    static QTime time(QTime::currentTime());

    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;

    if (key-lastPointKey > 0.2) // at most add point every 2 ms
    {
      ui->widget_customplot->graph(0)->addData(key, logged_str_angle);
      ui->widget_customplot->graph(1)->addData(key, inferenced_str_angle);

      //rescale value (vertical) axis to fit the current data:
      //    ui->customPlot->graph(0)->rescaleValueAxis();
      //    ui->customPlot->graph(1)->rescaleValueAxis(true);
      lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->widget_customplot->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->widget_customplot->replot();

    // calculate frames per second:
    static double lastFpsKey;
    static int frameCount;
    ++frameCount;
    if (key-lastFpsKey > 5) // average fps over 5 seconds
    {
      ui->statusBar->showMessage(
            QString("%1 FPS, Total Data points: %2")
            .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
            .arg(ui->widget_customplot->graph(0)->data()->size()+ui->widget_customplot->graph(1)->data()->size())
            , 0);
      lastFpsKey = key;
      frameCount = 0;
    }
}

void MainWindow::on_pushButton_log2textdata_clicked()
{
    emit Sig_Main_2_ImgThread(LogStatus);
    LogStatus = !LogStatus;

    if(LogStatus == true)
    {
        ui->pushButton_log2textdata->setText("now playing data");
    }
    else
    {
        ui->pushButton_log2textdata->setText("stop playing data");
    }
}

void MainWindow::Slot_ImgInferThread_2_Main(double infered, double logged)
{
    this->inferenced_str_angle = infered;
    this->logged_str_angle = logged;

    QString tmp1 = QString::number(inferenced_str_angle);
    QString tmp2 = QString::number(logged_str_angle);

    ui->lineEdit_infered->setText(tmp1);
    ui->lineEdit_logged->setText(tmp2);

    this->realtimeDataPlot();

}

void MainWindow::Slot_ImgThread_2_Main(cv::Mat img,double str_angle,std::string timestamp_str)
{
//    cv::Mat tmp_img;
    if(!img.empty())
    {
        img.copyTo(disp_img);
//        tmp_img.copyTo(disp_img);

        ui->graphicsView_loadimg->clearMask();

        cv::Scalar color(0,0,255);
        int thickness=1;	// line thickness

//        if(ui->checkBox_cropregionupdate->isChecked())
//        {
//            cv::line(disp_img,cv::Point(lefttop_row,lefttop_col),cv::Point(lefttop_row,rightbottom_col),color,thickness);
//            cv::line(disp_img,cv::Point(lefttop_row,lefttop_col),cv::Point(rightbottom_row,lefttop_col),color,thickness);
//            cv::line(disp_img,cv::Point(rightbottom_row,rightbottom_col),cv::Point(lefttop_row,rightbottom_col),color,thickness);
//            cv::line(disp_img,cv::Point(rightbottom_row,rightbottom_col),cv::Point(rightbottom_row,lefttop_col),color,thickness);
//        }

        disp_qimg = Mat2QImage(disp_img);
        disp_pimg.convertFromImage(disp_qimg);
        disp_img_scene->clear();
        disp_img_scene->addPixmap(disp_pimg);

        ui->graphicsView_loadimg->setScene(disp_img_scene);
        ui->graphicsView_loadimg->show();

        logged_str_angle = str_angle;
        this->final_image_path = this->loadimgpath_str + timestamp_str; // save image path + timestamp.jpg
        this->jpgaddedsavepath_str = this->imagesavepath_str + timestamp_str;

        emit Sig_Main_2_ImgSaveThread(SaveStatus,savetextlabelpath_str,jpgaddedsavepath_str,final_image_path,disp_img,logged_str_angle);
    }
}


QImage MainWindow::Mat2QImage(cv::Mat src)
{
    cv::Mat temp; // make the same cv::Mat
    cv::cvtColor(src, temp,CV_BGR2RGB); // cvtColor Makes a copt, that what i need
    QImage dest((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
    dest.bits(); // enforce deep copy, see documentation
    return dest;
}


void MainWindow::on_pushButton_set_imgpath_clicked()
{
    QString directory;
    directory = QFileDialog::getExistingDirectory(this,"Find Save Folder",QDir::currentPath());

    if (!directory.isEmpty())
    {
        ui->lineEdit_imgpath->setText(directory);
        loadimgpath_str = directory.toStdString();
    }

    emit Sig_Main_2_ImgThread_str(loadimgpath_str,imagesavepath_str);

}

void MainWindow::on_pushButton_testimgpath_clicked()
{
    QString directory;
    directory = QFileDialog::getOpenFileName(this,tr("Test image open"),"",tr("JPEG (*.jpg *.jpeg);;PNG (*.png)" ));

    if (!directory.isEmpty())
    {
        ui->lineEdit_testimgpath->setText(directory);
        testimgpath_str = directory.toStdString();
    }

    disp_testimg= cv::imread(testimgpath_str);
    disp_qtestimg = Mat2QImage(disp_testimg);
    disp_ptestimg.convertFromImage(disp_qtestimg);

    testimg_size.width = disp_testimg.rows;
    testimg_size.height= disp_testimg.cols;

    ui->lineEdit_testimagesize_col->setText(QString::number(testimg_size.width));
    ui->lineEdit_testimagesize_row->setText(QString::number(testimg_size.height));
}

void MainWindow::on_pushButton_testimgshow_clicked()
{
    if(!disp_ptestimg.isNull())
    {
        ui->graphicsView_loadimg->clearMask();

        lefttop_row = ui->lineEdit_lefttop_row->text().toInt();
        lefttop_col = ui->lineEdit_lefttop_col->text().toInt();
        rightbottom_row = ui->lineEdit_rightbottom_row->text().toInt();
        rightbottom_col = ui->lineEdit_rightbottom_col->text().toInt();

        disp_testimg= cv::imread(testimgpath_str);

        cv::Scalar color(0,0,255);
        int thickness=1;	// line thickness

        cv::line(disp_testimg,cv::Point(lefttop_row,lefttop_col),cv::Point(lefttop_row,rightbottom_col),color,thickness);
        cv::line(disp_testimg,cv::Point(lefttop_row,lefttop_col),cv::Point(rightbottom_row,lefttop_col),color,thickness);
        cv::line(disp_testimg,cv::Point(rightbottom_row,rightbottom_col),cv::Point(lefttop_row,rightbottom_col),color,thickness);
        cv::line(disp_testimg,cv::Point(rightbottom_row,rightbottom_col),cv::Point(rightbottom_row,lefttop_col),color,thickness);

        disp_qtestimg = Mat2QImage(disp_testimg);
        disp_ptestimg.convertFromImage(disp_qtestimg);
        disp_img_scene->clear();

        disp_img_scene->addPixmap(disp_ptestimg);
        ui->graphicsView_loadimg->setScene(disp_img_scene);
        ui->graphicsView_loadimg->show();
    }
    else
    {
        cout << "test image load error" << endl;
    }
}

void MainWindow::on_pushButton_updatesaveoption_clicked()
{
    flg_img_crop = ui->checkBox_cropregionupdate->isChecked();
    flg_img_augmentation = ui->checkBox_imageaugmentation->isChecked();
    flg_img_togray= ui->checkBox_imagetogray->isChecked();
    flg_from_logged_data = ui->checkBox_whileplayloggeddata->isChecked();
    flg_from_directory = ui->checkBox_fromdirectory->isChecked();

    flg_from_directory = !flg_from_logged_data;

    lefttop_row = ui->lineEdit_lefttop_row->text().toInt();
    lefttop_col = ui->lineEdit_lefttop_col->text().toInt();
    rightbottom_row = ui->lineEdit_rightbottom_row->text().toInt();
    rightbottom_col = ui->lineEdit_rightbottom_col->text().toInt();

    ui->checkBox_whileplayloggeddata->setChecked(flg_from_logged_data);
    ui->checkBox_fromdirectory->setChecked(flg_from_directory);

    emit Sig_Main_2_ImgSaveThread_SaveOption(flg_img_crop,
                                             flg_img_togray,
                                             cv::Point(lefttop_row,lefttop_col),
                                             cv::Point(rightbottom_row,rightbottom_col));
}

void MainWindow::on_pushButton_clicked()
{    
    QString directory;
    directory = QFileDialog::getOpenFileName(this,tr("txt file open"),"",tr("TXT(*.txt)" ));

    if (!directory.isEmpty())
    {
        savetextlabelpath_str = directory.toStdString();
        ui->lineEdit_savetextpath->setText(QString::fromStdString(savetextlabelpath_str));
    }
}

void MainWindow::on_pushButton_SaveData_clicked()
{
    if(ui->pushButton_SaveData->isChecked())
    {
        cout << "pressed" << endl;
    }
    else
        cout << "released" << endl;

    SaveStatus = ui->pushButton_SaveData->isChecked();


}

void MainWindow::on_pushButton_imagesavepath_clicked()
{
    QString directory;
    directory = QFileDialog::getExistingDirectory(this,"Find Save Folder",QDir::currentPath());

    if (!directory.isEmpty())
    {
        imagesavepath_str = directory.toStdString();
        ui->lineEdit_imagesavepath->setText(QString::fromStdString(imagesavepath_str));
    }
}

void MainWindow::on_pushButton_InferImgPath_clicked()
{
    QString directory;
    directory = QFileDialog::getExistingDirectory(this,"Find image Folder",QDir::currentPath());

    if (!directory.isEmpty())
    {
        ui->lineEdit_InferPath->setText(directory);
        inferimgpath_str = directory.toStdString();
    }
}

void MainWindow::on_pushButton_Infer_clicked()
{
    this->InferPauseStatus = !this->InferPauseStatus;

    if(this->InferPauseStatus == true)
    {
        ui->pushButton_Infer->setText("infer...Check the image size whether it is same with train data");
    }
    else
    {
        ui->pushButton_Infer->setText("infer pause");
    }

    emit Sig_Main_2_ImgInferThread(this->InferPauseStatus);
}

void MainWindow::on_pushButton_setlabelpath_fornorm_clicked()
{
    QString directory;
    directory = QFileDialog::getOpenFileName(this,tr("txt file open"),"",tr("TXT(*.txt)" ));

    if (!directory.isEmpty())
    {
        tab2_loadlabelpath_str = directory.toStdString();
        ui->lineEdit_normlabel->setText(QString::fromStdString(tab2_loadlabelpath_str));
    }
}

void MainWindow::on_pushButton_setoutputtxt_clicked()
{
    QString directory;
    directory = QFileDialog::getOpenFileName(this,tr("txt file open"),"",tr("TXT(*.txt)" ));

    if (!directory.isEmpty())
    {
        tab2_savelabelpath_str = directory.toStdString();
        ui->lineEdit_outputnormlabel->setText(QString::fromStdString(tab2_savelabelpath_str));
    }
}

void MainWindow::on_pushButton_3_clicked()
{}

void MainWindow::on_pushButton_setoutputnormpath_clicked()
{
    QString directory;
    directory = QFileDialog::getOpenFileName(this,tr("txt file open"),"",tr("TXT(*.txt)" ));

    if (!directory.isEmpty())
    {
        tab2_savelabelpath_str= directory.toStdString();
        ui->lineEdit_outputnormlabel->setText(QString::fromStdString(tab2_savelabelpath_str));
    }
}
int LabelHistQuantize(int label)
{
    int label_hist=0;
    label_hist = label / QuantizeSize + (1000/QuantizeSize)/2;
    return label_hist;
}
void MainWindow::on_pushButton_analyze_hist_clicked()
{
//    int total_label_after_norm = 22193;

    if(!ui->checkBox_upsample->isChecked() && !ui->checkBox_downsample->isChecked())
    {
        std::cout << " Upsample or Downsample? check first" << std::endl;
    }
    else
    {

        if(tab2_loadlabelpath_str.length() != 0)
        {
            QStringList fields;

            QString filename = QString::fromStdString(tab2_loadlabelpath_str);
            QFile file(filename);

            if(!file.open(QIODevice::ReadOnly)) {
                QMessageBox::information(0, "error", file.errorString());
            }

            QTextStream read(&file);


            int hist_sum =0;
            while(!read.atEnd())
            {
                QString line = read.readLine();
                fields = line.split(" ");

                hist_arr[LabelHistQuantize(fields[1].toInt())]++;
                hist_sum++;

            }



    //        std::sort(hist_arr,hist_arr + (sizeof(hist_arr)/sizeof(hist_arr[0])));

            maxhisto_cnt = -1000;

            int range = (sizeof(hist_arr)/sizeof(hist_arr[0]));
            int hist_cnt =0;

            for(int i=0; i < range; i++)
            {
                if(hist_arr[i] > maxhisto_cnt)
                {
                    maxhisto_cnt = hist_arr[i];
                    maxhisto_index = i;
                }
                if(hist_arr[i] != 0)
                {
                    if(i < hist_indexmin)
                    {
                        hist_indexmin = i;
                    }
                    if(i > hist_indexmax)
                    {
                        hist_indexmax = i;
                    }

                    hist_cnt++;
                }
            }

    //        int ratio_leftside = (maxhisto_cnt/abs(maxhisto_index-hist_indexmin));
    //        int ratio_righttside = (maxhisto_cnt/abs(maxhisto_index-hist_indexmax));

            // max histo index cnt : 15%
            int Tobenormed_tatal_img = int(maxhisto_cnt / 0.15);
            double augmentation_ratio = double(Tobenormed_tatal_img) / hist_sum;


            for(int i=0; i < range; i++)
            {
                double gap_btw_maxhistindex = abs(maxhisto_index - i)/200.0;

                if(hist_arr[i] != 0 && i != maxhisto_index)
                {
                    if(ui->checkBox_upsample->isChecked())
                        write_ratio_arr[i] = ceil(augmentation_ratio + gap_btw_maxhistindex +1);
                    else
                        write_ratio_arr[i] = ceil(augmentation_ratio + gap_btw_maxhistindex +1);


    //                if(i < 50 || i > 150)
    //                {
    //                    write_ratio_arr[i] += 1;
    //                }

                }
                else
                    write_ratio_arr[i] = 0;
            }

        }
        else
        {
            std::cout << "set laod path first" << std::endl;
        }
        int i =0;
    }
}


void MainWindow::on_pushButton_labeltext_norm_clicked()
{
    const int contrast_augment_unit_size = 10;

    if(maxhisto_cnt != 0 && tab2_augimgpath_str.length()!=0)
    {
        QString filename_read = QString::fromStdString(tab2_loadlabelpath_str);
        QFile file_read(filename_read);

        QString filename_write = QString::fromStdString(tab2_savelabelpath_str);
        QFile file_write(filename_write);

        if(!file_read.open(QIODevice::ReadOnly)) {
            std::cout << "read file error" << std::endl;
        }
        if(!file_write.open(QIODevice::WriteOnly)) {
            std::cout << "write file error" << std::endl;
        }

        QTextStream read(&file_read);
        QStringList fields;

        QTextStream write(&file_write);

        int cnt = 0;


        while(!read.atEnd())
        {
            QString line = read.readLine();
            fields = line.split(" ");

            if(write_ratio_arr[(LabelHistQuantize(fields[1].toInt()))] != 0)
            {
                if(write_ratio_arr[(LabelHistQuantize(fields[1].toInt()))] >= 1)
                {
                    for(int i=0; i < write_ratio_arr[(LabelHistQuantize(fields[1].toInt()))]; i++)
                    {
                        string img_name = this->tab2_augimgpath_str +"/"+ fields[0].toStdString();
                        cv::Mat aug_img = cv::imread(img_name);

                        if(aug_img.rows != 0)
                        {
                            write << i<<"_"<< line << endl;
                            cv::Mat new_image = cv::Mat::zeros( aug_img.size(), aug_img.type() );

                            int randsign = rand()%2;
                            for( int y = 0; y < aug_img.rows; y++ )
                            {
                                for( int x = 0; x < aug_img.cols; x++ )
                                {

                                    for( int c = 0; c < 3; c++ )
                                    {
                                        new_image.at<cv::Vec3b>(y,x)[c] = cv::saturate_cast<uchar>( ( aug_img.at<cv::Vec3b>(y,x)[c] ) + randsign*(i+1)*contrast_augment_unit_size );
                                    }
                                }
                            }
                            string save_img_name = this->tab2_augimgpath_str +"/"+ to_string(i) + "_"+ fields[0].toStdString();
                            cv::imwrite(save_img_name,new_image);
                        }
                    }
                }
                else
                {
                    if((std::rand() % int(1/write_ratio_arr[(LabelHistQuantize(fields[1].toInt()))]) ) == 0)
                    {
                        write << line << endl;
                    }
                }
            }
            else
            {
                write << line << endl;
            }

            std::cout << " image aumenting now..." << std::endl;
        }
        std::cout << " FINISH " << std::endl;
    }
    else
    {
        std::cout << " histo analize first and set image path" << std::endl;
    }

}

void MainWindow::Tab2_Data_assign()
{
    x1.resize(1000/QuantizeSize);
    y1.resize(1000/QuantizeSize);

    for (int i=0; i<x1.size(); ++i)
    {
      x1[i] = i;
      y1[i] = hist_arr[i];
    }
}

void MainWindow::on_pushButton_tab2_imgpath_clicked()
{
    QString directory;
    directory = QFileDialog::getExistingDirectory(this,"Find Save Folder",QDir::currentPath());

    if (!directory.isEmpty())
    {
        ui->lineEdit_tab2_imgpath->setText(directory);
        tab2_imgpath_str = directory.toStdString();
    }
    path_img.setPath(directory);
    filelist = path_img.entryList(QDir::Files);


    int fileNum=0;
    fileNum = filelist.size();

    ui->progressBar->setRange(0,fileNum);

}


void MainWindow::on_pushButton_tab2_imgcrop_clicked()
{
    cv::Mat origin_img;
    cv::Mat crop_img;

    // crop option for kaist
    cv::Rect lefttop;

    lefttop.x = 50; // In tab1, lefttop row
    lefttop.y = 30; // In tab1, lefttop col
    int width = 220;
    int height = 115;

    cv::Rect roi(lefttop.x,lefttop.y,width,height);

    QString tab2_imgpath_qstr;
    tab2_imgpath_qstr = QString::fromStdString(tab2_imgpath_str);

    QString out_pathname = tab2_imgpath_qstr+"/img_processed/";
    QDir out_dir;
    out_dir.mkpath(out_pathname);

    int fileNum=0;
    fileNum = filelist.size();

    for(int i=0; i<fileNum; i++)
    {
        QString readimg_qstr = tab2_imgpath_qstr+"/"+filelist[i];
        string readimg_str = readimg_qstr.toStdString();

        origin_img = cv::imread(readimg_str);
        //cv::cvtColor(origin_img,crop_img,CV_RGB2GRAY);

//        if(origin_img.rows <= 180 || origin_img.cols <= 200)
//            continue;

        crop_img = origin_img(roi);
        string save_path = (out_pathname+filelist[i]).toStdString();
        cv::imwrite(save_path,crop_img);

        ui->progressBar->setValue(i+1);
        ui->progressBar->paintingActive();

//        cv::waitKey(10);
    }

}

void MainWindow::on_pushButton_norm2_clicked()
{
    int range = (sizeof(hist_arr)/sizeof(hist_arr[0]));
    int hist_cnt =0;

    for(int i=0; i < range; i++)
    {
        if(hist_arr[i] != 0 /*&& hist_arr[i] > maxhisto_cnt*0.01*/)
        {
            write_ratio_arr[i] = double(maxhisto_cnt / hist_arr[i])/5.0;

            if(write_ratio_arr[i] > 100)
            {
                write_ratio_arr[i] = 100;
            }
            else if(write_ratio_arr[i] < 1)
                write_ratio_arr[i] = 1;
        }
        else
            write_ratio_arr[i] = 0;
    }

    if(maxhisto_cnt != 0)
    {
        QString filename_read = QString::fromStdString(tab2_loadlabelpath_str);
        QFile file_read(filename_read);

        QString filename_write = QString::fromStdString(tab2_savelabelpath_str);
        QFile file_write(filename_write);

        if(!file_read.open(QIODevice::ReadOnly)) {
            std::cout << "read file error" << std::endl;
        }
        if(!file_write.open(QIODevice::WriteOnly)) {
            std::cout << "write file error" << std::endl;
        }

        QTextStream read(&file_read);
        QStringList fields;

        QTextStream write(&file_write);

        int cnt = 0;

        while(!read.atEnd())
        {
            QString line = read.readLine();
            fields = line.split(" ");

            if(write_ratio_arr[(LabelHistQuantize(fields[1].toInt()))] != 0)
            {
                if(write_ratio_arr[(LabelHistQuantize(fields[1].toInt()))] > 1)
                {
                    for(int i=0; i < write_ratio_arr[(LabelHistQuantize(fields[1].toInt()))]; i++)
                    {
                        write << line << endl;
                    }
                }
                else
                {
                    if((std::rand() % int(1/write_ratio_arr[(LabelHistQuantize(fields[1].toInt()))]) ) == 0)
                    {
                        write << line << endl;
                    }
                }
            }
            else
            {
                write << line << endl;
            }
        }
    }
    else
    {
        std::cout << " histo analize first " << std::endl;
    }
}

void MainWindow::on_pushButton_tab2setaugdirectory_clicked()
{
    QString directory;
    directory = QFileDialog::getExistingDirectory(this,"Find Save Folder",QDir::currentPath());

    if (!directory.isEmpty())
    {
        tab2_augimgpath_str = directory.toStdString();
        ui->lineEdit_tab2augdir->setText(QString::fromStdString(tab2_augimgpath_str));
    }
}
