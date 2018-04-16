#ifndef _FACE_DETECTION_
#define _FACE_DETECTION_

#include <stdio.h>
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


class Face_Detect
{

public:
    Face_Detect();
    Mat get_face();
    int face_count;
    int get_face_count();
    vector<Mat> get_face_arr();
    vector<Rect> get_rect(); // hehe get rekt
    bool has_face(Mat image);

private:
    // Face and eye detection
    CascadeClassifier face_cascade;
    CascadeClassifier eyes_cascade;

    vector<Mat> good_faces;
    vector<Rect> face_rects;

};

#endif
