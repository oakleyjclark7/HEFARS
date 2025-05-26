#pragma once

#include <vector>
#include <map>

#include "Event.h"

const int FRAME_RATE = 9084;
constexpr double FRAME_LENGTH = 1.0/FRAME_RATE;

// Stores the number of electrons for a pulse to represent and the time of the pulse
struct PulseInformation{
    int electrons;
    double time;
    PulseInformation(int e, double t) : electrons(e), time(t) {}
};

class Timeframe{

    private:

        // Map where the key is the x,y coordinate pair, and value is a vector of PulseInformation struct
        std::map<std::pair<int, int>, std::vector<PulseInformation>> PulseMap;

        // The output_frame to go onto charge sharing
        std::vector<std::vector<unsigned short>> output_frame;

        // Reset the output frame
        void ResetOutputFrame();
    
        
    public:

        // Constructor
        Timeframe();

        // Add an event object to the map
        void AddEvent(Event eventObj, double time);

        // Process frame
        void ProcessFrame(double frameStart, double frameEnd);

        // Get the output frame
        std::vector<std::vector<unsigned short>> GetOutputFrame(){return output_frame;}

};



