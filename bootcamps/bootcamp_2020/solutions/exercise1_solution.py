############################################################################
############################################################################
#	exercise1_solution.py
#
#	September 2020,  baclark@msu.edu
#	Example solution for the first exercise in bootcamp 2020
#
#	just to mix it up, and demonstrate a more python-oriented analysis flow
#	unlike in the C++ version, I am not going to rely on any ROOT classes 
#	after I have accessed the waveforms
# 	I will also use matplotlib to make the plot
############################################################################

import ROOT
import numpy as np
import matplotlib.pyplot as plot
import os

ROOT.gSystem.Load(os.environ.get('ARA_UTIL_INSTALL_DIR')+"/lib/libAraEvent.so")

# open a data file
file = ROOT.TFile.Open("/data/wipac/ARA/2014/unblinded/L1/ARA02/0101/run002823/event002823.root")

# load in the event free for this file
eventTree = file.Get("eventTree")

# set the tree address to access our raw data type
rawEvent = ROOT.RawAtriStationEvent()
eventTree.SetBranchAddress("event",ROOT.AddressOf(rawEvent))

# get the number of entries in this file
num_events = eventTree.GetEntries()
print('total events:', num_events)

list_of_snrs = []

# loop over the events
for event in range(100):
	
	# get the event
	eventTree.GetEntry(event)

	#if not a cal pulser, continue
	if(rawEvent.isCalpulserEvent()==0): 
		continue

	# make a useful event
	usefulEvent = ROOT.UsefulAtriStationEvent(rawEvent,ROOT.AraCalType.kLatestCalib)

	graph = usefulEvent.getGraphFromElecChan(0)

	# unpack the voltages into a numpy array to make it easier to do math on
	# I'm going to do this to model the case where we want to stop using ROOT
	# after we've gotten the raw waveform

	volts = []
	for sample in range(graph.GetN()):
		volts.append(graph.GetY()[sample])
	volts = np.asarray(volts)

	# now, I can use np to compute the rms and the peak voltage

	rms = np.std(volts)
	peak = np.amax(volts)

	list_of_snrs.append(peak/rms)

	del usefulEvent

list_of_snrs = np.asarray(list_of_snrs)

# now we make a histogram
fig = plt.figure(figsize=(10,5))
ax = fig.add_subplot(111)
ax.hist(list_of_snrs, bins=20)
ax.set_ylabel('SNR')
ax.set_xlabel('Number of Events')
ax.set_title('Histogram of Ch 0 SNRs')
fig.savefig('hist_of_snr_py.png', bbox_inches='tight', dpi=300)


