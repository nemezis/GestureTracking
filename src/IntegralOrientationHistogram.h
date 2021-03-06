#pragma once
#include "opencv2/opencv.hpp"

class IntegralOrientationHistogram
{
	public:
        struct Sector {
            double angle; float value;
        };

        enum integramOrientationHistogramFilterType {
            INTEGRAL_ORIENTATION_HISTOGRAM_SOBEL,
            INTEGRAL_ORIENTATION_HISTOGRAM_SCHARR
        };

		IntegralOrientationHistogram();
		IntegralOrientationHistogram(int sectors, unsigned char *data, int w, int h, int threshold);
		IntegralOrientationHistogram(int sectors, integramOrientationHistogramFilterType filterType, unsigned char *data, int w, int h, int threshold);
		IntegralOrientationHistogram(int sectors, integramOrientationHistogramFilterType filterType, cv::Mat src, int threshold);
        IntegralOrientationHistogram(int sectors, integramOrientationHistogramFilterType filterType, cv::Mat src, cv::Rect roi, int threshold);
		IntegralOrientationHistogram(int sectors, integramOrientationHistogramFilterType filterType, cv::Mat *roi, int w, int h, int threshold);
        IntegralOrientationHistogram(std::vector<Sector> histogram, bool compareOnly = true);
        IntegralOrientationHistogram(std::vector<std::vector<Sector> > histograms);
		~IntegralOrientationHistogram();
		void calculate();
		double calculateDifference(IntegralOrientationHistogram *cmpr);
		double calculateAngle(IntegralOrientationHistogram *src);
		void drawCircularHistogram();
		void rotateToMaximum();
        std::vector<Sector> getHistogram();
		cv::Mat getHistogramMat();
        std::vector<Sector> histogram;
		std::string ToString();
        cv::Mat convertHistogramVectorToMat();
	protected:
		void filter();
		//globals
		int total_sectors;
		int filterType;
	private:
        std::vector<int> getMaximums(std::vector<Sector> values, int totalmaximums);
		void filterMask(cv::Mat *mask, cv::Mat *angle, cv::Mat *magn);
		void addToHistogram(float angle);
		void initializeHistogram();
		void normalizeHistogram();
		unsigned char *data;
		int w;
		int h;
		int threshold;
		cv::Mat *dx;
		cv::Mat *dy;
		cv::Mat *roi;
		unsigned char *histogramData;
		cv::Mat circularHistogram;
		//globals
};

