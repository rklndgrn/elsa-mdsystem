#include "material.h"
#include "parameters.h"
#include <iostream>

using namespace std;

int main()
{ 
	Material myMaterial;
	Parameters myParameters;

	myMaterial.setCrystalStructure("FCC");
	myMaterial.setEpsilon(3.1415926536);

	myParameters.setIsThermostatOn(true);
	myParameters.setChosenMaterial(myMaterial);
	myParameters.setCollisionFrequency(10.05);

	cout << myParameters.getIsThermostatOn() << endl;
	cout << myParameters.getCollisionFrequency() << endl;

	cout << myParameters.getChosenMaterial().getCrystalStructure() << endl;
	cout << myParameters.getChosenMaterial().getEpsilon() << endl;

	Material anotherMaterial("BCC", 4.5, 1.2, 2.3, 4.5, 55);
	Parameters anotherParameters(100, 10, 1000, 4.56, 1.00, true, true, anotherMaterial);

	cout << anotherParameters.getIsThermostatOn() << endl;
	cout << anotherParameters.getCollisionFrequency() << endl;

	cout << anotherParameters.getChosenMaterial().getCrystalStructure() << endl;
	cout << anotherParameters.getChosenMaterial().getEpsilon() << endl;


	char exit;
	cin >> exit;

	return 0; 
}
