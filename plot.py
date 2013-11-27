#!/usr/bin/python

import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as pl

import os

base_dir = "simoutputs0.01/"
base_dir = "simoutputsFixed/"
plots_dir = os.path.join(base_dir,"plots")
sigma_dir = os.path.join(base_dir,"sigmas")
ma_dir = os.path.join(base_dir,"ma")

files = os.listdir(base_dir)

files.sort()

i = 0

files = ["0.885000"]

for f in files:
    i += 1

    full_fname = os.path.join(sigma_dir,f)
    maxG4, sigma_leftG4, sigma_rightG4 = np.loadtxt(full_fname,dtype=np.float)

    full_fname = os.path.join(ma_dir,f+".txt")
    ma, ngamma, limit = np.loadtxt(full_fname,dtype=np.float)

    full_fname = os.path.join(plots_dir,f)
    data = np.loadtxt(full_fname)
    print full_fname
    g4 = data[:,1]
    expchi = data[:,7]
    chi = data[:,2]

    print limit

    pl.figure(figsize=(6,5))

    ax = pl.gca()

    #pl.title("$m_a= " +f + "eV$")

    p1 = pl.plot(g4,expchi,"b-",label="$\mathcal{L}_{m_a=%g}$"%(ma))
    ax.set_ylabel("$\mathcal{L}$")
    ax.set_xlabel("$g^4_{10}$")
    ax.set_xlim(-10,100)
    ax.set_xlim(g4.min()-(g4.max()-g4.min())/10,g4.max()+(g4.max()-g4.min())/10)

    pl.vlines((maxG4-sigma_leftG4,maxG4+np.abs(sigma_rightG4)),expchi.min(),expchi.max(),color='k',linestyles='dashed',label="left,right deviation")

    mask = (g4>0) * (g4<limit)
    g4 = g4[mask]
    chi = chi[mask]
    expchi = expchi[mask]

    ax.fill_between(g4,expchi,0,hatch="|",color="gray")


    '''
    ax2 = ax.twinx()
    p2 = pl.plot(g4,chi,"g:",label="chi")
    ax2.set_ylabel("chi")


    ps = p1 + p2

    pl.legend(ps,[p.get_label() for p in ps])
    '''

    pl.legend()
    pl.savefig("/tmp/"+ f +"_g4dist.pdf")
    pl.show()
