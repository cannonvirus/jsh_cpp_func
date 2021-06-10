#include "my_optical_func.h"

OpticalImage::OpticalImage() {
    img_cols_ = 1280;
    img_rows_ = 720;
    pyr_scale_ = 0.2;
    levels_ = 3;
    winsize_ = 15;
    iterations_ = 3;
    poly_n_ = 5;
    poly_sigma_ = 1.2;
    flags_ = 0;
}

OpticalImage::OpticalImage(const int cols_, const int rows_) {
    img_cols_ = cols_;
    img_rows_ = rows_;
    pyr_scale_ = 0.2;
    levels_ = 3;
    winsize_ = 15;
    iterations_ = 3;
    poly_n_ = 5;
    poly_sigma_ = 1.2;
    flags_ = 0;
}

OpticalImage::OpticalImage(const int cols_, const int rows_, const float pyr_scale, const int levels, const int winsize, const int iterations, \
    const int poly_n, const float poly_sigma, const int flags) {

    img_cols_ = cols_;
    img_rows_ = rows_;
    pyr_scale_ = pyr_scale;
    levels_ = levels;
    winsize_ = winsize;
    iterations_ = iterations;
    poly_n_ = poly_n;
    poly_sigma_ = poly_sigma;
    flags_ = flags;
}

void OpticalImage::Update(cv::Mat img) {
    
    //! prvs_ 초기값 설정
    if(self_init_ == true) {
        // img_cols_ = img.cols; // 1920
        // img_rows_ = img.rows; // 1080
        cv::Mat frame_resize;
        cv::resize( img, frame_resize, cv::Size( int(img_cols_), int(img_rows_) ), 0, 0, cv::INTER_LINEAR );
        cvtColor(frame_resize, prvs_, cv::COLOR_BGR2GRAY);
        self_init_ = false;
        std::cout << "@@@ optical func init complete @@@" << std::endl;
        std::cout << "cols : " << int(img_cols_) << "  rows : " << int(img_rows_) << std::endl;
    }
    cv::Mat img_resize_, next;

    cv::resize( img, img_resize_, cv::Size( int(img_cols_), int(img_rows_) ), 0, 0, cv::INTER_LINEAR );
    if (img.empty()) {
        std::cout << "IMAGE NOT INPUT, plz check frame" << std::endl;
    }
    cvtColor(img_resize_, next, cv::COLOR_BGR2GRAY);
    cv::Mat flow(prvs_.size(), CV_32FC2);

    // cv::calcOpticalFlowFarneback(prvs_, next, flow, 0.2, 2, 15, 3, 5, 1.2, 0);
    cv::calcOpticalFlowFarneback(prvs_, next, flow, pyr_scale_, levels_, winsize_, iterations_, poly_n_, poly_sigma_, flags_);
        
    cv::Mat flow_parts[2];
    split(flow, flow_parts);
    cv::Mat magnitude, angle, magn_norm;
    cartToPolar(flow_parts[0], flow_parts[1], magnitude, angle, true);
    normalize(magnitude, magn_norm, 0.0f, 1.0f, cv::NORM_MINMAX);
    angle *= ((1.f / 360.f) * (180.f / 255.f));
    //build hsv image
    cv::Mat _hsv[3], hsv, hsv8;
    _hsv[0] = angle;
    _hsv[1] = cv::Mat::ones(angle.size(), CV_32F);
    _hsv[2] = magn_norm;
    merge(_hsv, 3, hsv);
    hsv.convertTo(hsv8, CV_8U, 255.0);
    cvtColor(hsv8, bgr_, cv::COLOR_HSV2BGR);
    prvs_ = next;
}

cv::Mat OpticalImage::Get_Flows_bgr() {
    return bgr_;
}
