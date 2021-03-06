// Program to simulate CAST data

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <math.h>
#include <vector>
#include <cstdlib>
#include <castGas.h>
#include <castMagnet.h>
#include <time.h>

using namespace std;


int main(int argc,const char *argv[])
{

    double mai; // Initial mass
    double maf; // Final mass
    string cfgFileName;
    string outFileName;

    // Command Line Arguments {{{
    if(argc>=2)
    {
        for(int i = 1; i < argc; i++)
        {
            if( *argv[i] == '-')
            {
                argv[i]++;
                if( *argv[i] == '-') argv[i]++;
                {
                    switch ( *argv[i] )
                    {
                        case 'i' : mai=atof(argv[i+1]); break;
                        case 'f' : maf=atof(argv[i+1]); break;
                        case 'o' : outFileName=(argv[i+1]); break;
                        default : return 0;
                    }
                }
            }
        }
    }// }}}

    double mass_step = 0.001; // eV


    ofstream outFileMass;

    char fnameMass[256];
    sprintf(fnameMass,"/afs/cern.ch/exp/cast/mmscratch/cenk/excCodeCenk/inputs/mass_range.txt");

    outFileMass.open(fnameMass);
    outFileMass << mai << " " << maf;
    outFileMass.close();

    cout << "\nCreating magnet instance..." <<endl ;
    castMagnet *mag = new castMagnet();
    castGas *gas = new castGas(3.0160293,mag,1);

    double Tmag = 1.8;

    ofstream outFileTrk,outFileExp;

    char fnameTrk[256],fnameExp[256];
    sprintf(fnameTrk,"/afs/cern.ch/exp/cast/mmscratch/cenk/excCodeCenk/inputs/%strk1.txt",outFileName.c_str());
    sprintf(fnameExp,"/afs/cern.ch/exp/cast/mmscratch/cenk/excCodeCenk/inputs/%sexp1.txt",outFileName.c_str());

    cout << fnameTrk << endl;
    cout << fnameExp << endl;

    outFileTrk.open(fnameTrk);
    outFileTrk << std::setprecision(5);

    outFileExp.open(fnameExp);
    outFileExp << std::setprecision(5);

    // Number of shifts
    int nshifts ;
    nshifts = int((maf - mai) / mass_step);
    cout << "nshifts: " << nshifts <<endl ;
    double total_exposure_time = nshifts * 90 * 60; //seconds

    // Part1: Pressure exposure files{{{

    double pressure,density;

    double pressure1 = gas->getPressure(Tmag,gas->getDensityFromPhotonMass(mai)*1E3)/100;//mbar
    double pressure2 = gas->getPressure(Tmag,gas->getDensityFromPhotonMass(maf)*1E3)/100;//mbar
    //crosscheck if pressures are calculated right
        cout <<  gas->getPhotonMass(gas->getDensity(Tmag,pressure1*100)*1E-3) << " - " << pressure1 << endl;
        cout <<  gas->getPhotonMass(gas->getDensity(Tmag,pressure2*100)*1E-3) << " - " << pressure2 << endl;
    int Np = (maf - mai )/mass_step*2;  // Number of pressure points
    double pressure_step = (pressure2 - pressure1) / Np;
    cout << "Pressure step: " << pressure_step << endl;

    double angle;
    double angle_step = 1.;
    double angle1 = -7.5;
    double angle2 = +8.5;

    int Nangles = (angle2 - angle1) / angle_step; // number of different angles
    int count;

    double exposure_time = total_exposure_time / Nangles/ Np; // Exposure time for each entry

    for (int i = 0; i < Np; i++)
    {
        pressure = pressure1 + i * pressure_step;
        density = gas->getDensity(Tmag,pressure*100);


        for (int j = 0; j < Nangles; j++)
        {
            angle = angle1 + j * angle_step;
            count = i*Nangles + j;
            outFileExp << count << " " << count << " " << count  << " " << count << " "
                 << exposure_time << " " << angle << " " << Tmag << " " << pressure << " " << density << endl;
        }
    }

    //}}}

    // Part2: Tracking counts file{{{

    srand (time(NULL));

    /* generate secret number between 1 and 10: */


    double energy;
    double bck_level = 8E-6; //keV-1 cm-2 s-1
    double area = 14.55; //cm2
    double E[] = {2.0,7.0};

    int Ncounts = int(bck_level * area * total_exposure_time * (E[1] - E[0]));

    for (int i = 0; i < Ncounts ; i++)
    {
        energy = double(rand())/RAND_MAX*(E[1]-E[0]) + E[0];
        pressure = double(rand())/RAND_MAX*(pressure2-pressure1) + pressure1;
        angle = double(rand())/RAND_MAX*(angle2 - angle1) + angle1;

        density = gas->getDensity(Tmag,pressure*100);

        outFileTrk << count << " " << energy << " " << density << " " << bck_level << " "
                   << angle << " " << Tmag << " " << pressure <<  endl;

    }
    //}}}


    outFileTrk.close();
    return 0;
}
