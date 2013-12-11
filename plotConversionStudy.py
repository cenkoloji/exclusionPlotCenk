#!/usr/bin/python
# Program to compare the conversion probability and expected number of photons usign different methods

import os
import numpy as np

plot = 1

if plot:
    import matplotlib as mpl
    import matplotlib.pyplot as pl

#{{{ Constants

ang = -8
E = 3

#if true, run ngamma program instead of conversion probability 
ngamma = 0

basedir = "/afs/cern.ch/exp/cast/mmscratch/cenk/excCodeCenk/"

logdir = os.path.join(basedir,"outputs/conversionStudy/logs/")
plotdir = os.path.join(basedir,"outputs/conversionStudy/plots/")

#}}}

#Going over each pressure
for P in np.arange(3000,13000,2000):

    #Chosing between the conversion probability/ ngamma {{{
    if ngamma:
        logname = os.path.join(logdir,"ngamma%dPa%.1fdeg.log"%(P,ang))
        command = basedir + "./bin/conversionStudy --a %f --p %f --e %f --ngamma > %s"%(ang,P,E,logname)
    else:
        logname = os.path.join(logdir,"%dPa%.1fkeV%.1fdeg.log"%(P,E,ang))
        command = basedir + "./bin/conversionStudy --a %f --p %f --e %f > %s"%(ang,P,E,logname)
    print command
    #}}}

    #if not os.path.exists(logname):
    #    os.system(command)

    if not plot:
        os.system(command)
        continue

    #Checking how many lines to skip in the input file #{{{
    string = ""
    skip = 1
    f = file(logname,"r")
    while not string.startswith("DATA"):
        axion_mass = string
        string = f.readline()
        skip+=1
    axion_mass = float(axion_mass)
    #}}}

    #Reading the field names #{{{
    fields = f.readline().split()
    dtype={'names': (fields),'formats': ['f']*len(fields)}
    data = np.loadtxt(logname,skiprows=skip,dtype=dtype)
    ma= data["ma"]

    #Removing ma from fields
    fields = fields[1:]
    #}}}

    markers = ["o","*","s","x","s"]
    colors = ["black","red","blue","purple","pink"]

    fig = pl.figure(figsize=(10,6))

    #First plot: conv prob or ngamma {{{
    ax= pl.subplot(211)

    #Writing labels. If the label is 0, it means fixed length calculation
    for i in range(0,len(fields)):
        if fields[i] == "0":
            label = "fixed length/density"
        else:
            label = "step=" + fields[i] + "m"
        pl.plot(ma,data[fields[i]],markers[i],mfc='none',mec=colors[i],markersize=10,label = label)

    # If you want to plot difference in the same plot
    #for i in range(0,len(fields)-1):
    #    if fields[i] == "0":
    #        label = "fixed length - " + fields[-1]
    #    else:
    #        label = fields[i] + " - " + fields[-1]
    #    pl.plot(ma,np.abs(data[fields[i]] - data[fields[-1]]),markers[i],mfc='none',mec=colors[i],markersize=8,label = label)

    ax.set_yscale("log")
    #ax.set_xlabel("Axion mass(eV)")

    if ngamma:
        ax.set_ylabel("Ngamma(30 minutes exposure on 14.5cm2 surface)")
        pl.title("P=%.2fmbar, E=2-7keV, Ang=%.1fdeg.log"%(P/100.,ang))
    else:
        ax.set_ylabel("Conversion Probability")
        pl.title("P=%.2fmbar, E=%.1fkeV, Ang=%.1fdeg.log"%(P/100.,E,ang))

    ax.grid()
    ax.tick_params(labelbottom="off")
    leg = pl.legend(loc='best', fancybox=True,numpoints=1)
    leg.get_frame().set_alpha(0.5)
    #ax.vlines(axion_mass,ax.get_ylim()[0],ax.get_ylim()[1],color = "grey")
    #}}}

    #Second plot: Difference of methods: {{{
    ax= pl.subplot(212)
    #ax.set_yscale("log")
    ax.set_xlabel("Axion mass(eV)")
    #ax.set_ylabel("integration - fixed (% diff)")
    ax.grid()
    ax.set_ylabel("% Difference with " + fields[-1])

    for i in range(0,len(fields)-1):
        if fields[i] == "0":
            label = "fixed length - " + fields[-1]
        else:
            label = fields[i] + " - " + fields[-1]
        diff = (data[fields[i]] - data[fields[-1]]) / data[fields[-1]] * 100
        pl.plot(ma,diff,markers[i],mfc='none',mec=colors[i],markersize=10,label = label)

    if ngamma:
        plotname = os.path.join(plotdir,"ngamma%dPa%.1fdeg.png"%(P,ang))
    else:
        plotname = os.path.join(plotdir,"conversion%dPa%.1fkeV%.1fdeg.png"%(P,E,ang))
    #pl.legend(loc=0,numpoints=1)

    #}}}

    #pl.savefig(plotname,bbox_inches="tight")
    pl.show()

    # Figure to plot effect of using different step sizes in one plot {{{
    '''
    fig2 = pl.figure(2)

    tocompare = 0.0
    if tocompare == 0:
        label = "fixed length"
    else:
        label = "stepsize=" + str(tocompare) + "m"

    f = map(float,fields)
    index0 = f.index(tocompare)
    print index0
    f.pop(index0)
    for ar in data:
        arr = list(ar)
        print arr
        arr.pop(0)
        print arr
        val = arr[index0]
        print val
        arr.pop(index0)

        #Plot only for the ma=mgamma
        if np.abs(ar[0]-axion_mass)<0.0005:
            diff = (arr-val)*100/val
            pl.plot(f,diff,"o-",label=".%.2fmbar"%(P/100))

    if ngamma:
        ax.set_ylabel("Ngamma(30 minutes exposure on 14.5cm2 surface)")
        pl.title("2-7keV, Ang=%.1fdeg"%(ang))
    else:
        ax.set_ylabel("Conversion Probability")
        pl.title("E=%.1fkeV, Ang=%.1fdeg"%(E,ang))

    ax = pl.gca()
    ax.set_ylabel("% Difference compared to " + label )
    ax.set_xlabel("Step size (m) used for integration")
    maxarg = np.array(f).argmax()
    ax.text(f[maxarg],diff[maxarg],".%.2fmbar"%(P/100),horizontalalignment="right",bbox=dict(facecolor='white', alpha=0.8))
    #ax.set_xscale("log")
    '''
'''
if ngamma:
    plotname = os.path.join(plotdir,"ngamma%.1fdeg.png"%(ang))
else:
    plotname = os.path.join(plotdir,"conversion%.1fkeV%.1fdeg.png"%(E,ang))
#pl.legend()

#pl.savefig(plotname)
'''
#}}}

#pl.show()
