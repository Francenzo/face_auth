/*
 *
 *  Face detection algorithm
 *    
 *
 */

#include "face_detect.hpp"

 Face_Detect::Face_Detect()
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
Mat Face_Detect::get_face() 
{ 
  Mat face;
  if (good_faces.size() > 0)
  {
    face = good_faces[0];
  }
  return face; 
}

int Face_Detect::get_face_count() { return face_count; }
vector<Mat> Face_Detect::get_face_arr() { return good_faces; }
vector<Rect> Face_Detect::get_rect() { return face_rects; }

/** @function detectAndDisplay changed to work with current implementation */
bool Face_Detect::has_face(Mat image)
{
  std::vector<Rect> faces;
  Mat frame_gray;
  // image = image.clone();
  face_count = 0;
  // face_rects.clear();
  vector<Rect> tmp_face_rects;
  good_faces.clear();

  cvtColor( image, frame_gray, CV_BGR2GRAY );
  equalizeHist( frame_gray, frame_gray );

  //-- Detect faces
  face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0| CV_HAAR_SCALE_IMAGE, Size(30, 30) );

  face_count = faces.size();

  for( int i = 0; i < faces.size(); i++ )
  {
    // Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
    // ellipse( image, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );

    Mat faceROI = frame_gray(faces[i]);
    std::vector<Rect> eyes;

    //-- In each face, detect eyes
    eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );

    tmp_face_rects.push_back(Rect( faces[i].x, faces[i].y, faces[i].width, faces[i].height));
    // face_rects.push_back(Rect( faces[i].x, faces[i].y, faces[i].width, faces[i].height));

    if (eyes.size() >= 1)
    {
      good_faces.push_back(image(faces[i]));
      rectangle( image, Rect( faces[i].x, faces[i].y, faces[i].width, faces[i].height), Scalar( 0, 0, 255 ), 4, 8, 0 );
    }

    // for( size_t j = 0; j < eyes.size(); j++ )
    //  {
    //    Point center( faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5 );
    //    int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
    //    circle( image, center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
    //  }
  }

  if (tmp_face_rects.size() > 0)
  {
    face_rects = tmp_face_rects;
  }

  // cout << "faces found = " << good_faces.size() << endl;

  //-- Show what you got
  // imshow( window_name, image );
  return (good_faces.size() > 0);
 }