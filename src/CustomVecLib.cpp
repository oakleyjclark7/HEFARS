#include <vector>

#include "CustomVecLib.h"

using namespace std;

vector<int> linspace(int start, int end, int N){
    vector<int> vec;
    int element = start;
    int step = (end-start)/(N-1);
    for (int i=0;i<N;i++){
        vec.push_back(element);
        element = element+step;
    }
    return vec;
}

vector<double> linspace(double start, double end, int N){
    vector<double> vec;
    double element = start;
    double step = (end-start)/(N-1);
    for (int i=0;i<N;i++){
        vec.push_back(element);
        element = element+step;
    }
    return vec;
}

vector<int> arange(int start, int end, int step){
    vector<int> vec;
    int nextVal = start;
    while (nextVal<=end){
        vec.push_back(nextVal);
        nextVal += step;
    }
    return vec;
}

vector<double> arange(double start, double end, double step){
    vector<double> vec;
    double nextVal = start;
    while (nextVal<=end){
        vec.push_back(nextVal);
        nextVal += step;
    }
    return vec;
}

int maxIdxOfVector(vector<double> v){
    int maxIdx = 0;
    double maxVal = v[0];
    // Loop through rest of elements
    for (int i=1;i<v.size();i++){
        if (v[i]>maxVal){
            maxIdx = i;
            maxVal = v[i];
        }
    }
    return maxIdx;
}

int maxIdxOfVector(vector<int> v){
    int maxIdx = 0;
    int maxVal = v[0];
    // Loop through rest of elements
    for (int i=1;i<v.size();i++){
        if (v[i]>maxVal){
            maxIdx = i;
            maxVal = v[i];
        }
    }
    return maxIdx;
}

double maxOfVector(vector<double> v){
    double maxVal = v[0];
    // Loop through rest of elements
    for (int i=1;i<v.size();i++){
        if (v[i]>maxVal){
            maxVal = v[i];
        }
    }
    return maxVal;
}

int maxOfVector(vector<int> v){
    int maxVal = v[0];
    // Loop through rest of elements
    for (int i=1;i<v.size();i++){
        if (v[i]>maxVal){
            maxVal = v[i];
        }
    }
    return maxVal;
}

double sumOfVector(vector<double> v){
    double sum=0.0;
    for (int i=0;i<v.size();i++){
        sum += v[i];
    }
    return sum;
}

int sumOfVector(vector<int> v){
    int sum=0;
    for (int i=0;i<v.size();i++){
        sum += v[i];
    }
    return sum;
}
