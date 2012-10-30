/**********************************************************************
Definition of a detector parameters for CAST exclusion Plot
Based on a original codes of Javier Galan and Juan Antonio Garcia

Author: Cenk Yıldız
Bogazici University

Date: 24 Oct 2010
***********************************************************************/

#ifndef _CASTDETECTOR_
#define _CASTDETECTOR_

/*
#ifndef _ROOT_TOBJECT_
#include <TObject.h>
#endif
*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define EFF_POINTS 60 // Points on efficiency curve

using namespace std;

class castDetector // : public TObject
{

        private:
                double static const stepsize = 0.2; //Energy step size for MM Efficiency file
		double opticsEfficiency;            //Optic Efficiency for the focus X-ray
		double focusArea;                   //Focus Area of the detector in cm2 (For MM it is CB area)
                                                    //(e.g. for mM is the area of the coldbore and for CCD is around 0.15 cm2)

		double Einitial;                    //Minimum energy used in analysis
		double Efinal;                      //Maximum energy used in analysis
		double detEfficiency[EFF_POINTS];   //Efficiency array of the detector

	public:
		castDetector(double Ei = 2.0, double Ef = 7.0, double area = 14.55, double oeff = 1.0);
		~castDetector();

		//Sets
		void setDetEfficiency(char *effFileName,double softEff); // for MM
		void setEinitial(double Ei){Einitial = Ei;};
		void setEfinal(double Ef){Efinal = Ef;};
		void setFocusArea(double fA){focusArea = fA;};  //For SSMM, same as CB area

                //CCD only
		void setOpticsEfficiency(double oEff){ opticsEfficiency= oEff;}; // CCD
		void setCCDEfficiency(char *effFileName);       //CCD
		double getOpticsEfficiency(){return opticsEfficiency;};     // For CCD

		//Gets
		double getDetEfficiency(double e,bool interpolate = false); // Gets efficiency for a fixed energy
		double getFocusArea(){return focusArea;};
		double getEinitial(){return Einitial;};
		double getEfinal(){return Efinal;};
		double getMeanEfficiency();

                // Function to print information
                void Show();

	//ClassDef (CASTDetector,1);
};
#endif
