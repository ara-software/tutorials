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
#import matplotlib
#matplotlib.use( 'tkagg' )
import numpy as np
ROOT.gSystem.Load("/scratch/brianclark/test_cvmfs/build_cvmfs/ara_build/lib/libAraEvent.so")#point this to your AraRoot lib

# try:
test = ROOT.TFile.Open("/data/wipac/ARA/2013/filtered/burnSample1in10/ARA02/root/run1558/event1558.root")#directory where the files are
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
c = ROOT.TCanvas()
c.cd()
gr1.Draw()
c.Print("root_waveform.png")

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
plt.savefig('python_waveform.png')
