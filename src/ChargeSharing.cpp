#include <iostream>
#include <vector>
#include <string>

#include "ChargeSharing.h"

using namespace std;

// Constructor
ChargeSharing::ChargeSharing(vector<vector<unsigned short>> inFrame){
    frame = inFrame;
}

// Correct the frames using both charge sharing correction methods
void ChargeSharing::correctFrame(int inThreshold){
        threshold = inThreshold;
        CSD();
        CSA();
        NONE();
}

void ChargeSharing::NONE(){

    for (int i=0;i<80;i++){
        for (int j=0;j<80;j++){
            NONE_xpix.push_back(i);
            NONE_ypix.push_back(j);
            NONE_val.push_back(frame[i][j]);
        }
    }

}

void ChargeSharing::CSD(){
    
    // Create a dummy frame 2 bigger in every dimension - zeros
    vector<vector<unsigned short>> bigFrame(82,vector<unsigned short>(82,0));
    for (int i=1;i<81;i++){
        for (int j=1;j<81;j++){
            bigFrame[i][j] = frame[i-1][j-1];
        }
    }

    // `Loop over centrals of big frame
    for (int i=1;i<81;i++){
        for (int j=1;j<81;j++){
            // For values above threshold
            if (bigFrame[i][j]>threshold){
                frameOccupancy++;
                // ant neighbours above threshold
                int neighbours = 0;
                for (int m=i-1;m<=i+1;m++){
                    for (int n=j-1;n<=j+1;n++){
                        if (bigFrame[m][n]>threshold){
                            neighbours += 1;
                        }
                    }
                }
                // Only count isolated events
                if (neighbours == 1){
                    CSD_xpix.push_back(i-1);
                    CSD_ypix.push_back(j-1);
                    CSD_val.push_back(frame[i-1][j-1]);
                }
            }
        }
    }
}

void ChargeSharing::CSA(){

    // Create a dummy frame 2 bigger in every dimension - zeros
    vector<vector<unsigned short>> bigFrame(82,vector<unsigned short>(82,0));
    for (int i=1;i<81;i++){
        for (int j=1;j<81;j++){
            bigFrame[i][j] = frame[i-1][j-1];
        }
    }  

    // Keeping track
    unsigned int sumVal;
    unsigned int greatestVal;
    int gx;
    int gy;

    // Loop through each element - count number of neighbours above threshold
    for (int i=1;i<81;i++){
        for (int j=1;j<81;j++){

            // Reset the sum of values to zero for start of each pixel
            sumVal = 0;
            greatestVal = 0;

            // For values above threshold
            if (bigFrame[i][j]>threshold){
                frameOccupancy++;
                // count sum of vals above threshold + track greatest pix
                for (int m=i-1;m<=i+1;m++){
                    for (int n=j-1;n<=j+1;n++){
                        if (bigFrame[m][n]>threshold){
                            sumVal += bigFrame[m][n];
                            if (bigFrame[m][n]>greatestVal){
                                greatestVal = bigFrame[m][n];
                                gx = m-1;
                                gy = n-1;                 
                            }
                            // Rest frame value to zero so can't be counted twice
                            bigFrame[m][n] = 0;
                        }
                    }
                }
                
                CSA_xpix.push_back(gx);
                CSA_ypix.push_back(gy);
                CSA_val.push_back(sumVal);
            }
        }
    }
}

int ChargeSharing::GetBinIndex(int electrons, vector<int> electronEdges) {

    for (int i = 0; i < electronEdges.size(); i++) {
        if (electronEdges[i] >= electrons) {
            return i; 
        }
    }
    // If electrons is outside the range of electronEdges
    return -1;
}
