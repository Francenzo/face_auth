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
        vector<Rect> tmpEyes;
        Mat tmpUser = users[iCount];
        eyes_cascade.detectMultiScale( tmpUser, tmpEyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );
        if (tmpEyes.size() == 2)
        {
            Mat tmp;
            resize(tmpUser(tmpEyes[0]), tmp, Size(EYE_SIZE,EYE_SIZE));
            eyes_one.push_back(tmp.clone());
            resize(tmpUser(tmpEyes[1]), tmp, Size(EYE_SIZE,EYE_SIZE));
            eyes_two.push_back(tmp.clone());
        }
        else
        {
            cout << "Error loading user " << iCount << "." << endl;
        }
        eye_face_ratio(users[iCount]);
    }
}

int Algorithm_Quick::compare(Mat face)
{
    // eye_face_ratio(face);
    // eye_match(face);
  return 0;
}

bool Algorithm_Quick::eye_match(Mat face)
{
    std::vector<Rect> eyes;

    //-- Detect eyes
    eyes_cascade.detectMultiScale( face, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );

    if (eyes.size() == 2)
    {
        //Conversion to HSV
        Mat img_hsv, brown, blue;
        Mat out;

        int left = eyes[0].x + (eyes[0].width/2) - 20;
        int top = eyes[0].y + (eyes[0].height/2) - 20;
        int side = eyes[0].width/2;

        // cvtColor(face(Rect(left, top, side, side)), img_hsv, CV_BGR2HSV_FULL);

        // for (int iCount = 0 ; iCount < eyes.size(); iCount ++)
        // {
            // inRange(img_hsv, Scalar(112, 100, 100), Scalar(124, 255, 255), blue);
            // inRange(face(eyes[0]), Scalar(10, 0, 0), Scalar(20, 255, 255), brown);
            // resize(brown, out, Size(10,10),0,0,INTER_LINEAR);
            // imshow("Secure Your Face", out);
            cout << "eyes1: " << eyes_one.size() << ", eye2: " << eyes_two.size() << endl;
            comp_histogram(eyes_one[0].clone(),face(eyes[0]).clone());
            comp_histogram(eyes_two[0].clone(),face(eyes[1]).clone());
        // }

    }
    else
    {
        cout << "Error: invalid eye count: " << eyes.size() << endl;
    }
    return true;
}

void Algorithm_Quick::comp_histogram(Mat image, Mat compare)
{
    Mat src_base, hsv_base;
    Mat src_test1, hsv_test1;
    Mat hsv_half_down;

    src_base = image;
    src_test1 = compare;
    resize(compare, src_test1, Size(EYE_SIZE,EYE_SIZE));

    /// Convert to HSV
    cvtColor( src_base, hsv_base, COLOR_BGR2HSV );
    cvtColor( src_test1, hsv_test1, COLOR_BGR2HSV );

    hsv_half_down = hsv_base( Range( hsv_base.rows/2, hsv_base.rows - 1 ), Range( 0, hsv_base.cols - 1 ) );

    /// Using 50 bins for hue and 60 for saturation
    int h_bins = 50; int s_bins = 60;
    int histSize[] = { h_bins, s_bins };

    // hue varies from 0 to 179, saturation from 0 to 255
    float h_ranges[] = { 0, 180 };
    float s_ranges[] = { 0, 256 };

    const float* ranges[] = { h_ranges, s_ranges };

    // Use the o-th and 1-st channels
    int channels[] = { 0, 1 };


    /// Histograms
    MatND hist_base;
    MatND hist_half_down;
    MatND hist_test1;

    /// Calculate the histograms for the HSV images
    calcHist( &hsv_base, 1, channels, Mat(), hist_base, 2, histSize, ranges, true, false );
    normalize( hist_base, hist_base, 0, 1, NORM_MINMAX, -1, Mat() );

    calcHist( &hsv_half_down, 1, channels, Mat(), hist_half_down, 2, histSize, ranges, true, false );
    normalize( hist_half_down, hist_half_down, 0, 1, NORM_MINMAX, -1, Mat() );

    calcHist( &hsv_test1, 1, channels, Mat(), hist_test1, 2, histSize, ranges, true, false );
    normalize( hist_test1, hist_test1, 0, 1, NORM_MINMAX, -1, Mat() );

    /// Apply the histogram comparison methods
    for( int i = 0; i < 4; i++ )
    {
        int compare_method = i;
        double base_base = compareHist( hist_base, hist_base, compare_method );
        double base_half = compareHist( hist_base, hist_half_down, compare_method );
        double base_test1 = compareHist( hist_base, hist_test1, compare_method );

        printf( " Method [%d] Perfect, Base-Half, Base-Test(1) : %f, %f, %f \n", i, base_base, base_half , base_test1 );
    }
    printf("=================\r\n\r\n");
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

