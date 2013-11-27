#!/usr/bin/python
#Performs all plots for the exclusion chapter

import os
import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as pl

plot_sigma = 1
plot_limit = 1
lims = [(0.8,1.2)]

#colors = {1:"red",2:"green",3:"blue","all":"black"}
#ls = {1:"dotted",2:"-",3:"dashdot","all":"-"}
#labels = {1:"B3",2:"M6",3:"M9","all":"combined"}
#output_dirs = {1:"outputs1",2:"outputs2",3:"outputs3","all":"outputs"}
output_dirs = {"all":"simoutputsFixed","0.01":"simoutputs0.01"}#,2:"outputs2",3:"outputs3","all":"outputs"}
colors = {"0.01":"red","all":"green"}
ls = {"0.01":"--","all":"-"}
lw = {"0.01":2,"all":1}
labels = {"0.01":"0.01m","all":"Fixed"}

def plotSigma(det):#{{{

    sigma_outdir = os.path.join(output_dirs[det],"sigmas")
    sigma_files = os.listdir(sigma_outdir)
    sigma_files.sort()

    mass_array = []
    nsigma_r_array = []
    nsigma_l_array = []
    nsigma_mean_array = []
    nsigma_sign_array = [] #Filled with right sigma if maxG4<0, with left if otherwise

    for fi in sigma_files:

        try:
            ma = float(fi.strip(".txt"))

            full_fname = os.path.join(sigma_outdir,fi)
            maxG4,sigma_leftG4,sigma_rightG4 = np.loadtxt(full_fname,dtype=np.float)
            sigma_rightG4 = np.abs(sigma_rightG4)

            mass_array.append(ma)
            #print ma,maxG4,sigma_leftG4,"-",sigma_rightG4, "nsigmas: ", maxG4/((sigma_leftG4+sigma_rightG4)/2)
            nsigma_r = maxG4/sigma_rightG4
            nsigma_l = maxG4/sigma_leftG4
            nsigma_mean = maxG4/((sigma_leftG4+sigma_rightG4)/2.)
            nsigma_sign_array.append(nsigma_r if maxG4 < 0 else nsigma_l)

            nsigma_r_array.append(nsigma_r)
            nsigma_l_array.append(nsigma_l)
            nsigma_mean_array.append(nsigma_mean)

            if maxG4 < 0: nsigma_sign_array[-1] = 0

        except:
            print fi

    nsigma_sign_array = np.array(nsigma_sign_array)

    mass_array = np.array(mass_array)
    #lims = [(0.385,0.456),(0.464,0.5),(0.512,0.563),(0.572,0.615),(0.625,0.652)]
    #lims = [(0.80,1.95)]

    mask = (mass_array==-1)
    for lim in lims:
        mask = mask + (mass_array>lim[0]) * (mass_array<lim[1]) 

    mass_array = mass_array[mask]
    nsigma_sign_array = nsigma_sign_array[mask]

    N = nsigma_sign_array.shape[0]/2
    print N

    for n in [1,2,3,4]:
        g = np.count_nonzero(nsigma_sign_array>n)
        print g
        #print ("More than {} sigma: {:.4f}".format(n, g* 100./N ))


    pl.plot(mass_array,nsigma_sign_array,"o",color=colors[det],label=labels[det])

#}}}

def plotLimit(det,ngammafig=1,limitfig=2):# {{{
    ma_outdir = os.path.join(output_dirs[det],"ma")
    ma_files = os.listdir(ma_outdir)
    ma_files.sort()

    mass_array = []
    ngamma_array = []
    limit_array = []

    for fi in ma_files:

        ma = float(fi.strip(".txt"))

        mass_array.append(ma)

        full_fname = os.path.join(ma_outdir,fi)

        ma, ngamma, limit = np.loadtxt(full_fname,dtype=np.float)

        limit = np.sqrt(np.sqrt(limit))*1E-10

        ngamma_array.append(ngamma)

        limit_array.append(limit)

    pl.figure(ngammafig)
    pl.plot(mass_array,ngamma_array,color=colors[det],linestyle=ls[det],linewidth = lw[det],label=labels[det])

    mass_array = np.array(mass_array)
    limit_array = np.array(limit_array)
    #Limits to calculate mean
    #lims= [(0.385,0.652)]
    #lims = [(0.385,0.456),(0.464,0.5),(0.512,0.563),(0.572,0.615),(0.625,0.652)]
    #lims = [(0.8,1.95)]

    mask = (mass_array==-1)
    for lim in lims:
        mask = mask + (mass_array>lim[0]) * (mass_array<lim[1]) 
    print mask
    mass_array=mass_array[mask]
    limit_array=limit_array[mask]
    print("Mean=",limit_array.mean())

    pl.figure(limitfig)
    pl.plot(mass_array,limit_array,color=colors[det],linestyle=ls[det],linewidth = lw[det],label=labels[det])
    print mass_array
    print limit_array

#}}}

#Headers:
#sigmas: maxGl, sigmaLeft, sigmaRight -> Have to take abs of sigmaRight(bug in older version)
#ma: ma, ngamma, gl4

base_outdir="outputs"

dets = [1,2,3,"all"]

if plot_sigma:#{{{

    pl.figure(figsize=(12,5))
    #pl.title("Null Hypothesis Testing")
    pl.legend()
    ax = pl.gca()
    #ax.set_xlim(0.85,0.95)
    ax.set_xlim(lims[0])
    ax.set_xlabel("Axion mass(eV)")
    ax.set_ylabel("Number of sigmas")
    for det in ["all","0.01"]:
        plotSigma(det)
    pl.legend()
    #pl.savefig("exc_nsigma.pdf",bbox_inches="tight")
    pl.show()
#}}}

if plot_limit:#{{{
    pl.figure(1,figsize=(12,5))

    #pl.title("Number of Expected Photons")
    ax = pl.gca()
    #ax.set_xlim(0.85,0.95)
    ax.set_xlim(lims[0])
    ax.set_xlabel("Axion mass(eV)")
    ax.set_ylabel("Number of expected photons")

    pl.figure(2)

    #pl.title("Exclusion Plot")
    ax = pl.gca()
    #ax.set_xlim(0.85,0.95)
    ax.set_xlim(lims[0])
    ax.set_ylim(1E-11,1E-8)
    ax.set_xlabel("Axion mass(eV)")
    ax.set_ylabel("$g_{a\gamma}($GeV$^{-1})$")

    for det in ["all","0.01"]:
        plotLimit(det)

    pl.figure(1)
    pl.legend()
    #pl.savefig("exc_ngamma.pdf",bbox_inches="tight")

    pl.figure(2)

    #Plotting models:{{{
    '''
    def g_a(m_a,R):
        """g_a in terms of mass and R=E/N"""
        return m_a*1.936E-10*np.abs(R-1.93)

    m_array = np.array([1E-5,1E-4,1E-3,1E-2,1E-1,1])

    #pl.figure(figsize=(6,4))

    R = 0.
    g_array = g_a(m_array,R)
    pl.plot(m_array,g_array,"-",label="E/N=0")

    R = 2.
    g_array = g_a(m_array,R)
    pl.plot(m_array,g_array,"--",label="E/N=2")

    R = 8/3.
    g_array = g_a(m_array,R)
    pl.plot(m_array,g_array,"-.",label="E/N=8/3")

    R = 6.
    g_array = g_a(m_array,R)
    pl.plot(m_array,g_array,"-",lw=2,alpha=0.5,label="E/N=6")
    #}}}
    '''

    pl.yscale("log")
    pl.legend()
    #pl.savefig("exc_exclusionplot_models.pdf",bbox_inches="tight")
    pl.show()

#}}}

