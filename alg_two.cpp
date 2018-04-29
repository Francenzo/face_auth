/*
 *
 *  Second Algorithm to test image
 *    against known dataset
 *
 */

#include "alg_two.hpp"

Algorithm_Two::Algorithm_Two(vector<Mat> users) 
{
    // Constructor
}

int Algorithm_Two::compare(Mat face)
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
  
    // 如果没有读取到足够图片，我们也得退出.  
    if(images.size()<=1) {  
        string error_message ="This demo needs at least 2 images to work. Please add more images to your data set!";  
        CV_Error(CV_StsError, error_message);  
    }  
  
    // 得到第一张照片的高度. 在下面对图像  
    // 变形到他们原始大小时需要  
    int height = images[0].rows;  
    // 下面的几行代码仅仅是从你的数据集中移除最后一张图片  
    //[gm:自然这里需要根据自己的需要修改，他这里简化了很多问题]  
    Mat testSample = images[images.size() -1];  
    int testLabel = labels[labels.size() -1];  
    images.pop_back();  
    labels.pop_back();  
    // 下面几行创建了一个特征脸模型用于人脸识别，  
    // 通过CSV文件读取的图像和标签训练它。  
    // T这里是一个完整的PCA变换  
    //如果你只想保留10个主成分，使用如下代码  
    //      cv::createEigenFaceRecognizer(10);  
    //  
    // 如果你还希望使用置信度阈值来初始化，使用以下语句：  
    //      cv::createEigenFaceRecognizer(10, 123.0);  
    //  
    // 如果你使用所有特征并且使用一个阈值，使用以下语句：  
    //      cv::createEigenFaceRecognizer(0, 123.0);  
    //  
    Ptr<FaceRecognizer> model1 = createFisherFaceRecognizer();
    model->train(images, labels);  
    // 下面对测试图像进行预测，predictedLabel是预测标签结果  
    int predictedLabel = model->predict(testSample);  
    //  
    // 还有一种调用方式，可以获取结果同时得到阈值:  
    //      int predictedLabel = -1;  
    //      double confidence = 0.0;  
    //      model->predict(testSample, predictedLabel, confidence);  
    //  
  
    string result_message = format("Predicted class = %d / Actual class = %d.", predictedLabel, testLabel);  
    cout << result_message << endl;  
    return 0;
}
