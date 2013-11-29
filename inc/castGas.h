/**********************************************************************
 Definition of the Buffer Gas
 Based on a original codes of Javier Galan and Juan Antonio Garcia
 
 Author: Cenk Yıldız
 Bogazici University
 
 Date: 22 Oct 2010
 ***********************************************************************/

#ifndef _CASTGAS_
#define _CASTGAS_

#include <iostream>
#include<castMagnet.h>

using namespace std;

#define PRESSENTRIES 300000

// Parameters for peng-robinson
#define  MWT 3.016
#define  PCRIT 116000
#define  TCRIT 3.3243
#define  ACENT -0.474

// For hydrostatic
#define  GRAVITY  9.81 
#define  POSPCB  -5 // position of PCB
#define  PI 3.1415926

class castGas
{

	public:
		castGas(double atw, castMagnet *CASTM, int t);
		~castGas();

                double rgas;
                double a0;
                double b0;
                double k0;

		castMagnet *mag;    //definition of the magnet
		double atomicwg;    //atomic weight of the buffer gas
		double pressmGamma[2][PRESSENTRIES]; // Not used in Cenk code?
		int entries;
		int type;           //type of gas: 0 for ideal gas 1 for HW and 2 for CW

		//sets
		void setType(int t){type=t;};//Sets the gas type

		//gets
		double getPhotonMass(double density);//return the effective photon mass in eV from density in g/cm3
		double getDensityFromPhotonMass(double ma);//return the density in g/cm3 from effective photon mass in eV
		double getMagnetLength(double pressure, double angle=0.0);//return the magnet lengt for a given pressure inside de Coldbore,p:mbar

                // Function to print information
                void Show();


                double getDensity(double temp, double press); // Calculates density(kg/m3) from temperature(K) and pressure(Pa) (peng-robinson)

                double getPressure(double temp, double density); // Calculates pressure from temperature and density(peng robinson)

                // Hydrostatic pressure difference between some point in the magnet and PCB (which is at -5m)
                double getHydrostatic(double PCB, double temp, double x, double angle); //  press(Pa), temp(K), x(m), angle(deg)

                // Hydrostatic pressure difference between x0 and x1 (overloaded)
                double getHydrostatic(double press, double temp, double x0, double x1, double angle);

};
#endif


