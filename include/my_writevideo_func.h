#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
#include <math.h>
#include <string>
#include <queue>
#include <thread>
#include <vector>
#include <fstream>

struct My_Object_Info {
    int track_num;
    float center_x;
    float center_y;
    float width;
    float height;
    float angle;
    float landmarksX1;
    float landmarksY1;
    bool mounting_ox;
};

class MyWriteVideo {
    public: 
        // //default option
        MyWriteVideo();

        MyWriteVideo(int bfr_volume, bool save_info, int width, int height);

        // 고정 queue에 img를 저장
        void Normal_Update(cv::Mat img);
        void Normal_Update(cv::Mat img, std::vector<My_Object_Info> cow_info);

        // 현재 녹화 중인지 
        bool Record_Switch_Status();

        // 현재 변동 queue에 img를 가지고 있는지? = Getvideo를 실행했는지?
        bool Buffer_Full_Status();

        // 고정 queue 내용을 변동 queue에 전송
        void Record_Start();

        // 변동 queue에 img를 저장
        void Record_Making(cv::Mat img);
        void Record_Making(cv::Mat img, std::vector<My_Object_Info> cow_info);

        // 현재 승가행위가 일어났는지 확인
        bool Mounting_Rec_Status();

        void Mounting_Rec_On();

        // 녹화 중 false, 버퍼 내 저장 true
        // void Record_Finish();

        // 현재 queue를 file_name으로 저장 
        void My_GetVideo(std::string file_name, int frame_rate);
        void My_GetVideo(std::string file_name, int frame_rate, std::string txt_path);

        std::string My_Vec2String(std::vector<My_Object_Info> stc_vec, int fr_num);
    
    private:
        int bfr_img_vol_; // info que와 공유
        int atr_img_vol_;
        int atr_limit_vol_;
        int concret_img_width_;
        int concret_img_height_;
        bool recording_switch_;
        bool buffer_in_video_;
        bool mounting_rec_status_;
        std::queue<cv::Mat> before_queue_;
        std::queue<cv::Mat> now_queue_;
        bool save_info_with_video_ = false;
        std::queue<std::vector<My_Object_Info>> info_before_queue_;
        std::queue<std::vector<My_Object_Info>> info_now_queue_;

};