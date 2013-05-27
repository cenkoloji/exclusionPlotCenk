#!/usr/bin/python
#Performs all plots for the exclusion chapter

import os
import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as pl

plot_sigma = 0
plot_ngamma = 1
plot_limit = 1

#Headers:
#sigmas: maxGl, sigmaLeft, sigmaRight -> Have to take abs of sigmaRight(bug in older version)
#ma: ma, ngamma, gl4

base_outdir="outputs2"

if plot_sigma:#{{{
    sigma_outdir = os.path.join(base_outdir,"sigmas")
    sigma_files = os.listdir(sigma_outdir)
    sigma_files.sort()

    mass_array = []
    nsigma_r_array = []
    nsigma_l_array = []
    nsigma_mean_array = []


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

            nsigma_r_array.append(nsigma_r)
            nsigma_l_array.append(nsigma_l)
            nsigma_mean_array.append(nsigma_mean)

            #sigma_array.append(sigma)
        except:
            print fi
    pl.figure(1)
    pl.plot(mass_array,nsigma_l_array,label="left")
    pl.plot(mass_array,nsigma_r_array,label="right")
    pl.title("Sigmas")
    pl.legend()
    ax = pl.gca()
    pl.show()
#}}}

if plot_limit or plot_ngamma:#{{{
    ma_outdir = os.path.join(base_outdir,"ma")
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
#}}}

if plot_ngamma: #{{{
    pl.figure(2)
    #ax2=ax.twinx()
    pl.plot(mass_array,ngamma_array,color="orange",label="ngamma2")
    #pl.title("ngamma")
    #pl.show()
#}}}

if plot_limit:#{{{
    #ax2 = ax.twinx()
    pl.plot(mass_array,limit_array,color="orange")
    pl.title("limit")
    pl.yscale("log")
    #ax2.set_ylim(0,1E-9)
    pl.show()
#}}}

base_outdir="outputs3"

if plot_sigma:#{{{
    sigma_outdir = os.path.join(base_outdir,"sigmas")
    sigma_files = os.listdir(sigma_outdir)
    sigma_files.sort()

    mass_array = []
    nsigma_r_array = []
    nsigma_l_array = []
    nsigma_mean_array = []


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

            nsigma_r_array.append(nsigma_r)
            nsigma_l_array.append(nsigma_l)
            nsigma_mean_array.append(nsigma_mean)

            #sigma_array.append(sigma)
        except:
            print fi
    pl.figure(1)
    pl.plot(mass_array,nsigma_l_array,label="left")
    pl.plot(mass_array,nsigma_r_array,label="right")
    pl.title("Sigmas")
    pl.legend()
    ax = pl.gca()
    pl.show()
#}}}

if plot_limit or plot_ngamma:#{{{
    ma_outdir = os.path.join(base_outdir,"ma")
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
#}}}

if plot_ngamma: #{{{
    pl.figure(2)
    #ax2=ax.twinx()
    pl.plot(mass_array,ngamma_array,color="red",label="ngamma3")
    #pl.title("ngamma")
    pl.legend()
#}}}

if plot_limit:#{{{
    #ax2 = ax.twinx()
    pl.plot(mass_array,limit_array,color="orange")
    pl.title("limit")
    pl.yscale("log")
    #ax2.set_ylim(0,1E-9)
    pl.show()
#}}}

base_outdir="outputs1"

if plot_sigma:#{{{
    sigma_outdir = os.path.join(base_outdir,"sigmas")
    sigma_files = os.listdir(sigma_outdir)
    sigma_files.sort()

    mass_array = []
    nsigma_r_array = []
    nsigma_l_array = []
    nsigma_mean_array = []


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

            nsigma_r_array.append(nsigma_r)
            nsigma_l_array.append(nsigma_l)
            nsigma_mean_array.append(nsigma_mean)

            #sigma_array.append(sigma)
        except:
            print fi
    pl.figure(1)
    pl.plot(mass_array,nsigma_l_array,label="left")
    pl.plot(mass_array,nsigma_r_array,label="right")
    pl.title("Sigmas")
    pl.legend()
    ax = pl.gca()
    pl.show()
#}}}

if plot_limit or plot_ngamma:#{{{
    ma_outdir = os.path.join(base_outdir,"ma")
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
#}}}

if plot_ngamma: #{{{
    pl.figure(2)
    #ax2=ax.twinx()
    pl.plot(mass_array,ngamma_array,color="blue",label="ngamma1")
    #pl.title("ngamma")
    pl.legend()
#}}}

if plot_limit:#{{{
    #ax2 = ax.twinx()
    pl.plot(mass_array,limit_array,color="orange")
    pl.title("limit")
    pl.yscale("log")
    #ax2.set_ylim(0,1E-9)
    pl.show()
#}}}

base_outdir="outputs"

if plot_sigma:#{{{
    sigma_outdir = os.path.join(base_outdir,"sigmas")
    sigma_files = os.listdir(sigma_outdir)
    sigma_files.sort()

    mass_array = []
    nsigma_r_array = []
    nsigma_l_array = []
    nsigma_mean_array = []


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

            nsigma_r_array.append(nsigma_r)
            nsigma_l_array.append(nsigma_l)
            nsigma_mean_array.append(nsigma_mean)

            #sigma_array.append(sigma)
        except:
            print fi
    pl.figure(1)
    pl.plot(mass_array,nsigma_l_array,label="left")
    pl.plot(mass_array,nsigma_r_array,label="right")
    pl.title("Sigmas")
    pl.legend()
    ax = pl.gca()
    pl.show()
#}}}

if plot_limit or plot_ngamma:#{{{
    ma_outdir = os.path.join(base_outdir,"ma")
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
#}}}

if plot_ngamma: #{{{
    pl.figure(2)
    #ax2=ax.twinx()
    pl.plot(mass_array,ngamma_array,color="purple",label="ngamma_combined")
    #pl.title("ngamma")
    pl.legend()
    pl.show()
#}}}

if plot_limit:#{{{
    #ax2 = ax.twinx()
    pl.plot(mass_array,limit_array,color="orange")
    pl.title("limit")
    pl.yscale("log")
    #ax2.set_ylim(0,1E-9)
    pl.show()
#}}}
