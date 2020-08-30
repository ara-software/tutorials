////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////	exercise2_solution.cpp
////
////	September 2020,  baclark@msu.edu
////	Example solution for the second exercise in bootcamp 2020
////
////	In this example, I will use ROOT to make the plot
////////////////////////////////////////////////////////////////////////////////

// C/C++ Includes
#include <iostream>

//AraRoot Includes
#include "AraGeomTool.h"

//ROOT Includes
#include "TGraph.h"
#include "TCanvas.h"

using namespace std;

int main(int argc, char **argv)
{

	if(argc<2) {  // Check to make sure there are enough arguments to do something meaningful
		std::cout << "Usage requires you to provide input parameter of the form " << basename(argv[0]) << " <station>" << std::endl;
		return -1;
	}

	int station = atoi(argv[1]); // get the station number

	AraGeomTool *geomTool = AraGeomTool::Instance();

	// the x-y coordinates of channels 0-3 are enough for a top down view
	std::vector<double> x;
	std::vector<double> y;

	for(int i=0; i<4; i++){
		Double_t *ant_location = geomTool->getStationInfo(station)->getAntennaInfo(i)->antLocation;
		x.push_back(ant_location[0]);
		y.push_back(ant_location[1]);
	}

	TGraph *g = new TGraph(4, &x[0], &y[0]);

	TCanvas *c = new TCanvas("c","c", 1100,850);
	g->Draw("AP");
	g->SetMarkerStyle(8);
	g->SetMarkerSize(2);
	g->SetTitle("Borehole locations of A2; X [m]; Y [m]");
	c->SaveAs("ant_locations_cpp.png");

}//close the main program
