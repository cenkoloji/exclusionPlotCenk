#include <iostream>
#include<castGas.h>
#include<castProfile.h>
#include <cmath>


castProfile::castProfile(castGas * cG, double press, double Tmag, double angle, castConfig * cfg) :gas(cG), press(press), Tmag(Tmag), angle(angle), cfg(cfg)  // {{{
{

    pCenter = cfg->pCenter; // If true, the pressure is central pressure instead of P_CB(at -5m)
    increment = cfg->increment; // Can be made angle+pressure dependent

    GetStartEnd();

    if (increment == 0.0)
        elements = 1;
    else
        elements = int ((lenend - lenstart) / increment);
    density = new double[elements];
    pressure = new double[elements];
    gas = cG;

    double height = sin(angle*PI/180.) * increment;

    // Getting pressure at start of length, depending on the input being central pressure or PCB
    if (cfg->pCenter)// Input is central pressure, calculating pressure at start of the profile
    {
        pressure[0] = press + gas->getHydrostatic(press,Tmag,0,lenstart,angle);
        //cout << press <<  " " <<  gas->getHydrostatic(press,Tmag,0,lenstart,angle) << endl;
        centerpressure = press;
        centerdensity = gas->getDensity(Tmag,press);
    }
    else
    {
        pressure[0] = press; // + gas->getHydrostatic(press,Tmag,lenstart,angle); // Pressure given by Pcb is same as pressure in the start of the profile, as there is not much gas volume in between
        //cout << press <<  " " <<  gas->getHydrostatic(press,Tmag,lenstart,angle) << endl;
        centerpressure = press + gas->getHydrostatic(press,Tmag,0,angle);
        centerdensity = gas->getDensity(Tmag,centerpressure);
    }

    density[0] = gas->getDensity(Tmag,pressure[0]);
    double phydro;
    
    if (cfg->useProfile)
    {
        // Calculating the pressure and density over the length
        for (int i = 1; i < elements; i++)
        {
            phydro = density[i-1] * height * GRAVITY;
            pressure[i] = pressure[i-1]  + phydro;
            density[i] = gas->getDensity(Tmag,pressure[i]);

            // Getting the center pressure
            if ((i-1)*increment + lenstart <0. && (i)*increment + lenstart >=0.)
            {   
                //cout << "mass1, mass2: " << gas->getPhotonMass(centerdensity*1E-3) << ", " << gas->getPhotonMass(density[i]*1E-3) << endl;
                centerdensity = density[i];
                centerpressure = pressure[i];
                //cout << " Center vs Input Press at point " << i << ":  " << centerpressure << " " << press << endl;
            }
            //if (i%100==1) { cout << "L=" <<lenstart + i*increment << ", P=" << pressure[i] << " d=" << density[i] <<" mgamma=" << gas->getPhotonMass(density[i]*1E-3) << " phydro=" << phydro << endl; }
        }
    }

} //}}}

castProfile::~castProfile() //{{{
{
    delete []density;
    delete []pressure;
}//}}}

void castProfile::GetStartEnd()//{{{
{

    // TODO: This function should have a parametrization of start and end points of profile.
    lenstart = -4.; //Should be get  as a function of pressure and angle // TODO
    lenend = 4.; //Should be get as a function  of pressure and angle //TODO

}
//}}}

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
