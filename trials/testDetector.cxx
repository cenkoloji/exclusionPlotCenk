//Trial program to test castGas class

#include <iostream>
#include "castDetector.h"
using std::cout;
using std::endl;

int main()
{
    castDetector * cd = new castDetector();

    char efffilename[128] = "mMEfficiency.txt";
    char softefffilename[128] = "mMSoftEfficiency1.txt";
    cd->setDetEfficiency(efffilename,softefffilename);

    cd->setEinitial(2.0);
    cd->setEfinal(7.0);

    cout << "\n========= 2 - 7 eV =============" << endl;
    cout << "Ei: " << cd->getEinitial() << endl;
    cout << "Ef: " << cd->getEfinal()   << endl;
    cout << "Eff at 1.3keV: " << cd->getDetEfficiency(1.3,true) << endl;
    cout << "Mean Eff: " << cd->getMeanEfficiency() << endl;

    cd->setEinitial(2.0);
    cd->setEfinal(7.0);

    cout << "\n========= 1 - 9 keV =============" << endl;
    cout << "Ei: " << cd->getEinitial() << endl;
    cout << "Ef: " << cd->getEfinal()   << endl;

    for (int i = 0; i < 20; i++)
    {
       cout << "Eff at " << 2+i*0.05 << "keV: " << cd->getDetEfficiency(2+i*0.05,false) << endl;
    }

    cout << "Mean Eff: " << cd->getMeanEfficiency() << endl;

    return 0;
}
