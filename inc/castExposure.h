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
		castExposure();
		~castExposure();

		double pressure;
		double timeExp;
		int gasType;
                double angle; // Will be considered later ***ANGLE

	//ClassDef(castExposure,1);

};
#endif
