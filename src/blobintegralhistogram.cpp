#include "blobintegralhistogram.h"

BlobIntegralHistogram::BlobIntegralHistogram(size_t sectors, cv::Mat &blobMask, cv::Point histCenter)
{
    this->sectors = sectors;
    this->blobMask = blobMask;
    this->histCenter = histCenter;
    circularHistogram = cv::Mat(400, 400, CV_8UC3);
    area = blobMask.rows * blobMask.cols;
    InitializeHistogram();
}

BlobIntegralHistogram::BlobIntegralHistogram(std::vector<float> histogram)
{
    this->sectors = histogram.size();
    this->circularHistogram = cv::Mat(400, 400, CV_8UC3);
    InitializeHistogram();
    for (size_t i = 0; i < histogram.size(); ++i) {
        this->histogram.at(i).value = histogram.at(i);
    }
}

void BlobIntegralHistogram::Calculate()
{
    for (int y = 0; y < blobMask.rows; ++y){
        for (int x = 0; x < blobMask.cols; ++x){
            if (blobMask.at<uchar>(cv::Point(x,y)) > 150){
                double angle = atan2(static_cast<double>(histCenter.y - y), static_cast<double>(histCenter.x - x));
                AddToHistogram(angle - (CV_PI / 2));
            }
        }
    }
    NormalizeHistogram();
}

void BlobIntegralHistogram::Plot()
{
    int height = 400; int width = 400;
    circularHistogram.setTo(cv::InputArray(cv::Scalar(255,255,255)));
    for (int i = 0; i < this->sectors; ++i){
        double angle = histogram.at(i).angle;
        angle -= CV_PI / 2;
        double current_value = histogram.at(i).value * 1000;
        cv::Point endpoint = cv::Point((current_value*cos(angle))+width/2, height/2+(current_value*sin(angle)));
        cv::line(circularHistogram, cv::Point(width/2,height/2), endpoint, cv::Scalar(0,0,255),1,8,0);
    }
}

void BlobIntegralHistogram::InitializeHistogram()
{
    this->histogram.clear();
    for (int i = 0; i < this->sectors; ++i){
        IntegralOrientationHistogram::Sector current;
        current.value = 0;
        current.angle = (2*CV_PI/this->sectors)*i;
        this->histogram.push_back(current);
    }
}

void BlobIntegralHistogram::NormalizeHistogram()
{
    double norm = 1.0/(double) area;
    for (size_t i = 0; i < histogram.size(); ++i){
        histogram.at(i).value = ((histogram.at(i).value*norm));
    }
}

void BlobIntegralHistogram::AddToHistogram(double angle) {
    if (angle < 0){
        angle += (CV_PI*2);
    }

    for (int i = 0; i < this->sectors; ++i){
        if (histogram.at(i).angle > angle){
            if (i-1 > -1)
                histogram.at(i-1).value++;
            else
                histogram.at(histogram.size()-1).value++;
            break;
        }
    }
}




