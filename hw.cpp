#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, const char * argv[]) {
    Mat pic = imread("1.PNG");
    imshow("Pic", pic);
    
    Mat color[7];
    
    while(1) {
        int key = waitKey();
        if (key == 27)
            break;
        else if(key == '1') {
            pic = imread("1.PNG");
            imshow("Pic", pic);
        }
        else if (key == '2') {
            pic = imread("2.PNG");
            imshow("Pic", pic);
        }
        else if (key == '3') {
            pic = imread("3.PNG");
            imshow("Pic", pic);
        }
        else if (key == '4') {
            pic = imread("4.PNG");
            imshow("Pic", pic);
        }
        else if (key == '5') {
            pic = imread("5.PNG");
            imshow("Pic", pic);
        }
        else if (key == '6') {
            pic = imread("6.PNG");
            imshow("Pic", pic);
        }
        
    inRange(pic, Scalar(0,0,0), Scalar(50,50,255), color[0]);
    imshow("Red", color[0]);
    inRange(pic, Scalar(0,0,0), Scalar(0,150,255), color[1]);
    imshow("Orange", color[1]);
    inRange(pic, Scalar(0,234,0), Scalar(0,235,255), color[2]);
    imshow("Yellow", color[2]);
    inRange(pic, Scalar(0,0,0), Scalar(50,255,50), color[3]);
    imshow("Green", color[3]);
    inRange(pic, Scalar(240,150,0), Scalar(255,200,50), color[4]);
    imshow("Sky", color[4]);
    inRange(pic, Scalar(0,0,0), Scalar(255,200,0), color[5]);
    imshow("Blue", color[5]);
    inRange(pic, Scalar(244,0,100), Scalar(245,60,200), color[6]);
    imshow("Purple", color[6]);
    
    waitKey(0);
    }
    return 0;
}
