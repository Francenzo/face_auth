#ifndef _ALGORITHM_QUICK_
#define _ALGORITHM_QUICK_

#include <stdio.h>
#include <stdlib.h>
#include <iostream> 
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>

using namespace std;
using namespace cv;

#define FACE_DETECT_XML "haarcascades/haarcascade_frontalface_alt.xml"
#define EYE_DETECT_XML "haarcascades/haarcascade_eye_tree_eyeglasses.xml"
#define EYE_SIZE 20

class Algorithm_Quick
{

public:
    Algorithm_Quick(vector<Mat> users);
    int compare(Mat image);
    double eye_face_ratio(Mat face);
    bool eye_match(Mat face);
    void comp_histogram(Mat image, Mat compare);

private:
    // Face and eye detection
    CascadeClassifier face_cascade;
    CascadeClassifier eyes_cascade;

    vector<Mat> eyes_one;
    vector<Mat> eyes_two;

};


#endif
