#include <iostream>

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

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
    }

    namedWindow(window_name, CV_WINDOW_NORMAL);

    Mat frame;
    while (true){

       bool has_read = video.read(frame); // read a new frame from video

       if (!has_read){
           cout << "A reading error occured.";
           return -1;
       }


       imshow(window_name, frame);
       waitKey(1);
    }

    return 0;
}

