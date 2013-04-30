/**********************************************************************
 Definition of the Buffer Gas
 Based on a original codes of Javier Galan and Juan Antonio Garcia
 
 Author: Cenk Yıldız
 Bogazici University
 
 Date: 22 Oct 2010
 ***********************************************************************/

#ifndef _CASTGAS_
#define _CASTGAS_

/*
#ifndef _ROOT_TOBJECT_
#include <TObject.h>
#endif
*/

#include <iostream>
#include<castMagnet.h>

using namespace std;

#define PRESSENTRIES 300000

class castGas// : public TObject
{
	public:
		castGas(double atw, castMagnet *CASTM, int t);
		~castGas();

		castMagnet *mag;    //definition of the magnet
		double atomicwg;    //atomic weight of the buffer gas
		double pressmGamma[2][PRESSENTRIES]; // Not used in Cenk code?
		int entries;
		int type;           //type of gas: 0 for ideal gas 1 for HW and 2 for CW

		//sets
		void setType(int t){type=t;};//Sets the gas type

		//gets
		double getPhotonMass(double density);//return the effective photon mass in eV
		double getMagnetLength(double pressure, double angle=0.0);//return the magnet lengt for a given pressure inside de Coldbore

                // Function to print information
                void Show();

		//Double_t GetPhotonPressure(Double_t mgamma);//return the pressure for a expected mgamma in mbar // commented from Juanan

		//ClassDef (castGas,1); //artifact

};
#endif


