
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

    MyWriteVideo writevideo_object(60, true, 1280, 720);
    

    int i = 0;

    while(true) {
        i++;
        cv::Mat img;
        capture >> img;
        //! 항상 30프레임 고정

        std::vector<My_Object_Info> cow_info_vec;
        for(int i=1;i<5;i++) {
            My_Object_Info cow;
            cow.center_x = 10*i;
            cow.center_y = 20*i;
            cow.width = 200;
            cow.height = 400;
            cow.angle = 0.112;
            cow.landmarksX1 = 30*i;
            cow.landmarksY1 = 40*i;
            cow.track_num = i;
            cow.mounting_ox = 0;
            cow_info_vec.emplace_back(cow);
        }
        
        if (i < 600) {
            writevideo_object.Normal_Update(img, cow_info_vec);
        }
        else if (i == 600) {
            writevideo_object.Record_Start();
        }
        else if (i > 600 && i < 1000) {
            writevideo_object.Record_Making(img, cow_info_vec);
            if (writevideo_object.Buffer_Full_Status() == true) {
                std::string file_name = "/works/opticalflow/out/output_test2_30.mp4";
                writevideo_object.My_GetVideo(file_name, 30);
                break;
            }
        }
    }
}