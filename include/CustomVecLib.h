#pragma once

#include <vector>

using namespace std;

vector<int> linspace(int start, int end, int N);
vector<double> linspace(double start, double end, int N);
vector<int> arange(int start, int end, int step);
vector<double> arange(double start, double end, double step);
double maxOfVector(vector<double> v);
int maxOfVector(vector<int> v);
int maxIdxOfVector(vector<double> v);
int maxIdxOfVector(vector<int> v);
double sumOfVector(vector<double> v);
int sumOfVector(vector<int> v);