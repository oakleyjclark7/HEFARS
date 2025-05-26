#pragma once

double GetDriftTime(double z);
double GetSigmaZero(double edep);
double rungeKutta(double s0, double tf, double edep);
double GetdSdt(double s, double z, double edep);
double GetSigmaF(double edep,double z);