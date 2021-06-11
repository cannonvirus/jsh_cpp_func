// #include <iostream>
// #include <opencv2/core.hpp>
// #include <opencv2/highgui.hpp>
// #include <opencv2/imgproc.hpp>
// #include <opencv2/videoio.hpp>
// #include <opencv2/video.hpp>
// #include <math.h>

//! ---
#include "my_optical_func.h"

int i = 0;
// OpticalImage my_optical_func;
OpticalImage my_optical_func(640, 480);
// OpticalImage my_optical_func(640, 480, 0.2, 2, 15, 3, 5, 1.2, 0);


int main()
{
    cv::VideoCapture capture(cv::samples::findFile("/works/opticalflow/data/mounting_30min.mp4"));
    if (!capture.isOpened()){
        //error in opening the video input
        std::cout << "Unable to open file!" << std::endl;
        return 0;
    }

    while(true){
        // ********************************************************************
        cv::Mat img;
        capture >> img;
        my_optical_func.Update(img);
        // ********************************************************************

        std::string file_name = "/works/opticalflow/image/output_" + std::to_string(i) + ".jpg";
        cv::imwrite(file_name, my_optical_func.Get_Flows_bgr());
    }
}