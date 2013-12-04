/**********************************************************************
Definition of conversion formulas for axion-photon conversion
Based on a original codes of Javier Galan and Juan Antonio Garcia

Author: Cenk Yıldız
Bogazici University

Date: 22 Oct 2010
***********************************************************************/

#ifndef _CASTCONVERSION_
#define _CASTCONVERSION_

#include<castMagnet.h>
#include<castGas.h>
#include<castProfile.h>
#include<castConfig.h>

// I don't use these (They are from Javier-Juanan code)
//#define PhMeterIneV 5067731.236453719
//#define lightSpeed  2.99792458E8
//#define naturalElectron 0.302822120214353


// Conversions of important quantities to natural units
#define BLconversion 0.989842342144  // From Tm to GeV
#define Lconversion  5067730.93     // From m to eV-1

class castConversion
{

    public:

        castConversion(castMagnet *castMag, castGas *Cg, castConfig * cfg): mag(castMag), gas(Cg), cfg(cfg){};
        ~castConversion();

        castMagnet *mag;    //The parameters of the magnet
        castGas *gas;       //The parameters of the gas
        castConfig *cfg;    //The config class (defines settings for density profiles)

        //Probability of conversion for a axion mass (eV), a energy (keV), pressure(mbar), temperature(K) and angle(deg)
        double ConversionProbability(double ma, double Ea, double pressure, double temperature, double angle = 0.0);

        //absortion coefficient factor for a density( g/cm3) and a energy(keV)
        double AbsorptionCoeff(double en, double density,double angle = 0.0);

        //Axion flux cm-2 s-1 keV-1 (energy in eV)
        double getAxionFlux( double e );

        //Expected number of counts for a energy range (keV), an axion mass (eV), pressure(mbar), temp(K), ang(deg), the exposure time (seconds) 
        double ExpectedNumberOfCounts( double Ei, double Ef, double ma, double pressure, double temperature, double angle, double time );

};
#endif
