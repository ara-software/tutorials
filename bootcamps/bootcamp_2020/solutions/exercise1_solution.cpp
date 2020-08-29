////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////	exercise1_solution.cpp
////
////	September 2020,  baclark@msu.edu
////	Example solution for the first exercise in bootcamp 2020
////
////	In this example, I will use ROOT to make a histogram
////	And also rely on class member functions of the TGraph to help
////	calculate the SNRs
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

	// for output, we will make a histogram
	TH1D *h = new TH1D("snrs","snrs",20,0,10);

	if(argc<2) {  // Check to make sure there are enough arguments to do something meaningful
		std::cout << "Usage requires you to provide input parameter of the form " << basename(argv[0]) << " <input data file>" << std::endl;
		return -1;
	}
	
	// open a data file, where the data file was passed as the first argument to the executable
	TFile *fpIn = new TFile(argv[1], "OLD");
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
	RawAtriStationEvent *rawEvent=0;
	eventTree->SetBranchAddress("event",&rawEvent);
	
	double numEntries = eventTree -> GetEntries(); //get the number of entries in this file

	for(int event=0; event<numEntries; event++){ //loop over those entries
		
		eventTree->GetEntry(event); //get the event

		bool isCalpulser = rawEvent->isCalpulserEvent();

		if(!isCalpulser) continue;
		
		//make a *useful* event out of the *raw* event, which functionally just calibrates it
		UsefulAtriStationEvent *usefulEvent = new UsefulAtriStationEvent(rawEvent, AraCalType::kLatestCalib);
	
		//now, we'll get the waveform from channel 0
		TGraph *waveform = usefulEvent->getGraphFromRFChan(0);

		// because it's a TGraph, we can use some TMath utilities to calculate
		// the peak and rms, and that allows to calculate the SNR

		double rms = TMath::RMS(waveform->GetN(), waveform->GetY());

		// now, before computing the peak, ge absolute values 
		for(int sample=0; sample<waveform->GetN(); sample++) waveform->GetY()[sample] = abs(waveform->GetY()[sample]);

		double peak = TMath::MaxElement(waveform->GetN(), waveform->GetY());	

		h->Fill(peak/rms);

		delete waveform;
		delete usefulEvent;
	}

	TCanvas *c = new TCanvas("c","c", 1100,850);
	h->Draw("hist");
	h->SetTitle("Histogram of Ch 0 SNRs; SNR; Number of Events");
	c->SaveAs("hist_of_snr_cpp.png");

}//close the main program
