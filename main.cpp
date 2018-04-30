#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <mutex>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include "alg_one.hpp"
#include "alg_two.hpp"
#include "alg_three.hpp"
#include "alg_quick.hpp"
#include "face_detect.hpp"

#define THRESHOLD 30
#define MAX_USERS 10
#define MAX_TEST_IMAGES 20000

using namespace std;
using namespace cv;

// If TRUE then testing
bool TEST_MODE = false;

Size IMAGE_SIZE(100,100);

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

bool directory_exists( const char* pzPath )
{
    if ( pzPath == NULL) return false;

    DIR *pDir;
    bool bExists = false;

    pDir = opendir (pzPath);

    if (pDir != NULL)
    {
        bExists = true;    
        (void) closedir (pDir);
    }

    return bExists;
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
  vector<Mat> user;
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
        user.push_back(face);
        if (user.size() >= 2)
        {
          break;
        }
      }
    }

    //Wait to allow other processes to run
    // imshow("Secure Your Face", frame);
  }

  int iCount = 0;
  while (true) 
  {
    stringstream ss;
    ss << "database/user_" << iCount << "_one.jpg";
    string dir = ss.str();

    stringstream ss2;
    ss2 << "database/user_" << iCount << "_two.jpg";
    string dir2 = ss2.str();

    if (!file_exists(dir))
    {
      imwrite(dir,user[0]);
      imwrite(dir2,user[1]);
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
      cvtColor(face, face, CV_BGR2GRAY);
      resize(face, face, IMAGE_SIZE, 0, 0, CV_INTER_AREA);
      int rc = algorithm_quick->compare(face);
      if (rc >= 0)
      {
        return true;
      }
      else
      {
        // cout << "rc = " << rc << endl;
      }
      
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
    if(do_auth())
    {
      user_authenticated = true;
    } 
    // sleep(1);
    algo_done = true;
    algo_mtx.lock();
  }
}

void test_algorithms()
{
  int test_count = 0;
  int used_count = 0;
  int accepted_count =0;

  srand(time(NULL));

  cout << "Running algorithm against test data set." << endl;

  vector<Mat> users_one, users_two, users_three, users_quick;
  vector<int> labels;

  int rand_num = rand() % 2000; 

  for(int iCount = 0; iCount < MAX_USERS; iCount++)
  {
    // Need users with at least two images
    bool user_found = false;
    while (!user_found)
    {
      stringstream test_ss;
      test_ss << "test_faces/test_1_" <<  rand_num << ".jpg";
      string test_dir = test_ss.str();

      stringstream test_ss_2;
      test_ss_2 << "test_faces/test_2_" <<  rand_num << ".jpg";
      string test_dir_2 = test_ss_2.str();

      // cout << "running..." << test_dir << " | " << test_dir_2 << endl;

      if (file_exists(test_dir) && file_exists(test_dir_2))
      {
        user_found = true;
        cout << "Found user..." << endl;
      }
      else
      {
        rand_num++;
      }
    }

    for (int jCount = 1; jCount < 6; jCount++)
    {
      stringstream ss;
      ss << "test_faces/test_" << jCount << "_" <<  rand_num << ".jpg";
      string dir = ss.str();
      if (file_exists(dir))
      {
        cout << "Using: " << dir << endl;
        Mat tmpMat = imread(dir, CV_LOAD_IMAGE_GRAYSCALE);
        resize(tmpMat, tmpMat, IMAGE_SIZE, 0, 0, CV_INTER_AREA);

        users_one.push_back(tmpMat);
        users_two.push_back(tmpMat);
        users_three.push_back(tmpMat);
        
        labels.push_back(iCount + 1);
      }
    }
  }

  // cout << "Users2 size = " << users_two.size() << endl;

  face_detect = new Face_Detect();
  algorithm_one = new Algorithm_One(users_one, labels);
  algorithm_two = new Algorithm_Two(users_two, labels);
  algorithm_three = new Algorithm_Three(users_three);
  algorithm_quick = new Algorithm_Quick(users_quick);

  while (test_count < MAX_TEST_IMAGES)
  {
    stringstream ss;
    ss << "test_faces/test_1_" << test_count << ".jpg";
    string test_file_path = ss.str();
    if (file_exists(test_file_path))
    {
      auth_frame = imread(test_file_path, CV_LOAD_IMAGE_COLOR);
      if (do_auth())
      {
        accepted_count++;
      }
      if (face_detect->get_face_count() > 0)
      {
        used_count++;
      }
    }
    else
    {
      cout << "End of test..." << endl;
      cout << "Data set size: " << test_count << endl;
      cout << "Used: " << used_count << endl;
      cout << "Accepted Count: " << accepted_count << endl;
      exit(0);
    }

    test_count++;
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
  vector<Rect> face_rects;

  if (argc == 2)
  {
    if (strcmp(argv[1], "-s") == 0)
    {
      save_face();
    }
    if (strcmp(argv[1], "-t") == 0)
    {
      test_algorithms();
      // test_algorithms_two();
    }

  }

	VideoCapture camera(0);
	if (!camera.isOpened())
	{
		cout << "Error: Camera is missing." << endl;
		exit(-1);
	}

  vector<Mat> users_one, users_two, users_three, users_quick;
  vector<int> labels;
  for(int iCount = 0; iCount < MAX_USERS; iCount++)
  {
    stringstream ss;
    ss << "database/user_" << iCount << "_one.jpg";
    string dir = ss.str();

    stringstream ss2;
    ss2 << "database/user_" << iCount << "_two.jpg";
    string dir2 = ss.str();

    if (file_exists(dir))
    {

      Mat tmpMat = imread(dir, CV_LOAD_IMAGE_GRAYSCALE);
      resize(tmpMat, tmpMat, IMAGE_SIZE, 0, 0, CV_INTER_AREA);

      Mat tmpMat2 = imread(dir2, CV_LOAD_IMAGE_GRAYSCALE);
      resize(tmpMat2, tmpMat2, IMAGE_SIZE, 0, 0, CV_INTER_AREA);


      users_one.push_back(tmpMat.clone());
      users_two.push_back(tmpMat.clone());
      users_three.push_back(tmpMat.clone());

      users_one.push_back(tmpMat2.clone());
      users_two.push_back(tmpMat2.clone());
      users_three.push_back(tmpMat2.clone());

      users_quick.push_back(imread(dir, CV_LOAD_IMAGE_COLOR));

      labels.push_back(iCount);
      labels.push_back(iCount);

    }
  }

  face_detect = new Face_Detect();
  algorithm_one = new Algorithm_One(users_one, labels);
  algorithm_two = new Algorithm_Two(users_two, labels);
  algorithm_three = new Algorithm_Three(users_three);
  algorithm_quick = new Algorithm_Quick(users_quick);
  pthread_create(&algorithm_thread, NULL, algo_thread_func, NULL);

  while (true) 
  {
  	camera >> frame;

    if (frame.empty())
      continue;
  	// cout << "Capturing frame..." << endl;

    if (algo_done)
    {
      if (face_detect->get_face_count() > 0)
      {
        face_rects = face_detect->get_rect();
        face_count = 25;
      }
      auth_frame = frame.clone();
      algo_mtx.unlock();
    }

    if (face_count > 0)
    {
      if (face_rects.size() > 0)
      {
        for (int iCount = 0; iCount < face_rects.size(); iCount++)
        {
          rectangle(frame, face_rects[iCount], Scalar(0,0,255), 4,8,0);
        }
      }
      if (user_authenticated)
      {
        rectangle( frame, Rect( 0, 0, frame.cols, 30), Scalar( 0, 255, 0 ),  CV_FILLED, 8, 0 );
        putText(frame, "Authenticated!!", cvPoint(20,20), 
          FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(255,255,255), 1, CV_AA);
      }
      else
      {

        rectangle( frame, Rect( 0, 0, frame.cols, 30), Scalar( 0, 0, 0 ),  CV_FILLED, 8, 0 );
        putText(frame, "Analyzing face...", cvPoint(20,20), 
          FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(255,255,255), 1, CV_AA);
      }
      face_count--;
    } else {
      user_authenticated = false;
      face_rects.clear();
    }

    // if (user_authenticated)
    // {
    //   cout << "Authenticated" << endl;
    //   rectangle( frame, Rect( 0, 0, frame.cols, 30), Scalar( 0, 255, 0 ),  CV_FILLED, 8, 0 );
    //   putText(frame, "Authenticated!!", cvPoint(20,20), 
    //     FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(255,255,255), 1, CV_AA);
    //   if (face_rects.size() > 0)
    //   {
    //     for (int iCount = 0; iCount < face_rects.size(); iCount++)
    //     {
    //       rectangle(frame, face_rects[iCount], Scalar(0,0,255), 4,8,0);
    //     }
    //   }
    //   user_authenticated = false;
    //   imshow("Secure Your Face", frame);
    //   cvWaitKey(40);
    //   sleep(5);
    // }

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
