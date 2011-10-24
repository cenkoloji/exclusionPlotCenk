/**********************************************************************
Definition of the exposure for CAST exclusion Plot

Author: Juan Antonio García
University of Zaragoza

Date: 7 Febr 2010
***********************************************************************/


#ifndef _CAST_EXPOSURE_
#define _CAST_EXPOSURE_

#ifndef _ROOT_TOBJECT_
#include <TObject.h>
#endif

using namespace std;

class CASTExposure : public TObject
{
	public:
	
		CASTExposure();
		~CASTExposure();
		
		Double_t pressure;
		Double_t timeExp;
		Int_t gasType;
		
	ClassDef(CASTExposure,1);

};
#endif
