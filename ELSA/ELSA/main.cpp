#include "./Atom/Atom.h"
#include "./Material/Material.h"
#include "./Parameters/Parameters.h"
#include "./Results/Results.h"
#include <iostream>

using namespace std;

int main()
{
	Atom a{1, 1, 2, 3};

	cout << "Hej, vi har en atom med ID " << a.getID() << ", position (" << a.getPosX() << ", " << a.getPosY() << ", " << a.getPosZ() << ") och hastighet (" << a.getVelocityX() << ", " << a.getVelocityY() << ", " << a.getVelocityZ() << ")!" << endl;


	a.setPosition(6, 2, 3);
	a.setVelocity(5, 3, 2);

	cout << "Fast nu har vi en atom med ID " << a.getID() << ", position (" << a.getPosX() << ", " << a.getPosY() << ", " << a.getPosZ() << ") och hastighet (" << a.getVelocityX() << ", " << a.getVelocityY() << ", " << a.getVelocityZ() << ")!" << endl;
	
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

	Results myResults{4, 2, 2}; //Create a 2x2x3 array.
	myResults.setPositions(0, 0, 0, 0, 0);
	myResults.setPositions(5, 5, 5, 0, 1);
	myResults.setPositions(2, 3, 2, 1, 0);
	myResults.setPositions(1, 2, 1, 1, 1);

	double**** r = myResults.getPositions();

	myResults.prettyPrint();

	char exit;

	cin >> exit;

	return 0;
}