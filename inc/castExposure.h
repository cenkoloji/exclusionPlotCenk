/**********************************************************************
Definition of exposure, which pressure, how long etc.
Based on a original codes of Javier Galan and Juan Antonio Garcia

Author: Cenk Yıldız
Bogazici University

Date: 24 Oct 2010
***********************************************************************/

#ifndef _CASTEXPOSURE_
#define _CASTEXPOSURE_

using namespace std;

class castExposure
{
	public:

// Header of exposure file
//countNo	Ntrack	N_step	LabviewTime	TimeExp	angle	Tmag_new	PCBcenter	dens_kg_m3

		castExposure();
		~castExposure();

                // These 4 are not really needed, but keep
                int countNo;
                int ntrack;
                int nstep;
                double labviewtime;

                // These are important
		double timeExp; //exposure time
                double angle;   // tilting angle
                double tmag;    // Tmagnet
		double pressure; //P-CB in the center of the cold bore
                double density; // kg/m3 -> Not really used (deprecated)
		int gasType;


};
#endif
