/**********************************************************************
Definition of the Tracking for CAST exclusion Plot
Based on code of Juan Antonio Garcia

Author: Cenk Yildiz
Bogazici University

Date: 28 Oct 2012
***********************************************************************/


#ifndef _CAST_TRACKING_
#define _CAST_TRACKING_

using namespace std;

class castTracking
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
		double pressure; //pressure in the center of the cold bore, or at PCB (depending on how the file is prepared)
		int gasType;

};
#endif
