/*
 *
 *  First Algorithm to test image
 *    against known dataset
 *
 */

#include "alg_one.hpp"

Algorithm_One::Algorithm_One(vector<Mat> users) 
{
    // Constructor
}

int Algorithm_One::compare(Mat face)
{
    // INSERT ALGORITHM HERE
    if (argc <2)   
　　{  
        cout <<"usage: "<< argv[0]<<" <csv.ext> <output_folder> "<< endl;  
        exit(1);  
  
    }  
  
    string output_folder;  
  
    if (argc ==3)   
　　{  
  
        output_folder = string(argv[2]);  
  
    }  
    // read your CSV path
  
    string fn_csv = string(argv[1]);  
    // Use two containers to save images and correspongding labels 
    vector<Mat> images;  
    vector<int> labels;  
    // read the data, if the file is invalid, output error
    // the file you enter already exist
    try {  
  
        read_csv(fn_csv, images, labels);  
  
    } catch (cv::Exception& e) {  
        cerr <<"Error opening file \""<< fn_csv <<"\". Reason: "<< e.msg << endl;  
        // the file has problem, just exit
        exit(1);  
    }  
  
    if(images.size()<=1) {  
        string error_message ="This demo needs at least 2 images to work. Please add more images to your data set!";  
        CV_Error(CV_StsError, error_message);  
    }  
  
    int height = images[0].rows;  
   
    Mat testSample = images[images.size() -1];  
    int testLabel = labels[labels.size() -1];  
    images.pop_back();  
    labels.pop_back();  
    
    Ptr<FaceRecognizer> model1 = createEigenFaceRecognizer();
    model->train(images, labels);  

    int predictedLabel = model->predict(testSample);  
   
  
    string result_message = format("Predicted class = %d / Actual class = %d.", predictedLabel, testLabel);  
    cout << result_message << endl;  
    return 0;
}
