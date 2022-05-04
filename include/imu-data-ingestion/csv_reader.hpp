#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "opencv2/opencv.hpp"
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <numeric>
#include <vector>

class CsvReader
{
    private:

        std::string fileName;
        inline static const std::string newVideoName = "eltekor.avi";
        std::vector<std::vector<std::string>> content;

        std::vector<float> accX;
        std::vector<float> accY;
        std::vector<float> accZ;

    public:

        CsvReader(const std::string &fileName) : fileName(fileName) {}
        ~CsvReader() {}

        // Main functions
        void retrieveFileItems();
        void processData();
        void writeToVideo(const std::string &fileName);

        // Helper functions
        std::vector<std::string> split(const std::string &s, char delim) const;
        float getAverage(const std::vector<float> &v);
        void drawLine(cv::Mat &img, const cv::Point &start, const cv::Point &end, const cv::Scalar color) const;
        void showFrames() const;

};