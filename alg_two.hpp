#ifndef _ALGORITHM_TWO_
#define _ALGORITHM_TWO_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

using namespace std;
using namespace cv;

class Algorithm_Two
{

public:
    Algorithm_Two(vector<Mat> users, vector<int> labels);
    int compare(Mat image);

private:
    Ptr<FaceRecognizer> model;
    double ACCEPTANCE_THRESHOLD = 1000.00;
};

#endif
