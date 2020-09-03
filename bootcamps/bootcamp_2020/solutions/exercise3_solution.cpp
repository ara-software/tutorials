////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////  plot_sim.cpp
////
////  In this example, I show how to loop over simulation outputs
////  And how to make a histogram of the neutrino interaction vertices in the xz plane
////////////////////////////////////////////////////////////////////////////////

//Includes
#include <iostream>
#include <iomanip>
#include <sstream>

//ROOT Includes
#include "TTree.h"
#include "TFile.h"
#include "TH2D.h"
#include "TStyle.h"

//AraSim includes
#include "Event.h"
#include "Report.h"
#include "IceModel.h"

int main(int argc, char **argv)
{

	if(argc<3) {
		std::cout << "Usage\n" << argv[0] << " <output_directory> <input_file_1> <input_file_2> ...\n";
		return -1;
	}

	/*
	arguments
	0: exec
	1: output directory
	2: input file 1
	3: input file 2
	4: ....
	*/

	TH2D *all_events = new TH2D("all_events_xzplane", "all_events_xzplane", 100, -6000, 6000, 100, -3000, 200);
	TH2D *trig_events = new TH2D("trig_events_xzplane", "trig_events_xzplane", 100, -6000, 6000, 100, -3000, 200);

	for(int file=2; file<argc; file++){
		TFile *fpIn = TFile::Open(argv[file],"read");
		if(!fpIn) {
			std::cout << "Can't open file\n";
			return -1;
		}
		TTree *AraTree2 = (TTree*)fpIn->Get("AraTree2");
		TTree *AraTree = (TTree*)fpIn->Get("AraTree");
		if(!AraTree2 || !AraTree){
			std::cout<< "Can't find AraTree2"<<endl;
			fpIn->Close();
			continue;
		}
		cout<<"File: "<<argv[file]<<endl;
		Event *eventPtr = 0;
		Report *reportPtr = 0;
		IceModel *icemodel = 0; 

		AraTree2->SetBranchAddress("event", &eventPtr);
		AraTree2->SetBranchAddress("report", &reportPtr);
		AraTree->SetBranchAddress("icemodel", &icemodel);
		AraTree->GetEvent(0); // because AraTree is the same for all events, it only has one entry

		int num_events = AraTree2->GetEntries();
		for(int event=0; event<num_events; event++){
			
			AraTree2->GetEvent(event);

			double posnu[3]; // vertex location
			double nnu[3]; // neutrino momentum unit vector
			double weight; // the weight of the neutrino
			int flavor; // what was the flavor?
			int current; // was it a CC or NC event?
			double energy; // what was the energy

			posnu[0] = eventPtr->Nu_Interaction[0].posnu.GetX();
			posnu[1] = eventPtr->Nu_Interaction[0].posnu.GetY();
			posnu[2] = eventPtr->Nu_Interaction[0].posnu.GetZ();
			nnu[0] = eventPtr->Nu_Interaction[0].nnu.GetX();
			nnu[1] = eventPtr->Nu_Interaction[0].nnu.GetY();
			nnu[2] = eventPtr->Nu_Interaction[0].nnu.GetZ(); 
			weight=eventPtr->Nu_Interaction[0].weight;
			flavor=eventPtr->nuflavorint;
			current=eventPtr->Nu_Interaction[0].currentint;
			energy = eventPtr->pnu;

			if(weight<0 || weight>1.) continue; // protect against any funny business in the weights

			int trigger; // did the event trigger?
			trigger=reportPtr->stations[0].Global_Pass;


			// to simplify the vector math, the core of the ARA station is usually
			// displaced to the (x,y) coordinate of (10000, 10000)
			// so to get the interactions in station centric coordinates
			// we need to subtract 
			double core_x = 10000.;

			double local_x = posnu[0]-core_x;
			double depth = (icemodel->Surface(0.,0.))-posnu[2];

			all_events->Fill(local_x, -depth, weight);
			
			if(trigger>0){
				trig_events->Fill(local_x, -depth, weight);
			}
		}
		fpIn->Close();
		delete fpIn;
	}

	gStyle->SetOptStat(0);

	TCanvas *c = new TCanvas("c","c",2.5*1150,850);
	c->Divide(2,1);
	c->cd(1);
		all_events->Draw("colz");
		all_events->SetTitle("All Events; X [m]; Z [m]; Weighted Number of Neutrino Events");
		gPad->SetRightMargin(0.15);
	c->cd(2);
		trig_events->Draw("colz");
		trig_events->SetTitle("Triggered Events; X [m]; Z [m]; Weighted Number of Neutrino Events");
		gPad->SetRightMargin(0.15);
	c->SaveAs("posnu_xz_plane.png");

}
