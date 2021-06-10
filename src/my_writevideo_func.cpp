#include "my_writevideo_func.h"

MyWriteVideo::MyWriteVideo() {
    bfr_img_vol_ = 60;
    concret_img_width_ = 640;
    concret_img_height_ = 480;
    recording_switch_ = false;
    buffer_in_video_ = false;
    atr_img_vol_ = 0;
    atr_limit_vol_ = 300;
}

MyWriteVideo::MyWriteVideo(int bfr_volume, bool save_info, int width, int height) {
    bfr_img_vol_ = bfr_volume;
    concret_img_width_ = width;
    concret_img_height_ = height;
    recording_switch_ = false;
    buffer_in_video_ = false;
    atr_img_vol_ = 0;
    atr_limit_vol_ = 300;
    save_info_with_video_ = save_info;
}

// ===================================================================================================================================

// NOTE : Normal_Update 2가지 유형
// void Normal_Update(cv::Mat img)
// void Normal_Update(cv::Mat img, std::vector<My_Object_Info> cow_info)

void MyWriteVideo::Normal_Update(cv::Mat img) {
    cv::Mat tmp_img, rgb_img;
    cv::resize(img, tmp_img, cv::Size(concret_img_width_,concret_img_height_));
    if (tmp_img.type() != 16) {
        cv::cvtColor(tmp_img , rgb_img , cv::COLOR_RGBA2BGR);
    } else {
        rgb_img = tmp_img;
    }
    before_queue_.push(rgb_img);
    if (int(before_queue_.size()) == bfr_img_vol_+1) {
        before_queue_.pop();
    }
}

void MyWriteVideo::Normal_Update(cv::Mat img, std::vector<My_Object_Info> cow_info) {
    cv::Mat tmp_img, rgb_img;
    cv::resize(img, tmp_img, cv::Size(concret_img_width_,concret_img_height_));
    if (tmp_img.type() != 16) {
        cv::cvtColor(tmp_img , rgb_img , cv::COLOR_RGBA2BGR);
    } else {
        rgb_img = tmp_img;
    }
    before_queue_.push(rgb_img);
    if (int(before_queue_.size()) == bfr_img_vol_+1) {
        before_queue_.pop();
    }
    if (save_info_with_video_) {
        info_before_queue_.push(cow_info);
        if (int(info_before_queue_.size()) == bfr_img_vol_+1) {
            info_before_queue_.pop();
        }
    }
}
// ===================================================================================================================================

bool MyWriteVideo::Record_Switch_Status() {
    return recording_switch_;
}

bool MyWriteVideo::Buffer_Full_Status() {
    return buffer_in_video_;
}

bool MyWriteVideo::Mounting_Rec_Status(){
    return mounting_rec_status_;
}

void MyWriteVideo::Mounting_Rec_On(){
    mounting_rec_status_ = true;
}

void MyWriteVideo::Record_Start() {
    recording_switch_ = true;
    now_queue_ = before_queue_;
    if (save_info_with_video_) {
        info_now_queue_ = info_before_queue_;
    }
}

// ===================================================================================================================================
// NOTE : Record_Making 2가지 유형
// void Record_Making(cv::Mat img)
// void Record_Making(cv::Mat img, std::vector<My_Object_Info> cow_info)

void MyWriteVideo::Record_Making(cv::Mat img) {
    if (recording_switch_ == true) {
        cv::Mat tmp_img, rgb_img;
        cv::resize(img, tmp_img, cv::Size(concret_img_width_,concret_img_height_));
        if (tmp_img.type() != 16) {
            cv::cvtColor(tmp_img , rgb_img , cv::COLOR_RGBA2BGR);
        } else {
            rgb_img = tmp_img;
        }
        now_queue_.push(rgb_img);
        atr_img_vol_++;
        if (atr_img_vol_ + bfr_img_vol_ >= atr_limit_vol_) {
            recording_switch_ = false;
            buffer_in_video_ = true;
            atr_img_vol_ = 0;
            mounting_rec_status_ = false;
            std::cout << "FULL CHARGE" << std::endl;
        }
    }
}

void MyWriteVideo::Record_Making(cv::Mat img, std::vector<My_Object_Info> cow_info) {
    if (recording_switch_ == true) {
        cv::Mat tmp_img, rgb_img;
        cv::resize(img, tmp_img, cv::Size(concret_img_width_,concret_img_height_));
        if (tmp_img.type() != 16) {
            cv::cvtColor(tmp_img , rgb_img , cv::COLOR_RGBA2BGR);
        } else {
            rgb_img = tmp_img;
        }
        now_queue_.push(rgb_img);
        info_now_queue_.push(cow_info);
        atr_img_vol_++;
        if (atr_img_vol_ + bfr_img_vol_ >= atr_limit_vol_) {
            recording_switch_ = false;
            buffer_in_video_ = true;
            atr_img_vol_ = 0;
            mounting_rec_status_ = false;
            std::cout << "FULL CHARGE" << std::endl;
        }
    }
}
// ===================================================================================================================================

void MyWriteVideo::My_GetVideo(std::string file_name, int frame_rate) {
    std::cout << "Making Record img to Video" << std::endl;
    cv::VideoWriter video(file_name, cv::VideoWriter::fourcc('X','2','6','4'), frame_rate, cv::Size(concret_img_width_,concret_img_height_));
    // cv::Mat tmp_img;
    while(!now_queue_.empty()) {
        video.write(now_queue_.front());
        now_queue_.pop();
    }
    video.release();
    buffer_in_video_ = false;
    std::cout << "Making Video End ~~" << std::endl;
}

void MyWriteVideo::My_GetVideo(std::string file_name, int frame_rate, std::string txt_path) {
    std::cout << "Making Record img to Video" << std::endl;
    cv::VideoWriter video(file_name, cv::VideoWriter::fourcc('X','2','6','4'), frame_rate, cv::Size(concret_img_width_,concret_img_height_));
    // cv::Mat tmp_img;
    int fr_num = 0;
    std::string txt_string = "";
    while(!now_queue_.empty()) {
        video.write(now_queue_.front());
        now_queue_.pop();
        txt_string += My_Vec2String(info_now_queue_.front(), fr_num);
        info_now_queue_.pop();
        fr_num++;
    }
    video.release();
    buffer_in_video_ = false;
    std::cout << "Making Video End ~~" << std::endl;

    std::ofstream writeFile;
    writeFile.open(txt_path);
    writeFile.write(txt_string.c_str(), txt_string.size());
    writeFile.close();
    std::cout << "Making Text End ~~" << std::endl;
}

std::string MyWriteVideo::My_Vec2String(std::vector<My_Object_Info> stc_vec, int fr_num) {

    std::string str = "";
    for(auto iter=stc_vec.begin(); iter!=stc_vec.end(); iter++) {
        str += std::to_string(fr_num);
        str += ",";
        str += std::to_string(int((*iter).center_x));
        str += ",";
        str += std::to_string(int((*iter).center_y));
        str += ",";
        str += std::to_string(int((*iter).width));
        str += ",";
        str += std::to_string(int((*iter).height));
        str += ",";
        str += std::to_string((*iter).angle);
        str += ",";
        str += std::to_string(int((*iter).landmarksX1));
        str += ",";
        str += std::to_string(int((*iter).landmarksY1));
        str += ",";
        str += std::to_string(int((*iter).mounting_ox));
        str += "\n";
    }
    return str;
}







