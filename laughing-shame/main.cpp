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


int main(int argc, char * arv4lgv[]){
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
    }

    namedWindow(window_name, CV_WINDOW_NORMAL);

    Mat frame;
    bool has_read_correctly;

    while (true){

       has_read_correctly = video.read(frame);

       if (!has_read_correctly){
           cout << "A reading error occured." << endl;
           return -1;
       }


       imshow(window_name, frame);
       waitKey(1);
    }

    return 0;
}

