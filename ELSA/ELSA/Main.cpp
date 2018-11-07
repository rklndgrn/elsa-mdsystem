#include "./Atom/Atom.h"
#include "./Material/Material.h"
#include "./Parameters/Parameters.h"
#include "./Cell/Cell.h"
#include "./Simulation/Simulation.h"
#include <iostream>

using namespace std;

int main()
{
	Atom a{1, 1, 2, 3};
	Atom b{2, 0, 0, 0};

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

	Cell newCell;
	newCell.setCellID(1, 3, 4);

	newCell.addAtomToCellList( a );
	cout << newCell.getCellID()[0] << newCell.getCellID()[2] << newCell.getCellID()[1] << endl;

	Simulation mySim(anotherMaterial);
	cout << mySim.calcLJPotential(1e-10) << endl;

	array<double, 3> oldR = {0, 0, 0};
	array<double, 3> vel = {0.5, 0, 1};
	array<double, 3> acc = {0, 1, 0};

	array<double, 3> newR = mySim.calcPosition(oldR, vel, acc, 0.1);
	array<double, 3> newV = mySim.calcVelocity(vel, acc, 0.1);

	cout << "New position test: (" << newR[0] << ", " << newR[1] << ", " << newR[2] << ")!" << endl;
	cout << "New velocity test: (" << newV[0] << ", " << newV[1] << ", " << newV[2] << ")!" << endl;

	char exit;

	cin >> exit;

	return 0;
}
