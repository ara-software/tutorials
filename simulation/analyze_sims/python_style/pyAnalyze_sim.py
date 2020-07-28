"""
#####pyAnalize_sim.py#####

Get waveform from AraSim events with Python
Author: Jorge Torres
Date: Jul 28, 2020.
"""
from ROOT import TCanvas, TGraph
from ROOT import gROOT
import ROOT
import os
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

from ROOT import gInterpreter, gSystem
#add headers from AraSim. Not sure if all of them are needed, and I'm lazy to check that. MAK SURE to change the location of the headers
gInterpreter.ProcessLine('#include "/users/PCON0003/cond0068/ARA/AraSim/Position.h"')
gInterpreter.ProcessLine('#include "/users/PCON0003/cond0068/ARA/AraSim/Report.h"')
gInterpreter.ProcessLine('#include "/users/PCON0003/cond0068/ARA/AraSim/Detector.h"')
gInterpreter.ProcessLine('#include "/users/PCON0003/cond0068/ARA/AraSim/Settings.h"')

gSystem.Load('libAra.so') #load the simulation event library. You might get an error asking for the eventSim dictionry. To solve that, go to where you compiled AraSim, find that file, and copy it to where you set LD_LIBRARY_PATH.

test = ROOT.TFile.Open("/fs/scratch/PAS0654/jorge/sim_results/default/AraOut.default_A2_c1_E610.txt.run9.root")#directory where the simulation files are

reportPtr = ROOT.Report()#report pointer
eventTree = test.Get("eventTree")#eventTree, from AraSim output files
SimTree = test.Get("AraTree2") #AraTree2, from AraSim output files
rawEvent = ROOT.UsefulAtriStationEvent()
eventTree.SetBranchAddress("UsefulAtriStationEvent",ROOT.AddressOf(rawEvent))
SimTree.SetBranchAddress("report",ROOT.AddressOf(reportPtr))

totalEvents = eventTree.GetEntries()
print('total events:', totalEvents)
isTrue=False

for i in range(0,1000):#loop over events
    if(isTrue):
        break
    eventTree.GetEntry(i)
    SimTree.GetEntry(i)

    if(reportPtr.stations[0].Global_Pass <= 0):#making sure that the event did trigger, otherwise there won't be a waveform (this might not be needed if all waveforms are saved)
        continue
    gr1 = rawEvent.getGraphFromElecChan(0)#print waveform
    isTrue=True

t = []
v = []
for i in range(0,gr1.GetN()):
    t.append(gr1.GetX()[i])
    v.append(gr1.GetY()[i])

plt.plot(t,v,linewidth=0.5)
plt.title("An example of a triggered simulated event with Python")
plt.xlabel("Time [ns]")
plt.ylabel("Voltage [mV]")
plt.show()
