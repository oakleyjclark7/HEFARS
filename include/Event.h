#pragma once

#pragma once

class Event{

    private:

        // frame vectors
        std::vector<int> xpix;
        std::vector<int> ypix;
        std::vector<int> electrons;
    
    public:

        // Get Methods
        std::vector<int> GetXpix(){return xpix;}
        std::vector<int> GetYpix(){return ypix;}
        std::vector<int> GetElectrons(){return electrons;}

        // Other methods
        void AddNoise(int inSeed);
        void DepositionReadout(double x, double y, double edep, double sigmaF, int inSeed);

        // Static methods
        static int GetPixelCoordinate(double x);
};