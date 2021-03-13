////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//// calc_aeff.cpp
////
//// Mar 2021,  baclark@msu.edu
//// compute effective area
////////////////////////////////////////////////////////////////////////////////

// C/C++ Includes
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <cmath>

//ROOT Includes
#include "TTree.h"
#include "TFile.h"
#include "TMath.h"

#include "Settings.h"
#include "Primaries.h"
#include "Event.h"
#include "Report.h"

using namespace std;

int main(int argc, char **argv){

	if(argc<3) {  // Check to make sure there are enough arguments to do something meaningful
		std::cout << "Usage:" << basename(argv[0]) << " <string> <file_1> <file_2> ... <file_n>" << std::endl;
		return -1;
	}
	// we need these three pieces of information, but we can make AraSim tell us itself
	double volradius = -10;//We're just setting them to be negative, their content will be overridden.
	double voldepth = -10;
	double energy = -10;

	// Initializing variables where we will collect statistics: number of thrown events, weights, cross section
	int totnthrown=0;
	double totweight=0.;
	double totweight_trigger=0.;
	double totweightsq_trigger;
	double typetotweight[12];
	double sigma[12];
	for(int i=0; i<12; i++){
		typetotweight[i]=0.;
		sigma[i]=0.;
	}
	double totweight_thrown = 0;

	for(int file=2; file<argc; file++){
		cout<<"On file "<<argv[file]<<endl;
		TFile *fpIn = new TFile(argv[file], "OLD"); //we're going to open the data file
		if(!fpIn){ std::cerr<< "Cannot open the old file: " <<argv[file]<<endl; return -1;}
		fpIn->cd(); //go into that file

		TTree *AraTree2 = (TTree*) fpIn->Get("AraTree2");
			if(!AraTree2){ std::cerr<<"Can't find AraTree2 in file" <<argv[file]<<endl; continue;}
		TTree *AraTree = (TTree*) fpIn->Get("AraTree");
			if(!AraTree){ std::cerr<<"Can't find AraTree in file" <<argv[file]<<endl; continue;}

		Report *reportPtr=0;//Pointer to the report branch of the files created by AraSim
		Event *eventPtr=0;//Pointer to the Event branch of the files created by AraSim
		AraTree2->SetBranchAddress("report",&reportPtr);
		AraTree2->SetBranchAddress("event",&eventPtr);
		int numentries = AraTree2->GetEntries();//Number of entries in the file

		Settings *settings=0;
		AraTree->SetBranchAddress("settings",&settings);//Pointer to the Settings branch of the files created by AraSim

		// set the radius and depth if we haven't already
		if(volradius<0){
			AraTree->GetEntry(0);
			volradius = settings->POSNU_RADIUS;//Radius in which neutrinos are generated
			voldepth = settings->PICK_ABOVE_HEIGHT;//Depth of the fiducial volume
			energy = settings->EXPONENT;
		}

		for(int event=0; event<numentries; event++){

			AraTree2->GetEntry(event);
			int iflavor = (eventPtr->nuflavorint)-1;
			int inu = eventPtr->nu_nubar;
			int icurr = eventPtr->Nu_Interaction[0].currentint;//Type of current interaction (CC or NC)
			int globalpass = reportPtr->stations[0].Global_Pass;//If the event triggered the station
			double sigm = eventPtr->Nu_Interaction[0].sigma;//CC
			double weight = eventPtr->Nu_Interaction[0].weight;//Weight

			// make sure the weights are sensible before counting the event
			if(!std::isnan(weight) && (weight <1) && (weight>0)){
			
				totweight_thrown+=weight;
				sigma[inu+2*icurr+4*iflavor] = sigm;
				typetotweight[inu+2*icurr+4*iflavor]+=weight;

				totnthrown++;

				if(globalpass>0){//If event passes
					// totweight+=weight;
					totweight_trigger+=weight;
					totweightsq_trigger+=weight*weight;
				}			
			}


		}
		fpIn->Close();
		delete fpIn;
	}

	double sigmaave = 0.0;//Average CC
	for(int iflavor=0; iflavor<3; iflavor++){//Loop over flavors and save triggered events per flavor. Can be useful if we're interested in Veff per flavor
		for(int inu=0; inu<2; inu++){
			double tempsig=0.;
			double tempweight=0.;
			for(int icurr=0; icurr<2; icurr++){
				tempsig += sigma[inu + 2*icurr + 4*iflavor];
				tempweight += typetotweight[inu + 2*icurr + 4*iflavor];
			}
			// cout<<"iflavor "<<iflavor<<"and inu "<<inu<<" is "<<tempweight<<endl;
			sigmaave+= tempsig * (tempweight/totweight_thrown);
		}
	}

	cout<<"Tot weight "<<totweight_thrown<<" vs totweight_trigger "<<totweight_trigger<<endl;

	double vtot = TMath::Pi() * double(volradius) * double(volradius) * double(voldepth); // vtot in m^3
	double veff_sr = vtot * totweight_trigger / double(totnthrown) * 4.0 * TMath::Pi(); // veff*sr now in m^3 * sr
	double veff_sr_err = sqrt(totweightsq_trigger) / totweight_trigger * veff_sr;
	double nnucleon = 5.54e29; //units here are 1/m^3
	double aeff_sr = veff_sr*nnucleon*sigmaave; // m^2 sr (m^3 sr * 1/m^3 * m^2)

	double aeff_sr_err = sqrt(totweightsq_trigger) / totweight_trigger * aeff_sr;//Aeff uncertainty 
	printf("Aeff %e [m^2 sr] and err %e [m^2 sr] \n", aeff_sr, aeff_sr_err);
	printf("Tot n thrown %d, totweight %.2f \n", totnthrown, totweight_trigger);


	char buf[100];
	sprintf(buf, "./aeff_vs_energy_%s.csv",argv[1]);//Name of the file to be produced with results
	FILE *fout = fopen(buf, "a+");
	fprintf(fout, "%.1f, %.5f, %.5f \n", energy, aeff_sr, aeff_sr_err);//convert to cm^2 and save out to file
	fclose(fout);

}
