############################################################################
############################################################################
####	exercise2_solution.py
####
####	September 2020,  baclark@msu.edu
####	Example solution for the second exercise in bootcamp 2020
####
####	In this example, I will use python to make the plot
############################################################################


import ROOT

import matplotlib.pyplot as plt

import os

ROOT.gSystem.Load(os.environ.get('ARA_UTIL_INSTALL_DIR')+"/lib/libAraEvent.so")

# create a geomtool
geomTool = ROOT.AraGeomTool.Instance()

station = 2

# make storge containers for the x and y coordinates
x = []
y = []

# the x-y coordinates of channels 0-3 are enough for a top down view
for ant in range(3):
	ant_location = geomTool.getStationInfo(2).getAntennaInfo(ant).antLocation
	x.append(ant_location[0])
	y.append(ant_location[1])

# make a plot
fig = plt.figure(figsize=(5,5))
ax = fig.add_subplot(111)
ax.hist(list_of_snrs, bins=np.arange(0,10,0.5))
ax.set_ylabel('X [m]')
ax.set_xlabel('Y [m]')
ax.set_title('Borehole locations of A2')
fig.savefig('ant_locations_py.png', bbox_inches='tight', dpi=300)

