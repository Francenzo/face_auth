/*
 *
 *  Quick algorithms used to test image
 *    against known dataset
 *
 */

#include "alg_quick.hpp"

Algorithm_Quick::Algorithm_Quick(vector<Mat> users)
{
    if( !face_cascade.load(FACE_DETECT_XML) )
    { 
        printf("--(!)Error loading\n"); 
        exit(-1);
    }
    if( !eyes_cascade.load(EYE_DETECT_XML) )
    { 
        printf("--(!)Error loading\n"); 
        exit(-1);
    }

    for (int iCount = 0; iCount < users.size(); iCount++)
    {
        eye_face_ratio(users[iCount]);
    }
}

int Algorithm_Quick::compare(Mat face)
{
    // eye_face_ratio(face);
    skin_match(face);
  return 0;
}

bool Algorithm_Quick::skin_match(Mat face)
{
    std::vector<Rect> eyes;

    //-- Detect eyes
    eyes_cascade.detectMultiScale( face, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );

    if (eyes.size() == 2)
    {
        //Conversion to HSV
        Mat img_hsv, brown, blue;

        int left = eyes[0].x + (eyes[0].width/2) - 20;
        int top = eyes[0].y + (eyes[0].height/2) - 20;
        int side = eyes[0].width/2;

        // cvtColor(face(Rect(left, top, side, side)), img_hsv, CV_BGR2HSV_FULL);

        for (int iCount = 0 ; iCount < eyes.size(); iCount ++)
        {
            // inRange(img_hsv, Scalar(112, 100, 100), Scalar(124, 255, 255), blue);
            inRange(face(eyes[0]), Scalar(10, 0, 0), Scalar(20, 255, 255), brown);

        }

    }
    else
    {
        cout << "Error: invalid eye count: " << eyes.size() << endl;
    }
    return true;
}


double Algorithm_Quick::eye_face_ratio(Mat face)
{
    std::vector<Rect> eyes;
    int ratio;

    //-- Detect eyes
    eyes_cascade.detectMultiScale( face, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );

    if (eyes.size() == 2)
    {
        double eye_ratio_one =  (double)(eyes[0].width*eyes[0].height)/(face.cols*face.rows);
        double eye_ratio_two = (double)(eyes[1].width*eyes[1].height)/(face.cols*face.rows);
        ratio = 0;
        // ratio = (double)(abs(eye_mid_one - eye_mid_two))/ (double)face.cols;
        cout << "eye0: " << eye_ratio_one << ", eye1: " << eye_ratio_two << ", face.cols: " << face.cols << ", ratio: " << ratio << endl;
    }
    else
    {
        cout << "Error: invalid eye count: " << eyes.size() << endl;
    }
    return ratio;
}

