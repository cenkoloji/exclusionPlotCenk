
#include <iostream>
#include<castGas.h>
#include<castProfile.h>
#include <cmath>


castProfile::castProfile(castGas * cG, double PCB, double Tmag, double angle) :gas(cG), PCB(PCB), Tmag(Tmag), angle(angle) // {{{
{

    lenstart = -4.; //Should be get by a function in castGas
    lenend = 4.; //Should be get by a function in castGas
    increment = 0.01; // Can be angle+pressure dependent

    elements = int ((lenend - lenstart) / increment);
    density = new double[elements];
    pressure = new double[elements];
    gas = cG;

    double height = sin(angle*PI/180.) * increment;

    pressure[0] = PCB + gas->getHydrostatic(PCB,Tmag,lenstart,angle);
    density[0] = gas->getDensity(Tmag,pressure[0]);
    double phydro;

    for (int i = 1; i < elements; i++)
    {
        phydro = density[i-1] * height * GRAVITY;
        pressure[i] = pressure[i-1]  + phydro;
        density[i] = gas->getDensity(Tmag,pressure[i]);
        if ((i-1)*increment + lenstart <0. && (i)*increment + lenstart >=0.)
        {   
            centerdensity = density[i];
            centerpressure = pressure[i];
        }
        //if (i%100==1) { cout << "L=" <<lenstart + i*increment << ", P=" << pressure[i] << " d=" << density[i] <<" mgamma=" << gas->getPhotonMass(density[i]*1E-3) << " phydro=" << phydro << endl; }
    }

} //}}}

castProfile::~castProfile()
{
    delete []density;
    delete []pressure;
}

/*
    //{{{  int method = 2; // Different ways to calculate hydrostatic pressure

    cout << method << endl;
    if (method==0)
    {
        double hydrostatic_increment = gas->getHydrostatic(PCB,Tmag,POSPCB-increment,angle);

        pressure[0] = PCB + gas->getHydrostatic(PCB,Tmag,lenstart,angle);
        density[0] = gas->getDensity(Tmag,pressure[0]);

        for (int i = 1; i < elements; i++) {
            pressure[i] = PCB + i*hydrostatic_increment;
            density[i] = gas->getDensity(Tmag,pressure[i]);
            if (i%100==1)
                cout << "L=" <<lenstart + i*increment << ", P=" << pressure[i] << " d=" << density[i] << " mgamma=" << gas->getPhotonMass(density[i]*1E-3) << endl;
        }

    }
    else if (method==1)
    {
        pressure[0] = PCB + gas->getHydrostatic(PCB,Tmag,lenstart,angle);
        density[0] = gas->getDensity(Tmag,pressure[0]);

        for (int i = 1; i < elements; i++)
        {
            pressure[i] = PCB + gas->getHydrostatic(PCB,Tmag,lenstart+i*increment,angle);
            density[i] = gas->getDensity(Tmag,pressure[i]);
            if (i%100==1)
                cout << "L=" <<lenstart + i*increment << ", P=" << pressure[i] << " d=" << density[i] << " mgamma=" << gas->getPhotonMass(density[i]*1E-3) << endl;
        }
    }
    else if (method==2)
    {
        double height = sin(angle*PI/180.) * increment;

        pressure[0] = PCB + gas->getHydrostatic(PCB,Tmag,lenstart,angle);
        density[0] = gas->getDensity(Tmag,pressure[0]);
        double phydro;

        for (int i = 1; i < elements; i++)
        {
            phydro = density[i-1] * height * GRAVITY;
            pressure[i] = pressure[i-1]  + phydro;
            density[i] = gas->getDensity(Tmag,pressure[i]);
            if (i%100==1)
            {
                cout << "L=" <<lenstart + i*increment << ", P=" << pressure[i] << " d=" << density[i] <<" mgamma=" << gas->getPhotonMass(density[i]*1E-3) << " phydro=" << phydro << endl;
            }
        }
    }
    //}}}
*/
