#!/usr/bin/python

#bsub -q 1nh -o /afs/cern.ch/exp/cast/mmscratch/cenk/excCodeCenk/out_0.300000 /afs/cern.ch/exp/cast/mmscratch/cenk/excCodeCenk/bin/testLike  --a 0.300000

import os
import sys
import numpy as np

def findOutputPath(path):
    lines =  file(path,"r").readlines()
    for line in lines:
        try:
            vars = line.split()
            if vars[0]=="OUT_PATH":
                return vars[1]
        except:
            pass

mass_file = "/afs/cern.ch/exp/cast/mmscratch/cenk/excCodeCenk/inputs/mass_range.txt"
m0,m1 = map(float,file(mass_file,"r").read().split())
mass_array = np.arange(m0-0.01,m1+0.01,0.001)

print " Exiting, all sims done!"
bsub = 0

configFile = "config1.cfg"
configFile = "configCfixed.cfg"
configFile = "configC0.01.cfg"

outputpath = findOutputPath("./inputs/" + configFile)
print outputpath

mainpath = "/afs/cern.ch/exp/cast/mmscratch/cenk/excCodeCenk/"

program = os.path.join(mainpath,"bin/testSim") +" --i sim3 "
outpath = os.path.join(mainpath,outputpath,"logs/")

for mass in mass_array:
    ma = "%.6f"%(mass)

    if bsub:
        command = "bsub -q 8nh -o " + outpath + ma + " " + program +"  --a " + ma + "  --c " + configFile
    else:
        command = program +"  --a " + ma + "  --c " + configFile  + " > " + outpath + ma

    print(command)
    #os.system(command)

