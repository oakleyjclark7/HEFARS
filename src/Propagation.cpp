#define _USE_MATH_DEFINES
#include <cmath>

#include "Propagation.h"

double GetDriftTime(double z){
    
    double bias = 1000;
    double thickness = 0.002; // m
    double distance = thickness - z; // m
    double mobility = 0.1; // m^2 / V.s
    double time = (thickness*distance)/(mobility*bias);

    return time;
}

double GetSigmaZero(double edep){

    // Polynomial fitted paramaeters based off paper
    // Koch-Merin et al 'Soectroscopic monte-carlo model to sim...'
    double a = 1.7582417*pow(10,-10);
    double b = 2.24175824*pow(10,-8);
    double c = -8.54148011*pow(10,-22);

    double sigmaZero = a*pow(edep,2) + b*edep + c;
    return sigmaZero;
}

double GetdSdt(double s, double z, double edep){

    double mobility = 0.138;
    double N = 1000*edep/4.6;
    double kb = 1.38064852*pow(10,-23); // SI units
    double T = 298.0; // K
    double epsilon = 11 * 8.854187*pow(10,-12); // SI units
    double q = 1.6*pow(10,-19); // C
    double k1 = 2.0*mobility*kb*T/q;
    double k2 = (mobility*N*q)/(12*pow(M_PI,1.5)*epsilon);
    double val = k1 + k2/(sqrt(s+z));
    return val;
}

double rungeKutta(double s0, double tf, double edep)
{
    // Number of steps in integration
    // lower n = less accuracy = less computation time
    int n = 25;

    // Calculate step size
    double h = tf/n;

    double k1, k2, k3, k4;
 
    // Iterate for number of iterations
    double t0 = 0.0;
    double s = s0;
    double z = s0;
    for (int i=0; i<n; i++)
    {
        // Apply Runge Kutta Formulas to find
        // next value of s
        k1 = h*GetdSdt(s,z,edep);
        k2 = h*GetdSdt(s + 0.5*k1, z, edep);
        k3 = h*GetdSdt(s + 0.5*k2,z,edep);
        k4 = h*GetdSdt(s + k3,z,edep);
 
        // Update next value of s
        s = s + (1.0/6.0)*(k1 + 2*k2 + 2*k3 + k4);;
 
        // Update next value of t
        t0 = t0 + h;
    }
 
    return s;
}

double GetSigmaF(double edep, double z){

    double driftTime = GetDriftTime(z);
    double sigmaI = GetSigmaZero(edep);
    double sigmaDsq = rungeKutta(sigmaI*sigmaI,driftTime,edep);
    double sigmaF = sqrt(sigmaI*sigmaI + sigmaDsq);
    return sigmaF;
}

