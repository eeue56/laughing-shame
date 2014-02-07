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

typedef struct {
    int selected;
    vector<Vec3f> circles;    
} click_event;

void on_canny_low(int pos, void *options_){
    slider_options *options = (slider_options *)options_;
    options->canny_low = pos + 1;
}

void on_canny_high(int pos, void *options_){
    slider_options *options = (slider_options *)options_;
    options->canny_high = pos + 1;
}

void on_threshold(int pos, void *options_){
    slider_options *options = (slider_options *)options_;
    options->threshold = map_range(pos, MIN_THRESHOLD, MAX_THRESHOLD, 1, 100);
    cout << options->threshold << endl;
}

void on_click(int event, int x, int y, int flags, void *options_){
    click_event *click = (click_event *)options_;
    for(size_t i = 0; i < click->circles.size(); i++) {
        // if the distance from click to center of circle is less than radius
        if(euc_dist(x, y, click->circles[i][0], click->circles[i][1]) < click->circles[i][2]) {
            click->selected = i;
        }
    }
}

void registerClick(const string window_name, click_event *options) {
    setMouseCallback(window_name, on_click, options);
}

void createOptions(const string window_name, slider_options *options){
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
    click_event click;
            
    namedWindow(window_name, CV_WINDOW_NORMAL);
    namedWindow(results_window_name, CV_WINDOW_NORMAL);
    createOptions(window_name, &options);
    
    registerClick(window_name, &click);

    Mat frame;
    Mat paused_frame;
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
        GaussianBlur(canny, canny, Size(9, 9), 2, 2);
        HoughCircles(canny, click.circles, CV_HOUGH_GRADIENT, 1,
                     canny.rows / 10, options.canny_high, options.threshold, 0, 50);
        
        for( size_t i = 0; i < click.circles.size(); i++ ) {
            
            Point center(cvRound(click.circles[i][0]), cvRound(click.circles[i][1]));
            int radius = cvRound(click.circles[i][2]);
            // circle center
            if(i == click.selected) {
                circle(frame, center, 3, Scalar(0,255,0), -1, 8, 0 );
            } else {
                circle(frame, center, 3, Scalar(255,0,0), -1, 8, 0 );
            }
            // circle outline
            circle(frame, center, radius, Scalar(0,0,255), 3, 8, 0 );
        }
        
        imshow(window_name, frame);

        imshow(results_window_name, canny);


    }

    return 0;
}

