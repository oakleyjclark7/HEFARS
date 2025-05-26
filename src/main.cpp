#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <random>
#include <string>
#include <vector>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <direct.h>

#include "Event.h"
#include "Propagation.h"
#include "Energy.h"
#include "CustomVecLib.h"
#include "Timeframe.h"
#include "Pulseframe.h"
#include "ChargeSharing.h"

using namespace std;

// Generate random intervals between events given an event rate
random_device rd;
mt19937 gen(rd());
uniform_real_distribution<double> dis(0.0,1.0);
double get_event_interval(int event_rate){

    double U = dis(gen);
    double T = -log(U)/(double)event_rate;
    return T;
}

int main(){

    // Screen output
    cout << endl << "===============================================" << endl;
    cout << "Welcome to HEFARS" << endl;
    cout << "Hexitec Electric Field And Readout Simulation" << endl;
    cout << "Author: Oakley Clark - o.clark@surrey.ac.uk" << endl;
    cout << "===============================================" << endl << endl;


    // Process the input.txt file and set user options
    string fname;
    double threshold;
    double Emin;
    double Emax;
    double energyStepKeV;
    int event_rate;

    string txtFid;
    cout << "Enter the run txt file name: " << endl;
    cin >> txtFid;
    fstream txtFile;
    txtFile.open(txtFid,fstream::in);
    string txtLine;
    while (getline(txtFile,txtLine)){
        if (txtLine.find(".csv") != string::npos){
            fname = txtLine;
        }
        if (txtLine.find("Emin")!= string::npos){
            Emin = stod(txtLine.substr(7,txtLine.length()-7));
        }
        if (txtLine.find("Emax")!= string::npos){
            Emax = stod(txtLine.substr(7,txtLine.length()-7));
        }
        if (txtLine.find("Estep")!= string::npos){
            energyStepKeV = stod(txtLine.substr(8,txtLine.length()-8));
        }
        if (txtLine.find("threshold")!= string::npos){
            threshold = stod(txtLine.substr(12,txtLine.length()-12));
        }
        if (txtLine.find("eventrate")!= string::npos){
            event_rate = stoi(txtLine.substr(12,txtLine.length()-12));
        }
    }
    int thresholdElec = round(threshold*1000.0/4.6);

    // Get the electron bin width - must be a multiple of 10
    // User value of energyStep will be adjusted slightly to account for this
    double unroundedEnergyStepElec = energyStepKeV*1000.0/4.6;
    int energyStepElec = (int)10.0*round(unroundedEnergyStepElec/10.0);

    // Initializations
    int nEvents = 1;
    int oldEventId = -1;
    int EminElec = (int)round(Emin*1000.0/4.6);
    int EmaxElec = (int)round(Emax*1000.0/4.6);

    // HISTOGRAMS
    // First make integer electron edges
    vector<int> electronEdges = arange(EminElec,EmaxElec,energyStepElec);
    // Find how many channels there will be
    int nChannels = electronEdges.size()-1;
    // Make matching energy bin edges
    vector<double> energyEdges(nChannels+1,0);
    for (int i=0;i<nChannels+1;i++){
        energyEdges[i] = electronEdges[i]*4.6/1000.0;
    }
    // Make energy centres
    vector<double> energyCentres;
    for (int i=0;i<nChannels;i++){
        energyCentres.push_back(0.5*(energyEdges[i]+energyEdges[i+1]));
    }
    // Histogram initializations
    vector<int> summedCSD(nChannels,0);
    vector< vector<vector<int>>> CSDhist(80,vector<vector<int>>(80,vector<int>(nChannels)));
    vector<int> summedCSA(nChannels,0);
    vector< vector<vector<int>>> CSAhist(80,vector<vector<int>>(80,vector<int>(nChannels)));
    vector<int> summedNONE(nChannels,0);
    vector< vector<vector<int>>> NONEhist(80,vector<vector<int>>(80,vector<int>(nChannels)));

    // Other variable declarations
    double x;
    double y;
    double z;
    double E;
    double sigmaF;
    int CSxpix;
    int CSypix;
    int CSelectrons;
    int idx;
    int CSD_binning_errors;
    int CSA_binning_errors;
    int NONE_binning_errors;

    // Progress
    double percentComplete;
    time_t startTime;
    time_t nowTime;
    int secondsGone;
    int estRuntimeLeft;

    // RNG seed initialization
    time_t seed = time(0);

    // Create output directory if it doesn't already exist
    string outputDir = "Output";
    if (_mkdir("Output") == 0){
        cout << endl << "Output Directory Created" << endl;
    }

    // Print user options to screen
    cout << "User Options: " << endl;
    cout << "======================" << endl;
    cout << setw(12) << "Threshold" << setw(7) << threshold << endl;
    cout << setw(12) << "Emin" << setw(7) << Emin << endl;
    cout << setw(12) << "Emax" << setw(7) << Emax << endl;
    cout << setw(12) << "Estep " << setw(7) << energyStepKeV << endl;
    cout << setw(12) << "Eventrate" << setw(12) << event_rate << endl << endl;

    cout << "===========================================================" << endl;
    cout << "Estep was adjusted so that the electron bin edges" << endl;
    cout << "are a multiple of scalar, s = 10" << endl;
    cout << "Chosen from file = " << energyStepKeV << "keV, adjusted to " << energyStepElec*4.6/1000.0 << "keV" << endl;
    cout << "===========================================================" << endl << endl;


    // Open the File - Count the rows and then reset the file to the start
    cout << "Loading file: " << fname << endl << endl;
    fstream myfile;
    myfile.open("Data/" + fname,fstream::in);
    string line;
    int nRows=0;
    while (getline(myfile, line)){
        nRows++;
    }
    cout << "Processing " << nRows << " rows of file " << fname << endl << endl;
    myfile.clear();
    myfile.seekg (0, ios::beg);

    // Skip first 9 lines
    int skipRows = 0;
    while (skipRows < 9){
        getline(myfile,line);
        skipRows++;
    }  

    cout << "skipped first 9 rows" << endl;

    // Frame stuff
    double sim_time = 0.0; // The global simulation time
    int frame_num = 0; // The global frame number

    // Initially we set the frame to be frame 0. Frame 0 goes from -98uS to 2uS
    // But we set the time to 0s.
    // Therefore frame_start_time should be -98us
    double frame_start_time = (frame_num-1)*FRAME_LENGTH + PEAKING_TIME;

    // Create eventObj for the first event
    Event* eventObj = new Event;

    // Create timeframeObj
    Timeframe tf;

    startTime = time(NULL);
    // Iterate over the file
    for (int fileRows = 9; fileRows<nRows; fileRows++){        

        // Get the current row
        getline(myfile,line);
        stringstream myString(line);
        string element;
        vector<string> row;
        while(getline(myString,element,',')){
            row.push_back(element);
        }

        // Calc progress and print
        if (fileRows%(nRows/10)==0 && int(fileRows/(nRows/10)!=0)){
            percentComplete = 100.0*fileRows/nRows;
            nowTime = time(NULL);
            secondsGone = nowTime-startTime;
            estRuntimeLeft = ceil(((100/percentComplete)*secondsGone)-secondsGone);
            cout << setw(9) << "Runtime: " << setw(4) << secondsGone << setw(12) <<"s, Est left: " << setw(3) << floor(estRuntimeLeft/60) << setw(5) <<" mins " << setw(3) <<floor(estRuntimeLeft%60) << setw(2) <<" s " << endl;
        }

        // Increment random seed
        seed++;

        // If it is a new event (but not first event)
        if (oldEventId != stoi(row[0]) && oldEventId != -1){

            // Get the random time interval from the last event
            // and therefore update the sim time
            double time_interval = get_event_interval(event_rate);
            sim_time += time_interval;

            // If it is the same frame
            if (sim_time < frame_start_time + FRAME_LENGTH){
                // Add event to tf
                tf.AddEvent(*eventObj, sim_time);

            }            
            // If it is a different frame
            else{

                // Process tf
                cout << "###############################" << endl;
                cout << "PROCESSING FRAME: " << frame_num << endl;
                cout << "###############################" << endl;
                tf.ProcessFrame(frame_start_time, frame_start_time + FRAME_LENGTH);

                // Now do charge sharing on the frame 
                ChargeSharing* frameObj = new ChargeSharing(tf.GetOutputFrame());
                frameObj->correctFrame(thresholdElec);

                // Reset binning error counters
                CSD_binning_errors = 0;
                CSA_binning_errors = 0;
                NONE_binning_errors = 0;

                // Update CSD histogram - NEW
                for (int i=0;i<frameObj->GetCSD_xpix().size();i++){
                    CSxpix = frameObj->GetCSD_xpix()[i];
                    CSypix = frameObj->GetCSD_ypix()[i];
                    CSelectrons = frameObj->GetCSD_Val()[i];
                    idx = ChargeSharing::GetBinIndex(CSelectrons,electronEdges);
                    if (idx != -1){
                        CSDhist[CSxpix][CSypix][idx] += 1;
                        summedCSD[idx] += 1;
                    }
                    else{
                        CSD_binning_errors++;
                    }
                }
                cout << "CSD: " << frameObj->GetCSD_xpix().size() - CSD_binning_errors << "events binned, "
                << CSD_binning_errors << " binning errors" << endl;

                // Update CSA histogram
                for (int i=0;i<frameObj->GetCSA_xpix().size();i++){
                    CSxpix = frameObj->GetCSA_xpix()[i];
                    CSypix = frameObj->GetCSA_ypix()[i];
                    CSelectrons = frameObj->GetCSA_Val()[i];
                    idx = ChargeSharing::GetBinIndex(CSelectrons,electronEdges);
                    if (idx != -1){
                        CSAhist[CSxpix][CSypix][idx] += 1;
                        summedCSA[idx] += 1;
                    }
                    else{
                        CSA_binning_errors++;
                    }
                }
                cout << "CSA: " << frameObj->GetCSA_xpix().size() - CSA_binning_errors << "events binned, "
                << CSA_binning_errors << " binning errors" << endl;

                // Update NONE histogram
                for (int i=0; i<frameObj->GetNONE_xpix().size();i++){
                    CSxpix = frameObj->GetNONE_xpix()[i];
                    CSypix = frameObj->GetNONE_ypix()[i];
                    CSelectrons = frameObj->GetNONE_Val()[i];
                    idx = ChargeSharing::GetBinIndex(CSelectrons,electronEdges);
                    if (idx != -1){
                        NONEhist[CSxpix][CSypix][idx] += 1;
                        summedNONE[idx] += 1;
                    }
                    else{
                        NONE_binning_errors++;
                    }
                }
                cout << "NONE: " << frameObj->GetNONE_xpix().size() - NONE_binning_errors << "events binned, "
                << NONE_binning_errors << " binning errors" << endl;

                // Recalculate current frame and frame start
                // Can't just add frane length in case a whole frame is skipped
                frame_num = int((sim_time - PEAKING_TIME) / FRAME_LENGTH) + 1;
                frame_start_time = (frame_num-1)*FRAME_LENGTH + PEAKING_TIME; 

                // Now add event to tf
                tf.AddEvent(*eventObj, sim_time);                                                                                                                                                                                                                                                                                            

            }

            // Delete the event object
            delete eventObj;

            // Create a new event object
            eventObj = new Event;

            // Reinitialize row parameters
            x = stod(row[1]);
            y = stod(row[2]);
            z = stod(row[3]);
            E = stod(row[4]);

            // DO FANO sigmaF on row
            E = Fano(E,seed);
            sigmaF = GetSigmaF(E,z);

            // Do DepositionReadout() on current row, updating eventObj parameters
            eventObj->Event::DepositionReadout(x,y,E,sigmaF,seed);

            // Update eventID
            oldEventId = stoi(row[0]);

        }
        // If it another line from the same event or first line from first event
        else{
            //cout << "Another line of same event (or first line first event)" << endl;
            // Update event ID
            oldEventId = stoi(row[0]);

            // Increment random seed
            seed++;

            // Get row parameters
            x = stod(row[1]);
            y = stod(row[2]);
            z = stod(row[3]);
            E = stod(row[4]);

            // DO FANO, sigmaF on row
            E = Fano(E,seed);
            sigmaF = GetSigmaF(E,z);

            // Do DepositionReadout() on current row, updating eventObj parameters
            eventObj->Event::DepositionReadout(x,y,E,sigmaF,seed);

        }

    }

    nowTime = time(NULL);
    secondsGone = nowTime-startTime;
    cout << endl << nRows << " rows processed in " << floor(secondsGone/60) <<" mins " << floor(secondsGone%60) << " s " << endl;


    // Write vectors to file
    ofstream  CSDfout;
    ofstream CSAfout;
    ofstream NONEfout;

    string event_rate_string = to_string(event_rate);

    cout << endl << "SummedCSD written to " + outputDir + "/CSD_" + fname.substr(0,fname.size()-4) + "_ER=" + event_rate_string + ".txt" << endl;
    cout << "SummedCSA written to " + outputDir + "/CSA_" + fname.substr(0,fname.size()-4) + "_ER=" + event_rate_string + ".txt" << endl;
    cout << "SummedNONE written to " + outputDir + "/NONE_" + fname.substr(0,fname.size()-4) + "_ER=" + event_rate_string + ".txt" << endl;
    cout << endl << setw(10) << "Energy (keV)" << setw(10) << "Counts" << endl;
    cout << "========================" << endl;
    for (int i=0;i<5;i++){
        if (i<4){
            cout << setw(10) << energyCentres[i] << setw(10) << summedCSD[i] << endl;
        }
        else{
            cout << setw(10) << "..." << setw(10) << "..." << endl;
        }
    }
    cout << setw(10) << energyCentres[nChannels-1] << setw(10) << summedCSD[nChannels-1] << endl;

    CSDfout.open(outputDir + "/CSD_" + fname.substr(0,fname.size()-4) + "_ER=" + event_rate_string + ".txt");      
    for (int i=0;i<nChannels;i++){
        CSDfout << setw(10) << energyCentres[i] << setw(10) << summedCSD[i] << endl;
    }
    CSAfout.open(outputDir + "/CSA_" + fname.substr(0,fname.size()-4) + "_ER=" + event_rate_string + ".txt");      
    for (int i=0;i<nChannels;i++){
        CSAfout << setw(10) << energyCentres[i] << setw(10) << summedCSA[i] << endl;
    }
    NONEfout.open(outputDir + "/NONE_" + fname.substr(0,fname.size()-4) + "_ER=" + event_rate_string + ".txt");      
    for (int i=0;i<nChannels;i++){
        NONEfout << setw(10) << energyCentres[i] << setw(10) << summedNONE[i] << endl;
    }

    return 0;
}