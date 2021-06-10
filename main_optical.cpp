
//! ---
#include "my_optical_func.h"
#include "my_writevideo_func.h"

int main()
{
    cv::VideoCapture capture(cv::samples::findFile("/works/opticalflow/data/mounting_30min.mp4"));
    if (!capture.isOpened()){
        //error in opening the video input
        std::cout << "Unable to open file!" << std::endl;
        return 0;
    }

    MyWriteVideo writevideo_object;

    int i = 0;

    while(true) {
        i++;
        cv::Mat img;
        capture >> img;

        //! 항상 30프레임 고정
        
        if (i < 600) {
            writevideo_object.Normal_Update(img);
        }
        else if (i == 600) {
            writevideo_object.Record_Start();
        }
        else if (i > 600 && i < 850) {
            writevideo_object.Record_Making(img);
        }
        else if (i == 850) {
            writevideo_object.Record_Finish();
            std::string file_name = "../data/output_test_60.avi";
            writevideo_object.My_GetVideo(file_name, 30, 640, 360);
            break;
        }

    }

}