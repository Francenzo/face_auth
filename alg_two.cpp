/*
 *
 *  Second Algorithm to test image
 *    against known dataset
 *
 */

#include "alg_two.hpp"

Algorithm_Two::Algorithm_Two(vector<Mat> users, vector<int> labels) 
{
    model = createFisherFaceRecognizer();
    model->train(users, labels);   
}

int Algorithm_Two::compare(Mat face)
{
  
    int predictedLabel = -1;
    double confidence = 0.0;
    model->predict(face, predictedLabel, confidence);  
    
    string result_message = format("Predicted class = %d | confidence = %f", predictedLabel, confidence);  
    cout << result_message << endl; 
}
