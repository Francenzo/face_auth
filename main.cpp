#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <mutex>
#include <dirent.h>
#include <unistd.h>

#include "alg_one.hpp"
#include "alg_two.hpp"
#include "alg_three.hpp"
#include "alg_quick.hpp"
#include "face_detect.hpp"

#define THRESHOLD 30
#define MAX_USERS 10

using namespace std;
using namespace cv;

// Frame used during authentication cycle
Mat auth_frame;

Face_Detect * face_detect;
Algorithm_One * algorithm_one;
Algorithm_Two * algorithm_two;
Algorithm_Three * algorithm_three;
Algorithm_Quick * algorithm_quick;

bool keep_running = true;
bool user_authenticated = false;
bool algo_done = false;
mutex algo_mtx;  // Mutex lock for algo thread


//
//  Check if file in path exists
//
bool file_exists(string name)
{
  ifstream f(name.c_str());
  return f.good();
}

//
//  Save face to database for auth users
//
void save_face()
{
  VideoCapture camera(0);
  if (!camera.isOpened())
  {
    cout << "Error: Camera is missing." << endl;
    exit(-1);
  }

  Mat frame;
  Mat face;
  Face_Detect face_detect;

  while (true) 
  {
    camera >> frame;
    // cout << "Capturing frame..." << endl;

    if (!frame.empty() && face_detect.has_face(frame))
    {
      // cout << "Face found!." << endl;
      face = face_detect.get_face();

      if (!face.empty())
      {
        cout << "Face found!" << endl;
        break;
      }
    }

    //Wait to allow other processes to run
    imshow("Secure Your Face", frame);
  }

  int iCount = 0;
  while (true) 
  {
    stringstream ss;
    ss << "database/user_" << iCount << ".jpg";
    string dir = ss.str();

    if (!file_exists(dir))
    {
      imwrite(dir,face);
      cout << "File saved to: " << dir << endl;
      exit(0);
    } else {
      iCount++;
    }
  }
}

//
//  Authntication cycle
//
bool do_auth()
{
  Mat face;

  if (auth_frame.empty())
  {
    cout << "Error: Auth image empty!!!" << endl;
  }

  if (!auth_frame.empty() && face_detect->has_face(auth_frame))
  {
    // imshow("Secure Your Face", auth_frame);
    // cout << "Face found!" << endl;
    vector<Mat> faces = face_detect->get_face_arr();

    for (int iCount = 0; iCount < faces.size(); iCount++)
    {
      face = faces[iCount];
      algorithm_quick->compare(face);
      
      int result = 0;
      result = algorithm_one->compare(face);
      result = algorithm_two->compare(face);
      result = algorithm_three->compare(face);

      if (result > THRESHOLD)
      {
        return true;
      }
    }
  }

  return false;

}

//
//  Thread to keep running auth cycles
//
void * algo_thread_func(void *)
{
  while (keep_running)
  {
    cout << "Running!!!" << endl;
    user_authenticated = do_auth();
    // sleep(1);
    algo_done = true;
    algo_mtx.lock();
  }
}

//
// Main thread
//
int main(int argc, char* argv[])
{
  Mat frame;
  char c;
  int rc;
  pthread_t algorithm_thread;
  int face_count = 0;

  if (argc == 2)
  {
    if (strcmp(argv[1], "-s") == 0)
    {
      save_face();
    }
  }

	VideoCapture camera(0);
	if (!camera.isOpened())
	{
		cout << "Error: Camera is missing." << endl;
		exit(-1);
	}


  vector<Mat> users_one, users_two, users_three, users_quick;
  for(int iCount = 0; iCount < MAX_USERS; iCount++)
  {
    stringstream ss;
    ss << "database/user_" << iCount << ".jpg";
    string dir = ss.str();

    users_one.push_back(imread(dir, CV_LOAD_IMAGE_COLOR));
    users_two.push_back(imread(dir, CV_LOAD_IMAGE_COLOR));
    users_three.push_back(imread(dir, CV_LOAD_IMAGE_COLOR));
    users_quick.push_back(imread(dir, CV_LOAD_IMAGE_COLOR));
  }

  face_detect = new Face_Detect();
  algorithm_one = new Algorithm_One(users_one);
  algorithm_two = new Algorithm_Two(users_two);
  algorithm_three = new Algorithm_Three(users_three);
  algorithm_quick = new Algorithm_Quick(users_quick);
  pthread_create(&algorithm_thread, NULL, algo_thread_func, NULL);

  while (true) 
  {
  	camera >> frame;
  	// cout << "Capturing frame..." << endl;

    if (algo_done)
    {
      if (face_detect->get_face_count() > 0)
      {
        face_count = 25;
      }
      auth_frame = frame.clone();
      algo_mtx.unlock();
    }

    if (face_count > 0)
    {
      rectangle( frame, Rect( 0, 0, frame.cols, 30), Scalar( 0, 0, 0 ),  CV_FILLED, 8, 0 );
      putText(frame, "Analyzing face...", cvPoint(20,20), 
        FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(255,255,255), 1, CV_AA);
      face_count--;
    }

    if (user_authenticated)
    {
      cout << "Authenticated" << endl;
      break;
    }

    //Wait to allow other processes to run
    imshow("Secure Your Face", frame);
    int c = cvWaitKey(40);

    //Exit the loop if esc key
    if(27 == char(c))
    {
      keep_running = false;
      sleep(1);
      cout << "Exiting..." << endl;
      break;
    }
  }
}
