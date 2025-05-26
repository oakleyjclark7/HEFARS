#include <random>
#include <cmath>
#include <vector>

#include "Energy.h"

using namespace std;

double GetCCE(double inz){

    vector<double> z {0.00200,0.00193,0.00185,0.00177,0.00170,0.00164,0.00157,0.00151,0.00144,
    0.00139,0.00133,0.00128,0.00123,0.00118,0.00115,0.00111,0.00108,0.00105,0.00102,0.00100,0.00098,
    0.00096,0.00094,0.00092,0.00091,0.00089,0.00088,0.00086,0.00085,0.00083,0.00081,0.00080,0.00078,
    0.00077,0.00075,0.00074,0.00072,0.00071,0.00069,0.00068,0.00066,0.00065,0.00063,0.00061,0.00059,
    0.00057,0.00056,0.00000};
    

    vector<double> cce {0.77122,0.77619,0.78117,0.78614,0.79111,0.79609,0.80106,0.80603,0.81101,
    0.81598,0.82095,0.82593,0.83090,0.83587,0.84085,0.84582,0.85080,0.85577,0.86074,0.86572,0.87069,
    0.87566,0.88064,0.88561,0.89058,0.89556,0.90053,0.90550,0.91048,0.91545,0.92042,0.92540,0.93037,
    0.93534,0.94032,0.94529,0.95027,0.95524,0.96021,0.96519,0.97016,0.97513,0.98011,0.98508,0.99005,
    0.99503,1.00000,1.00000};

    // Find which point the z falls between
    int leftIndex;
    for (int i=0;i<z.size();i++){
        if (inz<z[i]){
            leftIndex = i;
        }
    }
    // Find how far through the left to the right index z is
    double percentComplete = (z[leftIndex]-inz)/(z[leftIndex]-z[leftIndex+1]);
    double interCCE = cce[leftIndex] - percentComplete*(cce[leftIndex]-cce[leftIndex+1]);

    return interCCE;
}   

double Fano(double edep,int inSeed){

    double fanoFactor = 0.09;
    double creationEnergy = 4.6;
    double N = 1000*edep/creationEnergy;
    double sigma = sqrt(fanoFactor*N);

    default_random_engine generator;
    generator.seed(inSeed);
    normal_distribution<double> distribution(N,sigma);
    double fanoN = distribution(generator);
    double fanoEnergy = fanoN*creationEnergy/1000.0;

    return fanoEnergy;
}