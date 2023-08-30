#include <iostream>
#include <cmath>
#include <numbers>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

double PI = 3.14159265;

int period = 1280;
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
    Mat pattern(720, 1280, CV_32F);
    for(int i = 0; i < 720; i++){
        auto p = pattern.ptr<float>(i);
        for(int j = 0; j < 1280; j++){
            double radianValue = dMapper((j%period), 0, period, 0, 2*PI);//current columns radian value
            p[j] = 1+cos(normalizeRadian(radianValue+shift));
            // p[j] = dMapper(sin(normalizeRadian(radianValue+shift)), -1, 1, 0, 255); //for visualization, mapping values 1, -1 to 0, 255
        }
    }
    return pattern;
}

Mat arctan(Mat value){//basicly applies arctan function on every element and returns new Mat object
    int width = value.rows, height = value.cols;
    Mat ret = value;
    for(int i = 0; i < width; i++){
        auto p = ret.ptr<float>(i);
        for(int j = 0; j < height; j++){
            p[j] = atan(p[j]);
        }
    }
    return ret;
}

Mat sinusoidalPattern[6];

int main(){
    //generate sinusoidal patterns 60 degrees shifted
    for(int i = 0; i < 6; i++) sinusoidalPattern[i] = generateSinusoidalPattern(phaseShift*i);
    // for(int i = 0; i < 6; i++) {
    //     imshow(to_string(i), sinusoidalPattern[i]);
    //     waitKey(0);
    // }
    
    //first phase map
    Mat firstPhaseMap = arctan(sqrt(3)*(sinusoidalPattern[4]-sinusoidalPattern[2])/(2*sinusoidalPattern[0]-(sinusoidalPattern[4]+sinusoidalPattern[2])));
    imshow("firstPhase", firstPhaseMap);
    waitKey(0);

    //second phase map
    Mat secondPhaseMap = arctan(sqrt(3)*(sinusoidalPattern[5]-sinusoidalPattern[3])/(2*sinusoidalPattern[1]-(sinusoidalPattern[5]+sinusoidalPattern[3])));
    imshow("secondPhase", secondPhaseMap);
    waitKey(0);

    //averaged phase map
    Mat averagedPhaseMap = (firstPhaseMap+secondPhaseMap)/2;
    imshow("averagedPhaseMap", averagedPhaseMap);
    waitKey(0);

}