/**********************************************************************
Definition of exposure, which pressure, how long?
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
		castExposure();
		~castExposure();

		double pressure;
		double timeExp;
		int gasType;
                double angle; // Will be considered later

	//ClassDef(castExposure,1);

};
#endif
