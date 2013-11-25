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
#include<castExposure.h>
#include<castTracking.h>

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
                char outputPath[256];
		int ndetectors;
		bool gasTypes;
                double nGamma;      //nGamma. Calculated by GetNgammaCounts

                double maxg4;       //maximum of distribution. Calculated by GetMaxLike
                double sigmaLeft;   //Left Sigma. Calculated by GetMaxLike
                double sigmaRight;   //Left Sigma. Calculated by GetMaxLike

                // Fills missing vars of Tracking vector
                void FillTrackingVectors(double ma,vector<castTracking> vecTrk[]);

		//readGasTypes for combined analysis 2008+2010 gaps can change inside the likelihood the type of gas
                // For 2008, keep as 0
		void readGasTypes(bool ty){gasTypes=ty;};

                // return the total expected number of counts for a axion mass (eV), uses as input the exposure in a tree
                double GetNgammaCounts(double ma, const vector<castExposure> vecExp[]);

                //This function implements the likelihood for a given axion; trackings counts and exposure for one or more detectors
                double GetgL4(double ma, const vector<castExposure> vecExp[],const vector<castTracking> vecTrk[],bool writeToFile=false);

                //Dummy function to check different values of g vs chi
                double plot_gL4(double ma, const vector<castExposure> vecExp[],const vector<castTracking> vecTrk[],double *range);

                //This function calculates the minimun value for gL4 for a given axion mass
		//double GetMinLike(double ma,TTree *expTree[],TTree *tckTree[],TTree &gL4Tree, double g4,double &sigmaLeft,double &sigmaRight);

                //This function finds maximum likelihood and shape of the curve for sigmas 
                double GetMaxLike(double ma, const vector<castExposure> vecExp[],const vector<castTracking> vecTrk[], double g4max, bool writeToFile=false);

                //Integration over the likelihood for different gL
		double IntegrateChi( double *chi, int n );

                //return limit with a confidence level of 95%
		double GetLimit( double *chi, double integral );

                //Prints the information about the instance
                void Show();
                
                // Trial to make likelihood calculation a function, problem is passign too many objects to it. It would be better to have these objects as class members
                double CalculateLikelihood(double ma, double g4, double nGamma, const vector<castExposure> vecExp[],const vector<castTracking> vecTrk[] );

	//ClassDef(CASTLike,1)

};
#endif
