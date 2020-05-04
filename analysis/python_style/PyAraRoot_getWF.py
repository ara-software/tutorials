"""
An example of how to retreive ARA's data using pyROOT. Thanks to
Cosmin and Kaeli for helping me to set this up.
"""
from ROOT import TCanvas, TGraph
from ROOT import gROOT
import ROOT
import os
#################################################################################################################
#  NOTE THE ORDER MAY BE IMPORTANT HERE
#  At least in some cases, there is a conflict between the ROOT GUI event loop and
#  the matplotlib event loop. So if you want to make matplotlib plots, you may have to import
#  matplotlib after ROOT.  You can of course make ROOT plots instead (and in that case, you don't need to import matplotlib)
#################################################################################################################
import matplotlib.pyplot as plt
import numpy as np
ROOT.gSystem.Load("/users/PAS0654/osu8354/ARA_cvmfs/build/lib/libAraEvent.so")#point this to your AraRoot lib

# try:
test = ROOT.TFile.Open("/fs/project/PAS0654/ARA_DATA/A23/100pct/RawData/A2/2013/run2674/event2674.root")#directory where the files are
# if(test.IsOpen()):
#     print('made it')
# else:
#     return -1
calibrator = ROOT.AraEventCalibrator.Instance()
eventTree = test.Get("eventTree")
rawEvent = ROOT.RawAtriStationEvent()
eventTree.SetBranchAddress("event",ROOT.AddressOf(rawEvent))
totalEvents = eventTree.GetEntries()
print('total events:', totalEvents)
isTrue=False

for i in range(0,10):#loop over events
    if( isTrue):
        break
    eventTree.GetEntry(i)
    if(rawEvent.isCalpulserEvent()==0): #if not a cal pulser
        continue
    usefulEvent = ROOT.UsefulAtriStationEvent(rawEvent,ROOT.AraCalType.kNoCalib)#get useful event
    gr1 = usefulEvent.getGraphFromElecChan(0)#print waveform
    isTrue=True
print("Two methods to draw:")
print("ROOT's cannonical drawer:")

gr1.GetXaxis().SetTitle("Time [ns]")
gr1.GetYaxis().SetTitle("Voltage [mV]")
gr1.SetTitle("An example of a waveform with ROOT")
gr1.Draw()

print("Or translate to python objects and then plot using matplotlib")
t = []
v = []
for i in range(0,gr1.GetN()):
    t.append(gr1.GetX()[i])
    v.append(gr1.GetY()[i])

plt.plot(t,v,linewidth=0.5)
plt.title("An example of a waveform with Python")
plt.xlabel("Time [ns]")
plt.ylabel("Voltage [mV]")
plt.show()
