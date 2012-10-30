//Trial program to test castGas class using vectors

#include <iostream>
#include <fstream>
#include <vector>
#include "castExposure.h"
using std::cout;
using std::endl;

void printExposures(const vector<castExposure> v[]);

int main()
{
    char expFileName[256];
    sprintf(expFileName,"pressureExposureSunset1.txt");

    //Getting number of lines in the file (Not needed using vector)
    char command[256];
    sprintf(command,"wc -l %s",expFileName);
    int nExpLines;
    FILE *p = popen(command, "r");
    fscanf (p,"%d",&nExpLines);
    cout << nExpLines <<endl ;

    //creating array of vector of castExposure objects
    vector<castExposure> vecExposure[2];
    castExposure exp = castExposure();

    ifstream expFile1;           // create object to read
    expFile1.open(expFileName);  // associate with a file

    if (!expFile1.is_open())
    {
        cout << "Could not open the file " << expFileName << " -> Terminating..." << endl;
        exit(EXIT_FAILURE);
    }

    for (int i = 0; expFile1.good(); i++)
    {
        expFile1 >> exp.pressure >> exp.timeExp;
        vecExposure[0].push_back(exp);
    }

    expFile1.close();         // Closing the file


    sprintf(expFileName,"pressureExposureSunset2.txt");

    ifstream expFile2;           // create object to read
    expFile2.open(expFileName);  // associate with a file

    if (!expFile2.is_open())
    {
        cout << "Could not open the file " << expFileName << " -> Terminating..." << endl;
        exit(EXIT_FAILURE);
    }

    for (int i = 0; expFile2.good(); i++)
    {
        expFile2 >> exp.pressure >> exp.timeExp;
        vecExposure[1].push_back(exp);
    }

    expFile2.close();         // Closing the file

    // Example of passing vector array to function
    printExposures(vecExposure);

    return 0;
}

void printExposures(const vector<castExposure> v[])
{
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < v[i].size(); j++)
        {
            cout << v[i][j].pressure << "  " << v[i][j].timeExp << endl;
        }
    }
}
