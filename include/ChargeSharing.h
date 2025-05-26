#include <vector>
#include <iostream>
#include <string>

#pragma once

using namespace std;

class ChargeSharing{

    private:
        // Private attributes
        int threshold;
        unsigned int frameOccupancy = 0;
        vector<vector<unsigned short>> frame;


        // These vectors are added to every time a hit is accepted
        // and then can be used outside the class for efficient binning
        vector<int> CSD_xpix;
        vector<int> CSD_ypix;
        vector<unsigned short> CSD_val;
        vector<int> CSA_xpix;
        vector<int> CSA_ypix;
        vector<unsigned short> CSA_val;
        vector<int> NONE_xpix;
        vector<int> NONE_ypix;
        vector<unsigned short> NONE_val;

        // Private methods
        void CSD();
        void CSA();
        void NONE();


    public:
        // Constructor
        ChargeSharing(vector<vector<unsigned short>> inFrame);
        
        // Public member function
        void correctFrame(int inThreshold);

        // Get methods
        vector<int> GetCSD_xpix(){return CSD_xpix;}
        vector<int> GetCSD_ypix(){return CSD_ypix;}
        vector<unsigned short> GetCSD_Val(){return CSD_val;}
        vector<int> GetCSA_xpix(){return CSA_xpix;}
        vector<int> GetCSA_ypix(){return CSA_ypix;}
        vector<unsigned short> GetCSA_Val(){return CSA_val;}
        vector<int> GetNONE_xpix(){return NONE_xpix;}
        vector<int> GetNONE_ypix(){return NONE_ypix;}
        vector<unsigned short> GetNONE_Val(){return NONE_val;}
        unsigned int GetFrameOccupancy(){return frameOccupancy;}

        // Static method for finding bin index
        static int GetBinIndex(int electrons, std::vector<int> electronEdges);

};