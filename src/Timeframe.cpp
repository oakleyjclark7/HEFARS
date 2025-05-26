#include "Timeframe.h"
#include "Pulseframe.h"
#include "Event.h"

#include <vector>
#include <iostream>

Timeframe::Timeframe(){
    // Initialize output frame to be all zeros
    output_frame.resize(80, std::vector<unsigned short>(80,0));
}

// Reset the output frame to be all zeros
void Timeframe::ResetOutputFrame(){
    for (int i=0;i<80;i++){
        for (int j=0;j<80;j++){
            output_frame[i][j] = 0;
        }
    }
}

// Add event objects to the timeframe
void Timeframe::AddEvent(Event eventObj, double time){

    // Loop through all pixel hits in the event and add to the PulseMap
    for (int i=0;i<eventObj.GetElectrons().size();i++){
        PulseInformation PulseInfo(eventObj.GetElectrons()[i],time);
        std::pair<int, int> key = std::make_pair(eventObj.GetXpix()[i],eventObj.GetYpix()[i]);
        PulseMap[key].push_back(PulseInfo);
    }
}

// At the end of a frame, process the previous frame
// This involves drawing all the pulses for each pixel and doing the peak hold
void Timeframe::ProcessFrame(double frameStart, double frameEnd){

    // Reset the output frame to all be zeros
    ResetOutputFrame();

    // Make a Pulseframe object
    Pulseframe pf;

    // Iterate through all keys in the map
    for (auto entry : PulseMap){
        // For each pixel, reset the pulse frame
        pf.ResetPulseFrame();

        // For each pulse in the pixel - iterating backwards
        std::pair<int,int> pixel = entry.first;
        std::vector<PulseInformation> pulses = entry.second;
        for (int i=pulses.size()-1;i>=0;i--){
         
            // Draw pulse onto pulseframe
            pf.AddPulseToFrame(pulses[i], frameStart);

            // If the pulse was not in the overlap period, remove from the vector
            // Otherwise we keep the pulse there for next time
            if (pulses[i].time + SHAPING_TIME <= frameEnd){
                pulses.pop_back();
            }

        }

        // Update PulseMap to the modified vector pulses, where overlapping events are left behind
        PulseMap[pixel] = pulses;
        // Do peak hold on pixel, find greatest pulse height, store in frame
        double peak_hold = pf.GetGreatestPulseHeight();
        output_frame[pixel.first][pixel.second] = peak_hold;

    }

}
