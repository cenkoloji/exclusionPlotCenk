/**********************************************************************
Definition of the Tracking for CAST exclusion Plot
Based on code of Juan Antonio Garcia

Author: Cenk Yildiz
Bogazici University

Date: 28 Oct 2012
***********************************************************************/


#ifndef _CAST_TRACKING_
#define _CAST_TRACKING_

//#ifndef _ROOT_TOBJECT_
//#include <TObject.h>
//#endif

using namespace std;

class castTracking// : public TObject
{
	public:

//Header of tracking info file
//timestamp, energy, density, bcklevel, angle, tmag, pcb

		castTracking();
		~castTracking();

		double timestamp;
		double energy;
                double density; // kg/m3
		double bckLevel;
                double angle;
                double tmag;
		double pressure; //P-CB in the center of the cold bore
		int gasType;

	//ClassDef(CASTTracking,1);

};
#endif
