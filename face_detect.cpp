/*
 *
 *  Face detection algorithm
 *    
 *
 */

#include "face_detect.hpp"

 // Face and eye detection
 CascadeClassifier face_cascade;
 CascadeClassifier eyes_cascade;

 void init_detect()
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
 }

/**
  * Looks for face. If none exist, returns null
  */
Mat get_face(Mat image)
{
  Mat face;
	// INSERT ALGORITHM HERE
  return face;
}

/** @function detectAndDisplay changed to work with current implementation */
bool has_face(Mat image)
{
  std::vector<Rect> faces;
  std::vector<Rect> good_faces;
  Mat frame_gray;
  // image = image.clone();

  cvtColor( image, frame_gray, CV_BGR2GRAY );
  equalizeHist( frame_gray, frame_gray );

  //-- Detect faces
  face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0| CV_HAAR_SCALE_IMAGE, Size(30, 30) );

  for( int i = 0; i < faces.size(); i++ )
  {
    // Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
    // ellipse( image, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );

    Mat faceROI = frame_gray(faces[i]);
    std::vector<Rect> eyes;

    //-- In each face, detect eyes
    eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );

    if (eyes.size() != 2)
    {
        good_faces.push_back(faces[i]);
    }

    // for( size_t j = 0; j < eyes.size(); j++ )
    //  {
    //    Point center( faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5 );
    //    int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
    //    circle( image, center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
    //  }
  }

  // cout << "faces found = " << good_faces.size() << endl;

  //-- Show what you got
  // imshow( window_name, image );
  return (faces.size() > 0);
 }