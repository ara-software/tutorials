////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////	exercise2.cpp
////
////	September 2020,  baclark@msu.edu
////	guided cpp example for the second exercise in bootcamp 2020
////
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

	// set a station number
	int station = 2;

	// create a geomtool
	AraGeomTool *geomTool = 

	// here are some containers for the x and y coordiantes of the strings
	std::vector<double> x;
	std::vector<double> y;

	// now, we loop over the four strings, and to do that, it's good enough
	// to loop over the top vpol antennas
	for(int i=0; i<4; i++){
		
		// get the antenna locations, as there is only 1 top vpol per string
		Double_t *ant_location = 
		
		// below here, you should put the x coordinate of ant_location into the x
		// vector of antenna locations, and the same for y

	}

	// and now make a plot
	TGraph *g = new TGraph(4, &x[0], &y[0]);

	TCanvas *c = new TCanvas("c","c", 1100,850);
	g->Draw("AP");
	g->SetMarkerStyle(8);
	g->SetMarkerSize(2);
	g->SetTitle("Borehole locations of A2; X [m]; Y [m]");
	c->SaveAs("ant_locations_cpp.png");

}//close the main program
