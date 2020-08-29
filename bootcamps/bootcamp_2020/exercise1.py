################################################################################
################################################################################
####	exercise1.py
####
####	September 2020,  baclark@msu.edu
####	guided python example for the first exercise in bootcamp 2020
####
################################################################################

import ROOT
import numpy as np
import matplotlib.pyplot as plt
import os

# load the libAraEvent.so
ROOT.gSystem.Load(os.environ.get('ARA_UTIL_INSTALL_DIR')+"/lib/libAraEvent.so")

# open a data file
file = 

# load in the event free for this file
eventTree = 

# here's a pointer to a raw event type
rawEvent = ROOT.RawAtriStationEvent()

# now, you need to SetBranchAddress for eventTree
eventTree.

# get the number of entries in this file
num_events = 

# make a empty list where we can keep the snrs
list_of_snrs = []

# loop over the events
for event in range(num_events):
	
	# do a get entry
	eventTree.

	# check if it's a cal pulser

	isCalpulser = 

	# make a conditional statement to exit the loop if it is not a cal pulser
	if(isCalpulser==0):
		continue


	# make a *useful* event out of the *raw* event, which functionally just calibrates it
	usefulEvent = 

	# now, get the waveform from channel 0
	graph = usefulEvent.getGraphFromRFChan(0)

	# now, calculate the rms and (absolute!) peak magnitude
	# you can use some ROOT/TMath functionality if you want
	# or you can write your own code to compute the rms and peak
	# because you're in python, you might consider transferring
	# the voltages into a numpy array and using that to do the math

	rms = 
	peak = 

	# add this entry to the list
	list_of_snrs.append(peak/rms)

	# bit of cleanup
	del graph
	del usefulEvent


# and do some plotting
list_of_snrs = np.asarray(list_of_snrs)
fig = plt.figure(figsize=(8,5))
ax = fig.add_subplot(111)
ax.hist(list_of_snrs, bins=np.arange(0,10,0.5))
ax.set_ylabel('SNR')
ax.set_xlabel('Number of Events')
ax.set_title('Histogram of Ch 0 SNRs')
fig.savefig('hist_of_snr_py.png', bbox_inches='tight', dpi=300)


