#!/usr/bin/python -i

##@file printEvents.py
# Print events.
#

import sys

from ROOT import TF1, TFile, TGaxis


def printEvents(fname):
    global file
    
    file = TFile.Open(fname)
    
    for event in file.decay:
        print event.eventID
        print str(event.pos) + ' mm'
        print str(event.time) + ' ns'
        print 'mu plus depos: ' + str(event.muplus_depos_energy) + ' MeV'
        print 'mu plus ind: ' + str(event.muplus_depos_ind0) + ' ' + str(event.muplus_depos_ind1) + ' ' + str(event.muplus_depos_ind2)
        print 'mu minus depos: ' + str(event.muminus_depos_energy) + ' MeV'
        print 'mu minus ind: ' + str(event.muminus_depos_ind0) + ' ' + str(event.muminus_depos_ind1) + ' ' + str(event.muminus_depos_ind2)

        print ''
        try:
            print 'mu plus skip: ' + str(event.muplus_skip_energy) + ' MeV'
            print 'mu plus ind: ' + str(event.muplus_skip_ind0) + ' ' + str(event.muplus_skip_ind1) + ' ' + str(event.muplus_skip_ind2)
            print 'mu minus skip: ' + str(event.muminus_skip_energy) + ' MeV'
            print 'mu minus ind: ' + str(event.muminus_skip_ind0) + ' ' + str(event.muminus_skip_ind1) + ' ' + str(event.muminus_skip_ind2)
        except Exception:
            print 'no cube'



if __name__=='__main__':
    fname='tree.root'
    # check for run time arguments
    if len(sys.argv)>1:
        fname=sys.argv[1]
    printEvents(fname)


print "\n to quit press Ctrl-D"
