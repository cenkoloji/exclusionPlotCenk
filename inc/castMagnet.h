/**********************************************************************
 Magnet properties
 Based on a original codes of Javier Galan and Juan Antonio Garcia

 Author: Cenk Yildiz
 Bogazici University

 Date: 20 Oct 2011
 ************************************************************************/

#ifndef _CASTMAGNET_
#define _CASTMAGNET_

/*
#ifndef _ROOT_TOBJECT_
#include <TObject.h>
#endif
*/

using namespace std;

class castMagnet //: public TObject
{
    private:
        double areaCB;      // Area of a cold bore(cm2)
        double TMag;        // Magnet Temperature
        double lenMag;      // Length of magnetic field(m)
        int nCB;            // Number of cold bores
        double BMag;        // Magnetic Field(T)

    public:
        castMagnet(double area = 14.5, double tmag = 1.8, double len = 9.26, int n = 1, double bmag = 8.802)
                   :areaCB(area), TMag(tmag),lenMag(len), nCB(n),BMag(bmag){};
        ~castMagnet(){};

        //Sets
        void setAreaCB(double area = 14.55){areaCB = area;}; // Sets the CB area
        void setTMag(double tmag = 1.8){TMag = tmag;};       // Sets the magnetic field
        void setLenMag(double len = 9.26){lenMag = len;};    // Sets the magnet lengt
        void setNCB(int n = 1){nCB = n;};                    // Sets the number of CB
        void setBMag(double bmag = 8.8){BMag = bmag;};       // Sets the Magnetic Field

        //Gets
        double getAreaCB(){return areaCB;};             // Gets the CB area
        double getTotalAreaCB(){return areaCB*nCB;};    // Gets the total area 
        double getTMag(){return TMag;};                 // Gets the magnetic field
        double getLenMag(){return lenMag;};             // Gets the magnet lengt
        int getNCB(){return nCB;};                      // Gets the number of CB
        double getBMag(){return BMag;};                 // Gets the Magnetic Field

        // Function to print information
        void Show();

};
#endif
