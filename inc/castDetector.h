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
		double opticsEfficiency;//Optic Efficiency for the focus X-ray
		double focusArea;//Focus Area of the detector in cm2 (e.g. for mM is the area of the coldbore and for CCD is around 0.15 cm2)
		double detEfficiency[EFF_POINTS];//Efficiency array of the detector
		double Einitial;//Minimum energy used in analysis
		double Efinal;//Maximum energy used in analysis

	public:
		castDetector();
		~castDetector();

		//Sets
		void setOpticsEfficiency(double oEff){ opticsEfficiency= oEff;}; // CCD
		void setDetEfficiency(char *effFile,double softEff); // for MM
		void setCCDEfficiency(char *effFile); //CCD
		void setFocusArea(double fA){focusArea = fA;}; // CCD
		void setEinitial(double Ei){Einitial = Ei;};
		void setEfinal(double Ef){Efinal = Ef;};

		//Gets
		double getOpticsEfficiency(){return opticsEfficiency;};
		double getDetEfficiency(double e);
		double getFocusArea(){return focusArea;};
		double getEinitial(){return Einitial;};
		double getEfinal(){return Efinal;};
		double getMeanEfficiency();

	//ClassDef (CASTDetector,1);
};
#endif







