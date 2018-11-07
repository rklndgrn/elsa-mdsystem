#include "./Atom/Atom.h"
#include "./Material/Material.h"
#include "./Parameters/Parameters.h"
#include "./Cell/Cell.h"
#include "./Simulation/Simulation.h"
#include "./World/World.h"
#include <iostream>

using namespace std;

int main()
{
	Atom a{1, 1, 2, 3};
	Atom* a1;
	a1 = &a;
	Atom b{ 2, 0 , 0, 0 };
	Atom* b1;
	b1 = &b;
	Material anotherMaterial("BCC", 1, 1, 2, 4.5, 55);
	Parameters myParameters(2, 1, 1, 100, 1, true, false, anotherMaterial);


	World myWorld(myParameters);

	
	myWorld.addAtomToAtomList(&a);
	myWorld.addAtomToAtomList(&b);
	a.addToNeighbourList(&b);

	myWorld.calcPotentialAndForce();

	cout << myWorld.getAtomInAtomList(1)->getPotential() << endl;
	cout << myWorld.getAtomInAtomList(0)->getPotential() << endl;
	cout << myWorld.getAtomInAtomList(0)->getNeighbourList().at(0)->getPotential() << endl;
	Material myMaterial;

	myMaterial.setCrystalStructure("FCC");
	myMaterial.setEpsilon(3.1415926536);

	myParameters.setIsThermostatOn(true);
	myParameters.setChosenMaterial(myMaterial);
	myParameters.setCollisionFrequency(10.05);

	cout << myParameters.getIsThermostatOn() << endl;
	cout << myParameters.getCollisionFrequency() << endl;

	cout << myParameters.getChosenMaterial().getCrystalStructure() << endl;
	cout << myParameters.getChosenMaterial().getEpsilon() << endl;

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


	cout << myWorld.getAtomInAtomList(1) << endl;
	cout << myWorld.getAtomInAtomList(0)->getNeighbourList().at(0) << endl;


	
	array<double, 3> randV;
	
	for (int i = 1; i < 5; i++)
	{
		randV = mySim.generateGaussianVelocity(9);
		cout << "Random velocity test " << i << ": (" << randV[0] << ", " << randV[1] << ", " << randV[2] << ")!" << endl;
	}

	array<double, 3> vel = {2, 0, 1 };
	double K = mySim.calcKineticEnergy(vel);
	double T = mySim.calcTemperature(K, anotherParameters.getBoltzmann(), 100);

	cout << "Kinetic energy test: " << K << "J!" << endl;
	cout << "Temperature test: " << T << " K!" << endl;

	char exit;

	cin >> exit;

	return 0;
}
