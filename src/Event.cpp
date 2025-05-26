#include <vector>
#include <cmath>
#include <random>

#include "Event.h"

#include <iostream>

using namespace std;


void Event::AddNoise(int inSeed){

    // Sqrt(900) default ENC
    int sqrtENC = 26;

    // RNG
    default_random_engine generator;
    generator.seed(inSeed);
    normal_distribution<double> distribution(0.0,sqrtENC);

    double noise;

    // Loop through xpix
    for (int i=0;i<xpix.size();i++){
        noise = distribution(generator);
        electrons[i] += round(noise);
    }

}

void Event::DepositionReadout(double x, double y, double edep, double sigmaF, int inSeed){

    // Default number of electrons to sim per ball ball of charge
    // Less = better accuracy but longer computation
    // !!! WARNING: BE CAREFUL IF YOU CHANGE THIS !!!:
    // !!! IF THE ELECTRON BIN WIDTH IS NOT A MULTIPLE OF THIS !!!
    // !!! THEN THE BINNING BEHAVIOUR WILL BE STRANGE !!!
    int scalar = 10;

    // N charge balls to simulate
    int N = round((edep*1000/4.6)/scalar);

    // RNG
    default_random_engine generator;
    generator.seed(inSeed);
    normal_distribution<double> distribution_x(x,sigmaF);
    normal_distribution<double> distribution_y(y,sigmaF);

    double xsample;
    double ysample;
    int xpixball;
    int ypixball;
    int noMatch;

    // Loop N times over each charge ball
    for (int i=0;i<N;i++){
        // Generate a random x and y position
        xsample = distribution_x(generator);
        ysample = distribution_y(generator);
        // Find the x and y pix of the global x and y samples
        xpixball = GetPixelCoordinate(xsample);
        ypixball = GetPixelCoordinate(ysample);

        // Set noMatch counter to zero for this charge ball
        noMatch = 0;

        // Only count if xpix and ypix are in detector bounds
        if (xpixball>=0 && xpixball<=79 && ypixball>=0 && ypixball<=79){

            // Loop through size of x array
            for (int z=0;z<xpix.size();z++){
                // Check if xpixball, ypixball pair already exists - if so add electrons
                if (xpixball == xpix[z] && ypixball == ypix[z]){
                    electrons[z] += scalar;
                }
                // If the x,y pairings do not match - count
                else{
                    noMatch += 1;
                }
            }

            // If it is a new combination, add to the vectors
            if (noMatch == xpix.size()){
                xpix.push_back(xpixball);
                ypix.push_back(ypixball);
                electrons.push_back(scalar);
            }
        }
    }
    AddNoise(inSeed);
}

int Event::GetPixelCoordinate(double x){
    int pix;

    double pitch = 0.00025; // m
    pix = (int)floor(x/pitch);
    return pix;
}
