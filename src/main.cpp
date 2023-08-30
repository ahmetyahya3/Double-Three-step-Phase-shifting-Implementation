#include <iostream>
#include <cmath>
#include <numbers>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

double PI = 3.14159265;

int period = 10;
double phaseShift = PI/3;

double dMapper(double value, double start, double end, double mappedStart, double mappedEnd){//it maps the segments for doubles
    return (value-start)*(mappedEnd-mappedStart)/(end-start)+mappedStart;
}
double normalizeRadian(double value){
    value = fmod(value, 2*PI);
    if(value < 0) return value+2*PI;
    return value;
}

Mat generateSinusoidalPattern(double shift){
    Mat pattern(720, 1280, CV_8U);
    for(int i = 0; i < 720; i++){
        auto p = pattern.ptr<uchar>(i);
        for(int j = 0; j < 1280; j++){
            double radianValue = dMapper((j%period), 0, period, 0, 2*PI);//current columns radian value
            p[j] = dMapper(sin(normalizeRadian(radianValue+shift)), -1, 1, 0, 255);
        }
    }
    return pattern;
}

Mat sinusoidalPattern[6];

int main(){
    for(int i = 0; i < 6; i++) sinusoidalPattern[i] = generateSinusoidalPattern(phaseShift*i);
    for(int i = 0; i < 6; i++) {
        imshow(to_string(i), sinusoidalPattern[i]);
        waitKey(0);
    }
}