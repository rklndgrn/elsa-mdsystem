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
	Parameters myParameters(2, 1, 20, 100, 0.01, true, false, anotherMaterial);


	World myWorld(myParameters);

	
	myWorld.addAtomToAtomList(&a);
	myWorld.addAtomToAtomList(&b);
	a.addToNeighbourList(&b);

	//myWorld.calcPotentialAndForce();

	//cout << myWorld.getAtomInAtomList(1)->getPotential() << endl;
	//cout << myWorld.getAtomInAtomList(0)->getPotential() << endl;
	//cout << myWorld.getAtomInAtomList(0)->getNeighbourList().at(0)->getPotential() << endl;
	Material myMaterial;

	myMaterial.setCrystalStructure("FCC");
	myMaterial.setEpsilon(3.1415926536);

	myParameters.setIsThermostatOn(true);
	myParameters.setChosenMaterial(myMaterial);
	myParameters.setCollisionFrequency(0.01); 

	/*/
	cout << myParameters.getIsThermostatOn() << endl;
	cout << myParameters.getCollisionFrequency() << endl;

	cout << myParameters.getChosenMaterial().getCrystalStructure() << endl;
	cout << myParameters.getChosenMaterial().getEpsilon() << endl;
	*/

	Parameters anotherParameters(100, 10, 1000, 4.56, 1.00, true, true, anotherMaterial);

	/*
	cout << anotherParameters.getIsThermostatOn() << endl;
	cout << anotherParameters.getCollisionFrequency() << endl;

	cout << anotherParameters.getChosenMaterial().getCrystalStructure() << endl;
	cout << anotherParameters.getChosenMaterial().getEpsilon() << endl;
	*/

	Cell newCell;
	newCell.setCellID(1, 3, 4);

	newCell.addAtomToCellList( a );
	//cout << newCell.getCellID()[0] << newCell.getCellID()[2] << newCell.getCellID()[1] << endl;

	Simulation mySim(anotherMaterial);
	//cout << mySim.calcLJPotential(1e-10) << endl;


	//cout << myWorld.getAtomInAtomList(1) << endl;
	//cout << myWorld.getAtomInAtomList(0)->getNeighbourList().at(0) << endl;


	/*
	array<double, 3> randV;
	
	for (int i = 1; i < 5; i++)
	{
		randV = mySim.generateGaussianVelocity(9);
		cout << "Random velocity test " << i << ": (" << randV[0] << ", " << randV[1] << ", " << randV[2] << ")!" << endl;
	}
	*/

	/*

	cout << "STARTING SIMULATIONS FOR E.O.M..." << endl;
	cout << "   Starting positions are" << endl;
	cout << "      Atom 0 (x, y, z):" << " (" << myWorld.getAtomInAtomList(0)->getPosX() << ", " << myWorld.getAtomInAtomList(0)->getPosY() << ", " << myWorld.getAtomInAtomList(0)->getPosZ() << ")" << endl;
	cout << "      Atom 1 (x, y, z):" << " (" << myWorld.getAtomInAtomList(1)->getPosX() << ", " << myWorld.getAtomInAtomList(0)->getPosY() << ", " << myWorld.getAtomInAtomList(0)->getPosZ() << ")" << endl;

	cout << "   Starting velocities are" << endl;
	cout << "      Atom 0 (x, y, z):" << " (" << myWorld.getAtomInAtomList(0)->getVelocityX() << ", " << myWorld.getAtomInAtomList(0)->getVelocityY() << ", " << myWorld.getAtomInAtomList(0)->getVelocityZ() << ")" << endl;
	cout << "      Atom 1 (x, y, z):" << " (" << myWorld.getAtomInAtomList(1)->getVelocityX() << ", " << myWorld.getAtomInAtomList(0)->getVelocityY() << ", " << myWorld.getAtomInAtomList(0)->getVelocityZ() << ")" << endl;

	for (int i = 1; i < 10; i++)
	{
		if (i == 2)
		{
			a.setVelocityX(0.25e-10);
			a.setVelocityZ(0.5e-10);
		}

		myWorld.solveEquationsOfMotion(i);

		cout << "ELAPSED TIME: " << i << endl;
		cout << "   POSITIONS" << endl;
		cout << "      Atom 0 (x, y, z):" << " (" << myWorld.getAtomInAtomList(0)->getPosX() << ", " << myWorld.getAtomInAtomList(0)->getPosY() << ", " << myWorld.getAtomInAtomList(0)->getPosZ() << ")" << endl;
		cout << "      Atom 1 (x, y, z):" << " (" << myWorld.getAtomInAtomList(1)->getPosX() << ", " << myWorld.getAtomInAtomList(0)->getPosY() << ", " << myWorld.getAtomInAtomList(0)->getPosZ() << ")" << endl;

		cout << "   VELOCITIES" << endl;
		cout << "      Atom 0 (x, y, z):" << " (" << myWorld.getAtomInAtomList(0)->getVelocityX() << ", " << myWorld.getAtomInAtomList(0)->getVelocityY() << ", " << myWorld.getAtomInAtomList(0)->getVelocityZ() << ")" << endl;
		cout << "      Atom 1 (x, y, z):" << " (" << myWorld.getAtomInAtomList(1)->getVelocityX() << ", " << myWorld.getAtomInAtomList(0)->getVelocityY() << ", " << myWorld.getAtomInAtomList(0)->getVelocityZ() << ")" << endl;
	}
	*/

	array<double, 3> rc1 = { 0, 0, 0 };
	array<double, 3> rc2 = { 1, 1, 1 };
	array<double, 3> ri1 = { 0, 0, -1 };
	array<double, 3> ri2 = { 1, 0, 1 };

	double** cP;
	double** iP;

	cP = new double*[2];
	iP = new double*[2];

	for (int i{ 0 }; i < 2; i++)
	{
		cP[i] = new double[3];
		iP[i] = new double[3];
	}

	for (int i{ 0 }; i < 3; i++)
	{
		cP[0][i] = rc1[i];
		cP[1][i] = rc2[i];
		iP[0][i] = ri1[i];
		iP[1][i] = ri2[i];
	}

	double MSD = mySim.calcMeanSquareDisplacement(cP, iP, 2);

	cout << "MSD is " << MSD << "!" << endl;

	char exit;

	cin >> exit;

	return 0;
}
