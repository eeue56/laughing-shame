#include "helper_methods.h"

#include <iostream>

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

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
    options->threshold = map_range(pos, 0, 255, 0.0, 1.0);
}

void createOptions(const string window_name, slider_options * options){
    int low = 5;
    int high = 100;
    int threshold = 5;

    createTrackbar("Canny low", window_name, &low, 255, &on_canny_low, options);

    createTrackbar("Canny high", window_name, &high, 255, &on_canny_high, options);

    createTrackbar("Threshold", window_name, &threshold, 255, &on_threshold, options);
}

int main(int argc, char * argv[]){
    const string window_name = "main";
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
    createOptions(window_name, &options);



    Mat frame;
    bool has_read_correctly;

    while (true){

       has_read_correctly = video.read(frame);

       if (!has_read_correctly){
           cout << "A reading error occured." << endl;
           return -1;
       }


       imshow(window_name, frame);
       int key = waitKey(1);

       if (key == 27){
           return 0;
       }
    }

    return 0;
}

