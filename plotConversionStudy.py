#!/usr/bin/python
# Program to compare the conversion probability usign different methods

import os
import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as pl

ang = -6
E = 4

basedir = "/afs/cern.ch/exp/cast/mmscratch/cenk/excCodeCenk/"

logdir = os.path.join(basedir,"outputs/conversionStudy/logs/")
plotdir = os.path.join(basedir,"outputs/conversionStudy/plots/")

for P in np.linspace(2800,11800,10):
    logname = os.path.join(logdir,"%dPa%.1fkeV%.1fdeg.log"%(P,E,ang))
    command = basedir + "./bin/conversionStudy --a %f --p %f --e %f > %s"%(ang,P,E,logname)
    if not os.path.exists(logname):
        os.system(command)

    fig = pl.figure()
    ax= pl.subplot(211)
    data = np.loadtxt(logname,skiprows=11,dtype=[('ma', 'f'),('profile', 'f'),('length', 'f')])

    ma= data["ma"]
    new = data["profile"]
    old = data["length"]

    minval = min(new.min(),old.min())

    pl.plot(ma,new,'o',mfc='none',mec="blue",label = "through integration")
    pl.plot(ma,old,'*',mfc='none',mec="red",label = "fixed density/length")
    ax.set_yscale("log")
    #ax.set_xlabel("Axion mass(eV)")
    ax.set_ylabel("Conversion Probability")
    ax.grid()
    ax.tick_params(labelbottom="off")
    pl.title("P=%.2fmbar, E=%.1fkeV, Ang=%.1fdeg.log"%(P/100.,E,ang))
    pl.legend(loc=0,numpoints=1)


    ax= pl.subplot(212)
    #ax.set_yscale("log")
    ax.set_xlabel("Axion mass(eV)")
    ax.set_ylabel("integration - fixed")
    pl.plot(ma,(new-old),'o',mfc='none',mec="blue",label = "new - old")

    plotname = os.path.join(plotdir,"%dPa%.1fkeV%.1fdeg.png"%(P,E,ang))
    pl.savefig(plotname)

    pl.show()
