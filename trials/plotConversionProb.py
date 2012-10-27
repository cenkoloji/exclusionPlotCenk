#!/usr/bin/python
# Plots conversion probability for different pressures and energies.
# Uses files created by conversionProb.cxx

import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as pl
import os

inputdir = "out/"

"""
for filename in os.listdir(inputdir):
    pressure = float(filename[:filename.find("_")])
    energy = float(filename[filename.find("_")+1:filename.find("_")])
"""

# Opening data of pressure vs. mgamma
presMassArray = np.loadtxt(inputdir + "pvsM.txt",dtype=[('pressure','f'),('mgamma','f')],skiprows=1)

Earray = np.linspace(1,7,4)
parray = np.linspace(0,100,11)
data = {} ## data[p][e] is holding info of pressure = p and Energy = e

for p in parray:
    data[p] = {}
    for E in Earray:
        data[p][E] = np.loadtxt(inputdir + str(int(p)) + "_" + str(int(E)) + ".txt",dtype=[('ma','f'),('prob','f')])

# Plot of vacuum for different energies{{{
ax = pl.gca()
ax.set_xlabel("Axion Mass[eV]")
ax.set_ylabel("(Conversion Probability)")

for E in Earray:
    marray = data[0][E]['ma']
    probVac = data[0][E]['prob']
    ax.plot(marray,probVac,lw=2,alpha=1,label = "E=%gkeV"%(E))

ax.legend()
ax.set_xlim(0.0001,0.08)
#ax.set_xscale("log")
#ax.set_yscale("log")
#pl.savefig('plots/probVac.pdf',bbox_inches='tight')
pl.show() # }}}

# Plot of 10mbar for different energies {{{
p = 10
ax = pl.gca()
ax.set_xlabel("Axion Mass[eV]")
ax.set_ylabel("Conversion Probability(10mbar)")

for E in Earray:
    marray = data[p][E]['ma']
    probVac = data[p][E]['prob']
    ax.plot(marray,probVac,lw=2,alpha=1,label = "E=%gkeV"%(E))

ax.vlines(presMassArray['mgamma'][presMassArray['pressure'].searchsorted(p)],0,1.5e-17,color='black')

ax.legend()
ax.set_xlim(0.33,0.343)
#ax.set_xscale("log")
#ax.set_yscale("log")
#pl.savefig('plots/prob' + str(p) + 'mbar.pdf',bbox_inches='tight')
pl.show()
#}}}

#Plot for different pressures, for a fixed photon energy = 3keV {{{
fig = pl.figure(figsize=(12,5))
ax = pl.gca()
ax.set_xlabel("Axion Mass[eV]")
ax.set_ylabel("(Conversion Probability)")
E = 3 # keV

for p in parray:
    marray = data[p][E]['ma']
    probVac = data[p][E]['prob']
    ax.plot(marray,probVac,lw=1.5,alpha=1,label = "p=%gmbar"%(p))

#ax.legend()
ax.set_xlim(1e-5,1)
#ax.set_xscale("log")
#ax.set_yscale("log")
#pl.savefig('plots/probvsPressure.pdf',bbox_inches='tight')
pl.show() # }}}
