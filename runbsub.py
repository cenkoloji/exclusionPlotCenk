#!/usr/bin/python

#bsub -q 1nh -o /afs/cern.ch/exp/cast/mmscratch/cenk/excCodeCenk/out_0.300000 /afs/cern.ch/exp/cast/mmscratch/cenk/excCodeCenk/bin/testLike  --a 0.300000

import os
import sys

def findOutputPath(path):
    lines =  file(path,"r").readlines()
    for line in lines:
        try:
            vars = line.split()
            if vars[0]=="OUT_PATH":
                return vars[1]
        except:
            pass

bsub = 0

configFile = "configC0.01.cfg"
configFile = "configCfixed.cfg"

outputpath = findOutputPath("./inputs/" + configFile)
print outputpath

mainpath = "/afs/cern.ch/exp/cast/mmscratch/cenk/excCodeCenk/"

program = os.path.join(mainpath,"bin/testSim")
outpath = os.path.join(mainpath,outputpath,"logs/")

for mass in range(835,1115):
    ma = "%.6f"%(mass*1E-3)

    if bsub:
        command = "bsub -q 1nh -o " + outpath + ma + " " + program +"  --a " + ma + "  --c " + configFile
    else:
        command = program +"  --a " + ma + "  --c " + configFile  + " > " + outpath + ma

    print(command)
    os.system(command)

