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
#include<castConfig.h>
using std::cout;
using std::endl;


int main(int argc,char **argv)
{


    double pressure = 0.;
    double angle = 0.;
    double energy = 0.;
    bool ngamma = false;
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
                        case 'n' : ngamma = true; break;

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

    //cout <<  pressure << " "<<   angle << " "<<  energy << ", ngamma: " << ngamma << endl;

    //Fixed temperature
    double temperature = 1.8;

    // Creating config instances with different parameters
    castConfig * cfg1 = new castConfig("config1.cfg");
    castConfig * cfg2 = new castConfig("config2.cfg");
    castConfig * cfg3 = new castConfig("config3.cfg");
    castConfig * cfg4 = new castConfig("config4.cfg");
    castConfig * cfg5 = new castConfig("config5.cfg");

    //castMagnet instance
    castMagnet *mag = new castMagnet();
    mag->Show();

    //castGas instance
    castGas *gas = new castGas(3.0160293,mag,1);

    //castConversion instance
    castConversion *conv1 = new castConversion(mag,gas,cfg1);
    castConversion *conv2 = new castConversion(mag,gas,cfg2);
    castConversion *conv3 = new castConversion(mag,gas,cfg3);
    castConversion *conv4 = new castConversion(mag,gas,cfg4);
    castConversion *conv5 = new castConversion(mag,gas,cfg5);


    cout << "Increment1: " << cfg1->increment <<endl ;
    cout << "Increment2: " << cfg2->increment <<endl ;
    cout << "Increment3: " << cfg3->increment <<endl ;
    cout << "Increment4: " << cfg4->increment <<endl ;
    cout << "Increment5: " << cfg5->increment <<endl ;

    castProfile * profile;
    profile = new castProfile(gas, pressure, temperature, angle, cfg1);
    double ma = gas->getPhotonMass(profile->centerdensity*1E-3);
    //cout << "P vs Pcen: " << pressure << " " << profile->centerpressure << endl;
    //cout << "d vs dcen: " <<  gas->getDensity(temperature, pressure)<< " " << profile->centerdensity << endl;
    cout << ma << endl;
    //double ma2 = gas->getPhotonMass(gas->getDensity(temperature, pressure)*1E-3);
    //cout << ma << " " << ma2 << endl;

    //castProfile * profile2;
    //profile2 = new castProfile(gas, pressure, temperature, angle, cfg2);
    //ma = gas->getPhotonMass(profile2->centerdensity*1E-3);
    //cout << ma << endl;

    cout << "DATA" <<endl ;
    cout << "ma "<< cfg1->increment << " " << cfg2->increment  << " "<<  cfg3->increment  << " " <<  cfg4->increment  << endl;

    double E0=2.0;
    double E1=7.0;
    double prob;

    double ngammafixed = 0.; // For integration over angles
    double ngammaprof = 0.; // For integration over angles
    double prob1,prob2;

    double mass_increment = 0.001;

    for (double m = ma-2*mass_increment; m <= ma + 2*mass_increment ; m = m + mass_increment)
    {
        if (ngamma)
        {
            prob = conv1->ExpectedNumberOfCounts(E0,E1,m, pressure/100., temperature , angle,30*60);
            cout << m << " " <<prob << " ";

            prob = conv2->ExpectedNumberOfCounts(E0,E1,m, pressure/100., temperature , angle,30*60);
            cout << prob  << " ";

            prob = conv3->ExpectedNumberOfCounts(E0,E1,m, pressure/100., temperature , angle,30*60);
            cout << prob  << " ";

            prob = conv4->ExpectedNumberOfCounts(E0,E1,m, pressure/100., temperature , angle,30*60);
            cout << prob <<  " ";

            prob = conv5->ExpectedNumberOfCounts(E0,E1,m, pressure/100., temperature , angle,30*60);
            cout << prob << endl;
        }
        else
        {
            prob = conv1->ConversionProbability(m,energy, pressure/100., temperature , angle);
            cout << m << " " <<prob << " ";

            prob = conv2->ConversionProbability(m,energy, pressure/100., temperature , angle);
            cout << prob  << " ";

            prob = conv3->ConversionProbability(m,energy, pressure/100., temperature , angle);
            cout << prob  << " ";

            prob = conv4->ConversionProbability(m,energy, pressure/100., temperature , angle);
            cout << prob <<  " ";

            prob = conv5->ConversionProbability(m,energy, pressure/100., temperature , angle);
            cout << prob << endl;
        }


        /* // {{{
        ngammaprof = 0;
        ngammafixed = 0;
        for (angle=-8.; angle < +9.; angle = angle + 1.0)
        {

            delete cfg4, cfg5;
            cfg4 = new castConfig("configCprof.cfg");
            cfg5 = new castConfig("configCfixed.cfg");

            delete conv4, conv5;
            conv4 = new castConversion(mag,gas,cfg4);
            conv5 = new castConversion(mag,gas,cfg5);

            prob1 = conv4->ExpectedNumberOfCounts(E0,E1,m, pressure/100., temperature , angle,337);
            ngammaprof += prob1;

            prob2 = conv5->ExpectedNumberOfCounts(E0,E1,m, pressure/100., temperature , angle,337);
            ngammafixed += prob2;

            cout << "ma= " << m << " ang=" << angle << " probprof=" << prob1 << " probfixed=" << prob2  <<" %% diff=" << 100*(prob1-prob2)/prob1 <<endl;
        }

        cout << "ma= "<<m << ", Profile:" <<ngammaprof << ", Fixed:" << ngammafixed << ", %% diff:"  << 100*(ngammaprof - ngammafixed)/ngammaprof << endl ;

        */ // }}}
    }


    return 0;
}


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
