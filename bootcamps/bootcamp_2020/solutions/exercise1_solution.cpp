////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////	exercise1_solution.cxx
////
////	September 2020,  baclark@msu.edu
////	Example solution for the first exercise in bootcamp 2020
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

using namespace std;

int main(int argc, char **argv)
{

	// for output, we will make a histogram
	TH1D *h = new TH1D("snrs","snrs",20,0,20);

	if(argc<2) {  // Check to make sure there are enough arguments to do something meaningful
		std::cout << "Usage requires you to provide input parameter of the form " << basename(argv[0]) << " <input data file>" << std::endl;
		return -1;
	}
	
	TFile *fpIn = new TFile(argv[1], "OLD"); //we're going to open the data file
	if(!fpIn){
		std::cerr<< "Can not open the old file: " <<argv[1]<<endl;
		return -1;
	} //throw a warning if you can't open it
	
	fpIn->cd(); //go into that file
	TTree *eventTree = (TTree*) fpIn->Get("eventTree"); //load in the event free for this file
	if(!eventTree){
		std::cerr<<"Can't find eventTree in file" <<argv[1]<<" with filename " <<argv[1]<<endl;
		return -1;
	} //throw a warning if you can't open it
	
	//set the tree address to access our raw data type
	RawAtriStationEvent *rawAtriEvPtr=0;
	eventTree->SetBranchAddress("event",&rawAtriEvPtr);
	
	double numEntries = eventTree -> GetEntries(); //get the number of entries in this file

	for(int event=0; event<numEntries; event++){ //loop over those entries
		
		eventTree->GetEntry(event); //get the event

		bool isCalpulser = rawAtriEvPtr->isCalpulserEvent();

		if(!isCalpulser) continue;
		
		//make a *useful* event out of the *raw* event, which functionally just calibrates it
		UsefulAtriStationEvent *realAtriEvPtr = new UsefulAtriStationEvent(rawAtriEvPtr, AraCalType::kLatestCalib);
	
		//now, we'll get the waveform from channel 2
		TGraph *waveform = realAtriEvPtr->getGraphFromRFChan(0);

		// because it's a TGraph, we can use the TGraph class, and some TMath tools
		// to do basic RMS and peak amplitude detection
		// and with the peak and the rms, we can calculate the SNR

		double rms = waveform->GetRMS(2);
		double peak = TMath::MaxElement(waveform->GetN(), waveform->GetY());

		h->Fill(peak/rms);

		delete waveform;
		delete realAtriEvPtr;
	}

	TCanvas *c = new TCanvas("c","c", 1100,850);
	h->Draw("hist")
	h->SetTitle("Histogram of SNRs; SNR; Number of Events");
	c->SaveAs("hist_of_snr.png");

}//close the main program