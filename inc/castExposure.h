/**********************************************************************
Definition of exposure, which pressure, how long etc. Its just used to create a root file.
Not too usefull if you won't use ROOT
Based on a original codes of Javier Galan and Juan Antonio Garcia

Author: Cenk Yıldız
Bogazici University

Date: 24 Oct 2010
***********************************************************************/

#ifndef _CASTEXPOSURE_
#define _CASTEXPOSURE_

/*
#ifndef _ROOT_TOBJECT_
#include <TObject.h>
#endif
*/

using namespace std;

class castExposure //: public TObject
{
	public:

// Header of exposure file
//countNo	Ntrack	N_step	LabviewTime	TimeExp	angle	Tmag_new	PCBcenter	dens_kg_m3

		castExposure();
		~castExposure();

                // These 4 are not really needed
                int countNo;
                int ntrack;
                int nstep;
                double labviewtime;

                // These are important
		double timeExp;
                double angle; // Will be considered later ***ANGLE
                double tmag;
		double pressure; //P-CB in the center of the cold bore
                double density; // kg/m3
		int gasType;


	//ClassDef(castExposure,1);

};
#endif
