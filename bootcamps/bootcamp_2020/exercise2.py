############################################################################
############################################################################
####	exercise2.py
####
####	September 2020,  baclark@msu.edu
####	guided python example for the second exercise in bootcamp 2020
####
############################################################################


import ROOT

import matplotlib.pyplot as plt

import os

# load the libAraEvent.so
ROOT.gSystem.Load(os.environ.get('ARA_UTIL_INSTALL_DIR')+"/lib/libAraEvent.so")

# create a geomtool
geomTool = 

# set a station number
station = 2

# make storge containers for the x and y coordinates
x = []
y = []

# now, we loop over the four strings, and to do that, it's good enough
# to loop over the top vpol antennas
for ant in range(4):
	
	# get the antenna locations, as there is only 1 top vpol per string
	ant_location = 

	# below here, you should put the x coordinate of ant_location into the x
	# vector of antenna locations, and the same for y



# make a plot
fig = plt.figure(figsize=(5,5))
ax = fig.add_subplot(111)
ax.plot(x,y,'ko')
ax.set_ylabel('X [m]')
ax.set_xlabel('Y [m]')
ax.set_title('Borehole locations of A2')
fig.savefig('ant_locations_py.png', bbox_inches='tight', dpi=300)

