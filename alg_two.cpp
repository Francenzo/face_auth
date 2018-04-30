/*
 *
 *  Second Algorithm to test image
 *    against known dataset
 *
 */

#include "alg_two.hpp"

Algorithm_Two::Algorithm_Two(vector<Mat> users, vector<int> labels) 
{
    Mat testSample = users[users.size() -1];  
    int testLabel = labels[labels.size() -1];  
    users.pop_back();  
    labels.pop_back();  

    model = createFisherFaceRecognizer();
    model->train(users, labels);

    int predictedLabel = model->predict(testSample);  
    
    string result_message = format("Predicted class = %d / Actual class = %d.", predictedLabel, testLabel);  
    cout << result_message << endl;    
}

int Algorithm_Two::compare(Mat face)
{
  
    // if(images.size()<=1) {  
    //     string error_message ="This demo needs at least 2 images to work. Please add more images to your data set!";  
    //     CV_Error(CV_StsError, error_message);  
    // }  
 
    // Mat testSample = images[images.size() -1];  
    // int testLabel = labels[labels.size() -1];  
    // images.pop_back();  
    // labels.pop_back();  

    // Ptr<FaceRecognizer> model1 = createFisherFaceRecognizer();
    // model->train(images, labels);  

    // int predictedLabel = model->predict(testSample);  
  
    // string result_message = format("Predicted class = %d / Actual class = %d.", predictedLabel, testLabel);  
    // cout << result_message << endl;  
}
