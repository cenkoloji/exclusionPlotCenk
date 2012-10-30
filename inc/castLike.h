/**********************************************************************
This library implements the likelihood for CAST exclusion plot
Based on a original codes of Javier Galan and Juanan Gracia

Author: Cenk Yıldız
Bogazici University

Date: 27 Oct 2011
***********************************************************************/


#ifndef _CAST_LIKE_
#define _CAST_LIKE_

/*
#ifndef _ROOT_TOBJECT_
#include <TObject.h>
#endif
*/

#define MAX_ITERATIONS 100000
#define CONVERGENCE 5000
#define MAX_DETECTORS 12

#include<castDetector.h>
#include<castConversion.h>
#include<castMagnet.h>
#include<castGas.h>
//#include<castUtils.h>
#include<castExposure.h>
#include<castTracking.h>
//#include<TTree.h>
//#include<TBranch.h>
//#include<TGraph.h>

using namespace std;

class castLike// : public TObject
{

	public:

		//Constructor with definitions of detector, conversion, magnet and gas
                castLike(castConversion *cconv, castMagnet *cmag, castGas *cgas, castDetector *cdet[], int ndet);

		~castLike();

		castDetector *det[MAX_DETECTORS];
		castConversion *conv;
		castMagnet *mag;
		castGas *gas;
		int ndetectors;
		bool gasTypes;

		//readGasTypes for combined analysis 2008+2010 gaps can change inside the likelihood the type of gas
                // For 2008, keep as 0
		void readGasTypes(bool ty){gasTypes=ty;};

                // return the total expected number of counts for a axion mass (eV), uses as input the exposure in a tree
                double GetNgammaCounts(double ma, const vector<castExposure> vecExp[]);

                //This function implements the likelihood for a given axion; trackings counts and exposure for one or more detectors
                double GetgL4(double ma, const vector<castExposure> vecExp[],const vector<castTracking> vecTrk[],bool writeToFile=false);

                //Dummy function to check different values of g vs chi
                double GetgL4_Dummy(double ma, const vector<castExposure> vecExp[],const vector<castTracking> vecTrk[]);

                //This function calculates the minimun value for gL4 for a given axion mass
		//double GetMinLike(double ma,TTree *expTree[],TTree *tckTree[],TTree &gL4Tree, double g4,double &sigmaLeft,double &sigmaRight);

                //Integration over the likelihood for different gL
		double IntegrateChi( double *chi, int n );

                //return limit with a confidence level of 95%
		double GetLimit( double *chi, double integral );

                //Prints the information about the instance
                void Show();

	//ClassDef(CASTLike,1)

};
#endif
