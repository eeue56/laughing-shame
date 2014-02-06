#include "helper_methods.h"

#include <iostream>

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#define MAX_THRESHOLD 255
#define MIN_THRESHOLD 0

using namespace std;
using namespace cv;

typedef struct {
    int canny_low;
    int canny_high;
    double threshold;
} slider_options;

void on_canny_low(int pos, void *options_){
    slider_options * options = (slider_options *)options_;
    options->canny_low = pos;
}

void on_canny_high(int pos, void *options_){
    slider_options * options = (slider_options *)options_;
    options->canny_high = pos;
}

void on_threshold(int pos, void *options_){
    slider_options * options = (slider_options *)options_;
    options->threshold = map_range(pos, MIN_THRESHOLD, MAX_THRESHOLD, 0.0, 1.0);
}

void createOptions(const string window_name, slider_options * options){
    int low = 5;
    int high = 100;
    int threshold = 5;

    createTrackbar("Canny low", window_name, &low, 255, &on_canny_low, options);
    options->canny_high = high;

    createTrackbar("Canny high", window_name, &high, 255, &on_canny_high, options);
    options->canny_low = low;

    createTrackbar("Threshold", window_name, &threshold, MAX_THRESHOLD, &on_threshold, options);
    options->threshold = threshold;
}

int main(int argc, char * argv[]){
    const string window_name = "main";
    const string results_window_name = "results";
    const int exit_key = 27;
    const int pause_key = 32;

    VideoCapture video;

    if (argc < 2){
        video = VideoCapture(0);
        cout << "Using camera.." << endl;
    }
    else {
        video = VideoCapture(argv[1]);
        cout << "Using file " << argv[1] << endl;
    }

    if (!video.isOpened()){
        cout << "Failed to open video." << endl;
        return -1;
    }


    slider_options options;

    namedWindow(window_name, CV_WINDOW_NORMAL);
    namedWindow(results_window_name, CV_WINDOW_NORMAL);
    createOptions(window_name, &options);


    Mat frame;
    Mat paused_frame;
    frame.zeros(frame.size(), 0);
    bool has_read_correctly;
    bool paused = false;

    while (true){

        int key = waitKey(25);

        if (key == exit_key){
            return 0;
        }
        else if (key == pause_key){
            paused = !paused;

            has_read_correctly = video.read(paused_frame);

            if (!has_read_correctly){
               cout << "A reading error occured." << endl;
               return -1;
            }
        }


        if(!paused){
            has_read_correctly = video.read(frame);

            if (!has_read_correctly){
               cout << "A reading error occured." << endl;
               return -1;
            }

        }
        else{
            paused_frame.copyTo(frame);
        }


        Mat canny;

        Canny(frame, canny, options.canny_low, options.canny_high);
        vector<Vec3f> circles;
        GaussianBlur(canny, canny, Size(9, 9), 2, 2);
        HoughCircles(canny, circles, CV_HOUGH_GRADIENT, 1,
                     canny.rows / 10, options.canny_high, options.canny_high / 2, 5, 50);
        
        for( size_t i = 0; i < circles.size(); i++ ) {
            Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);
            // circle center
            circle(frame, center, 3, Scalar(0,255,0), -1, 8, 0 );
            // circle outline
            circle(frame, center, radius, Scalar(0,0,255), 3, 8, 0 );
        }
        
        imshow(window_name, frame);

        imshow(results_window_name, canny);


    }

    return 0;
}

