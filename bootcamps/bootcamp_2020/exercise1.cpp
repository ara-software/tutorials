////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////	exercise1.cpp
////
////	September 2020,  baclark@msu.edu
////	guided cpp example for the first exercise in bootcamp 2020
////
////////////////////////////////////////////////////////////////////////////////

// C/C++ Includes
#include <iostream>

//AraRoot Includes
#include "RawAtriStationEvent.h"
#include "UsefulAtriStationEvent.h"

//ROOT Includes
#include "TTree.h"
#include "TFile.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TMath.h"

using namespace std;

int main(int argc, char **argv)
{

	// for output, we will make a 1D ROOT histogram
	TH1D *h = new TH1D("snrs","snrs",20,0,10);

	// open a datafile
	TFile *input_file = 


	// load the event tree
	TTree *eventTree = 

	
	// here's a pointer to a raw event type
	RawAtriStationEvent *rawEvent=0;


	// now, you need to SetBranchAddress for eventTree
	eventTree->


	// get the number of entries
	int numEntries =


	for(int event=0; event<numEntries; event++){ //loop over those entries

		// now, do a get entry
		eventTree->


		// check if it's a cal pulser
		bool isCalpulser =


		// make a conditional statement to exit the loop if it is not a cal pulser
		if(!isCalpulser) continue;
		

		// make a *useful* event out of the *raw* event, which functionally just calibrates it
		UsefulAtriStationEvent *usefulEvent = 
	

		// now, get the waveform from channel 0
		TGraph *waveform = 


		// now, calculate the rms and (absolute!) peak magnitude
		// you can use some ROOT/TMath functionality if you want
		// or you can write your own code to compute the rms and peak
		double rms = 
		double peak = 


		// add this entry to the histogram
		h->Fill(peak/rms);


		// now, we clean up
		delete waveform;
		delete realAtriEvPtr;
	}

	// and do some plotting
	TCanvas *c = new TCanvas("c","c", 1100,850);
	h->Draw("hist");
	h->SetTitle("Histogram of Ch 0 SNRs; SNR; Number of Events");
	c->SaveAs("hist_of_snr_cpp.png");

}//close the main program
