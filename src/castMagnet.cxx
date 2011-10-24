
#include <iostream>
#include "castMagnet.h"
using namespace std;


void castMagnet::Show()
{

cout << "\nMagnet Information" << endl;
cout << "==================" << endl;
cout << "Magnetic Field: " << BMag << endl; 
cout << "Temperature: " << TMag << endl; 
cout << "Cross Section Area: " << areaCB << endl; 
cout << "Length : " << lenMag << endl; 
cout << "Number of Bores: " << nCB << endl; 

}
