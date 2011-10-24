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

// Conversions of important quantities to natural units
#define BLconversion 0.989842342144  // From Tm to GeV
#define Lconversion  5067730.93     // From m to eV-1

#define PhMeterIneV 5067731.236453719
#define lightSpeed  2.99792458E8
#define naturalElectron 0.302822120214353

//using namespace std;


class castConversion// : public TObject
{

	public:

		castConversion(castMagnet *castMag, castGas *Cg);
		~castConversion();

		castMagnet *mag;//The parameters of the magnet
		castGas *gas;//The parameters of the gas

		double ConversionProbability(double ma, double Ea, double press, double angle = 0.0);//Probability of conversion for a axion mass (eV), a energy (keV) and pressure(mbar)
		double CalculateNormalizedFactor(double mg,double angle = 0.0);//return the normalized factor for the conversion probability 
		double AbsorptionCoeff(double en, double p,double angle = 0.0);//absortion coefficient factor for a pressure and a energy in the magnet (energy in keV and p in mbar)
		double getAxionFlux( double e ); //Axion flux cm-2 s-1 keV-1 (energy in eV)
		double ExpectedNumberOfCounts( double Ei, double Ef, double ma, double mgamma, double time );//return the expected number of counts for a energy range (keV) a axion mass (eV) a expected mgamma (eV) and the exposure time (seconds) 

		//ClassDef(castConversion,1)
};
#endif
