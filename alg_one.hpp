#ifndef _ALGORITHM_ONE_
#define _ALGORITHM_ONE_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

class Algorithm_One
{

public:
    Algorithm_One(vector<Mat> users);
    int compare(Mat image);

private:
};

#endif
