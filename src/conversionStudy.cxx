


//Program to compare different ways of calculating conversion probability
// 1) Old way, using coherence length and photon mass
// 2) New way, using the density profile

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include<castConversion.h>
#include<castMagnet.h>
#include<castGas.h>
#include<castExposure.h>
#include<castTracking.h>
using std::cout;
using std::endl;


int main(int argc,char **argv)
{


    double pressure = 0.;
    double angle = 0.;
    double energy = 0.;
    int index;
    int c;

    // Command Line Arguments {{{
    if(argc>=2)
    {
        for(int i = 1; i < argc; i++)
        {
            if( *argv[i] == '-')
            {
                argv[i]++;
                if( *argv[i] == '-')
                {
                    argv[i]++;
                    switch ( *argv[i] )
                    {
                        case 'p' : pressure=atof(argv[i+1]); break;
                        case 'e' : energy=atof(argv[i+1]); break;
                        case 'a' : angle=atof(argv[i+1]); break;

                        default : cout << "Please enter arguments!!!" << endl;return 0;
                    }
                }
                else
                {
                    argv[i]++;
                    continue;
                }
            }
        }
    }// }}}

    //cout <<  pressure << " "<<   angle << " "<<  energy << endl;
    double temperature = 1.8;

    //castMagnet instance
    castMagnet *mag = new castMagnet();
    mag->Show();


    //castGas instance
    castGas *gas = new castGas(3.0160293,mag,1);

    //castConversion instance
    castConversion *conv = new castConversion(mag,gas);

    /* Comparing different methods to calculate hydrostatic {{{
    double p2,p3;


    cout << "Angle=" << angle << endl;
    cout << "PCB[Pa](at -5m)  PCB[Pa](at +5m)(fixed density) PCB[Pa](at +5m)(variable density) mgamma(variable density)  mgamma(fixed density)" << endl;
    for (double p = 3000; p < 11000; p=p+1000) {
        p2 = p + gas->getHydrostatic(p,temperature,0,angle);
        p3 = p + gas->getHydrostatic(p,temperature,POSPCB,0,angle);

        cout << p << "\t\t" << p2 << " \t\t" << p3 << " \t\t" << gas->getPhotonMass(gas->getDensity(temperature,p2)*1E-3) << " \t\t" << gas->getPhotonMass(gas->getDensity(temperature,p3)*1E-3) << endl;
        //cout << "Pressure at +5m(Pa) (variable density)="<< p2 << " Pressure at +5 m(Pa) (fixed density)=" << p3 << endl;
        //cout << "mgamma (variable density)="<<gas->getPhotonMass(gas->getDensity(temperature,p2)*1E-3) << " mgamma (fixed density)=" <<gas->getPhotonMass(gas->getDensity(temperature,p3)*1E-3) << endl;
    }
    // }}} */


    castProfile * profile;
    profile = new castProfile(gas, pressure, temperature, angle, true);
    double ma = gas->getPhotonMass(profile->centerdensity*1E-3);
    cout << ma << endl;
    cout << "ma Prob(new) Prob(old) "  << endl;

    for (double m = ma-0.01; m < ma + 0.01; m = m + 0.0001)
    {
        double prob = conv->ConversionProbabilityFromProfile(m,energy, pressure/100., temperature , angle);

        cout << m << " " <<prob << " ";

        prob = conv->ConversionProbabilityFromLength(m,energy, pressure/100., temperature , angle);

        cout << prob << endl;

        prob = conv->ConversionProbabilityFromLength(m,energy, pressure/100., temperature , angle);

        cout << prob << endl;

    }
    return 0;
}
