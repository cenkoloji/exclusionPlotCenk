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

		castTracking();
		~castTracking();

		double pressure;
		double energy;
		double bckLevel;
		int gasType;

	//ClassDef(CASTTracking,1);

};
#endif
