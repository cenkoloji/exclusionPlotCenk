#!/usr/bin/python
import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as pl
import os

dtype = [('m','f'),('ngamma','f')]

ngamma = []

fig = pl.figure(figsize=(5,4))

i = 0
for f in os.listdir("."):
    if f.find("stepOut")!= -1:
        dens = f[14:]
        data = np.loadtxt(f,dtype=dtype)
        pl.plot(data["m"],data["ngamma"],label="$\\rho="+dens+"$")
        ngamma.append( data["ngamma"] )
        i += 1

ngamma = ngamma[0] + ngamma[1]

ax = pl.gca()
ax.set_xlabel("Axion Mass(eV)")
ax.set_ylabel("Expected Number of Photons")
ax.set_xlim(0.522,0.53)

pl.plot(data["m"],ngamma,label="Total")
pl.legend()
fig.autofmt_xdate()

pl.savefig("stepSize.pdf",bbox_inches="tight")
pl.show()
