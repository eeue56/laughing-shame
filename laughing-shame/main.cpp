#include <iostream>

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int main(int argc, char * argv[]){
    bool uses_camera = false;
    const string window_name = "main";

    if (argc < 2){
        cout << "Using camera.." << endl;
        uses_camera = true;
    }
    else {

    }

    namedWindow(window_name, CV_WINDOW_NORMAL);
}

