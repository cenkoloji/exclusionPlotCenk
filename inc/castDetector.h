/**********************************************************************
Definition of a detector for CAST exclusion Plot
Based on a original code of Javier Galan

Author: Juan Antonio García
University of Zaragoza

Date: 7 Febr 2010
***********************************************************************/


#ifndef _CAST_DETECTOR_
#define _CAST_DETECTOR_

#ifndef _ROOT_TOBJECT_
#include <TObject.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define EFF_POINTS 60

using namespace std;

class CASTDetector : public TObject
{
	public:
		
		CASTDetector();
		~CASTDetector();
		
		Double_t opticsEfficiency;//Optic Efficiency for the focus X-ray
		Double_t focusArea;//Focus Area of the detector in cm2 (e.g. for mM is the area of the coldbore and for CCD is around 0.15 cm2)
		Double_t detEfficiency[EFF_POINTS];//Efficiency of the detector
		Double_t Einitial;//Theshold energy of the detector
		Double_t Efinal;//Final energy for the analysis
		
		
		//Sets
		void setOpticsEfficiency(Double_t oEff){ opticsEfficiency= oEff;}; // CCD
		void setDetEfficiency(Double_t eff); // Obsolete
		void setDetEfficiency(char *effFile,Double_t softEff); // for MM
		void setCCDEfficiency(char *effFile); //CCD
		void setFocusArea(Double_t fA){focusArea = fA;}; // CCD
		void setEinitial(Double_t Ei){Einitial = Ei;};
		void setEfinal(Double_t Ef){Efinal = Ef;};
		
		
		//Gets
		Double_t getOpticsEfficiency(){return opticsEfficiency;};
		Double_t getDetEfficiency(Double_t e);
		Double_t getFocusArea(){return focusArea;};
		Double_t getEinitial(){return Einitial;};
		Double_t getEfinal(){return Efinal;};
		Double_t getMeanEfficiency();

	ClassDef (CASTDetector,1);
};
#endif







