#ifndef _ALGORITHM_ONE_
#define _ALGORITHM_ONE_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <iostream>

using namespace std;
using namespace cv;

class Algorithm_One
{

public:
    Algorithm_One(vector<Mat> users, vector<int> labels);
    int compare(Mat image);

private:
};

#endif
