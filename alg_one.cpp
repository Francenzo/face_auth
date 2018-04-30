
#include "alg_one.hpp"

Algorithm_One::Algorithm_One(vector<Mat> users, vector<int> labels) 
{
    // Constructor
}

void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator = ';') {
    // std::ifstream file(filename.c_str(), ifstream::in);
    // if (!file) {
    //     string error_message = "No valid input file was given, please check the given filename.";
    //     CV_Error(CV_StsBadArg, error_message);
    // }
    // string line, path, classlabel;
    // while (getline(file, line)) {
    //     stringstream liness(line);
    //     getline(liness, path, separator);
    //     getline(liness, classlabel);
    //     if(!path.empty() && !classlabel.empty()) {
    //         images.push_back(imread(path, 0));
    //         labels.push_back(atoi(classlabel.c_str()));
    //     }
    // }
}

int Algorithm_One::compare(Mat face)
{
    // read your CSV path
    return 0;
  
    string fn_csv;  
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
   
    Ptr<FaceRecognizer> model = createEigenFaceRecognizer();
    model->train(images, labels);  
    
    int predictedLabel = model->predict(testSample);  
   
    string result_message = format("Predicted class = %d / Actual class = %d.", predictedLabel, testLabel);  
    cout << result_message << endl;  
}
