#include "Pulseframe.h"
#include "CustomVecLib.h"

#include <cmath>
#include <iostream>

Pulseframe::Pulseframe(){
    
    // Define the pulse height vector
    int num_elements = static_cast<int>(FRAME_LENGTH / FRAME_RESOLUTION) + 1;
    for (int i = 0; i < num_elements; ++i) {
        pulse_height_vector.push_back(0.0);
    }

    // Define the pulse shape with a height of 1
    double unit_pulse_height;
    double t = 0.0;
    while (t <= PEAKING_TIME + SHAPING_TIME){

        // front side of gaussian
        if (t<PEAKING_TIME){
            unit_pulse_height = exp(-((t-PEAKING_TIME)*(t-PEAKING_TIME))/(2*S_PEAKING));
            unit_pulse.push_back(unit_pulse_height);
        }

        // tail side of gaussian
        else{
            unit_pulse_height = exp(-((t-PEAKING_TIME)*(t-PEAKING_TIME))/(2*S_DECAY));
            unit_pulse.push_back(unit_pulse_height);
        }

        t += FRAME_RESOLUTION;
    }
}

// Get the greatest pulse height from the frame
double Pulseframe::GetGreatestPulseHeight(){
    double greatest = maxOfVector(pulse_height_vector);
    return greatest;
}

// Reset the pulse frame to zeros
void Pulseframe::ResetPulseFrame(){
    pulse_height_vector.assign(pulse_height_vector.size(),0.0);
}

// Draw a pulse onto the pulse height vector
void Pulseframe::AddPulseToFrame(PulseInformation PulseInfo, double frameStart){

    // Find the initial pulse_index - frame index where peaking begins
    int pulse_index = static_cast<int>((PulseInfo.time - PEAKING_TIME - frameStart)/FRAME_RESOLUTION);

    // Add pulse to pulse height vectors
    for (int i=0;i<unit_pulse.size();i++){
        // Only add to this frame, for overlapping events ignore the bit in other frames
        if (pulse_index+i >=0 && pulse_index+i < pulse_height_vector.size()){
            pulse_height_vector[pulse_index+i] += PulseInfo.electrons * unit_pulse[i];
        }
    }

}