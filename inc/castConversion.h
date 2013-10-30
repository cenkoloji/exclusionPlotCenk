/**********************************************************************
Definition of conversion formulas for axion-photon conversion
Based on a original codes of Javier Galan and Juan Antonio Garcia

Author: Cenk Yıldız
Bogazici University

Date: 22 Oct 2010
***********************************************************************/

#ifndef _CASTCONVERSION_
#define _CASTCONVERSION_

/*
#ifndef _ROOT_TOBJECT_
#include <TObject.h>
#endif
*/

#include<castMagnet.h>
#include<castGas.h>
#include<castProfile.h>

// I don't use these (They are from Javier-Juanan code)
//#define PhMeterIneV 5067731.236453719
//#define lightSpeed  2.99792458E8
//#define naturalElectron 0.302822120214353


// Conversions of important quantities to natural units
#define BLconversion 0.989842342144  // From Tm to GeV
#define Lconversion  5067730.93     // From m to eV-1


//using namespace std;


class castConversion// : public TObject
{

    public:

        castConversion(castMagnet *castMag, castGas *Cg);
        ~castConversion();

        castMagnet *mag;    //The parameters of the magnet
        castGas *gas;       //The parameters of the gas

        //Probability of conversion for a axion mass (eV), a energy (keV), pressure(mbar) and angle(deg)
        double ConversionProbability(double ma, double Ea, double pressure, double density, double angle = 0.0);


        //Probability of conversion for a axion mass (eV), Ea energy (keV), pressure(Pa), temp(K) and angle(deg), integrating over density profile
        double ConversionProbabilityFromProfile(double ma, double Ea, double pressure, double temperature, double angle);

        //Probability of conversion for a axion mass (eV), Ea energy (keV), pressure(Pa), temp(K) and angle(deg), calculating using center density and fixed length
        double ConversionProbabilityFromLength(double ma, double Ea, double pressure, double temperature, double angle);

        //return the normalized factor for press(mbar), angle(deg)
        double CalculateNormalizedFactor(double press,double angle = 0.0);

        //absortion coefficient factor for a pressure and a energy in the magnet (energy in keV and density in g/cm3)
        double AbsorptionCoeff(double en, double density,double angle = 0.0);

        //Axion flux cm-2 s-1 keV-1 (energy in eV)
        double getAxionFlux( double e );

        //Expected number of counts for a energy range (keV) a axion mass (eV) a expected mgamma (eV) and the exposure time (seconds) 
        double ExpectedNumberOfCounts( double Ei, double Ef, double ma, double pressure, double density, double time );

        //ClassDef(castConversion,1)
};
#endif
