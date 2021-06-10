#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
#include <math.h>
#include <string>

class OpticalImage {
    public: 
        //default option
        OpticalImage();

        // output revised
        OpticalImage(const int cols_, const int rows_);

        // output revised & parameter tuning
        OpticalImage(const int cols_, const int rows_, const float pyr_scale, const int levels, const int winsize, const int iterations, \
            const int poly_n, const float poly_sigma, const int flags);

        // optical flow img update
        void Update(cv::Mat img);

        // Get optical image color version
        cv::Mat Get_Flows_bgr();
    
    private:
        cv::Mat prvs_;
        cv::Mat bgr_;
        int img_cols_;
        int img_rows_;
        bool self_init_ = true;

        //! parameter
        float pyr_scale_;
        int levels_;
        int winsize_;
        int iterations_;
        int poly_n_;
        float poly_sigma_;
        int flags_;
};