#!/usr/bin/python
# Program to compare the conversion probability usign different methods

import os
import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as pl

ang = -8
E = 4

logdir = 
plotdir = 

for P in np.linspace(2800,11800,10):
    logname = "%dPa%.1fkeV%.1fdeg.log"%(P,E,ang)
    command = "./bin/conversionStudy --a %f --p %f --e %f > %s"%(ang,P,E,logname)
    os.system(command)

