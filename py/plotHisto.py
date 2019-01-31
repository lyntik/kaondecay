#!/usr/bin/python


##@file plotHisto.py
# Plot histogramms.
#

import sys

sys.path.append("/opt/root/lib")


from ROOT import TF1, TFile, TGaxis
from ROOT import TCanvas, TF1, TFile, TGaxis
from ROOT import gROOT, gStyle, gApplication
from ROOT import kRed, kTRUE

# set some global style options
gROOT.SetStyle('Plain')
gStyle.SetOptFit(1)
TGaxis.SetMaxDigits(3)


def plotPos(fname):
    global file, c1, c2
    
    file = TFile.Open(fname)
    
    c1 = TCanvas('c1','Decay Position', 250, 20, 700, 500)
    file.decayPos.Draw()    
    
    c2 = TCanvas('c2','Decay Position', 250, 20, 700, 500)
    file.decayTime.Draw()

if __name__=='__main__':
    fname='histo.root'
    # check for run time arguments
    if len(sys.argv)>1:
        fname=sys.argv[1]
    plotPos(fname)
    
    gApplication.Run()


print "\n to quit press Ctrl-D"
