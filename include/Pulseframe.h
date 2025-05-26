#pragma once

#include <vector>
#include <cmath>

#include "Timeframe.h"
#include "CustomVecLib.h"

const double PEAKING_TIME = 0.000002; // 2us
const double SHAPING_TIME = 0.000008; // 8us
const double FRAME_RESOLUTION = 0.0000001; // 0.1us -> pulse is 100 points
const double S_PEAKING = -PEAKING_TIME*PEAKING_TIME / -13.815510558; // 013.8 = ln(0.001)
const double S_DECAY = -SHAPING_TIME*SHAPING_TIME / -13.815510558; // -13.8 = ln(0.001)

class Pulseframe{

    private:

        // The pulse height vector which stores pulses in a frame
        std::vector<double> pulse_height_vector;

        // Define the unit pulse - 2 sided gaussian
        std::vector<double> unit_pulse;

    public:
        Pulseframe();
        void AddPulseToFrame(PulseInformation PulseInfo, double frameStart);
        void ResetPulseFrame();
        double GetGreatestPulseHeight();
        void PrintPulseFrameTest();

};