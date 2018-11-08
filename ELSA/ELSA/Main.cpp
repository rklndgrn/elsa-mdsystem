#include "./Atom/Atom.h"
#include "./Material/Material.h"
#include "./Parameters/Parameters.h"
#include "./Cell/Cell.h"
#include "./Simulation/Simulation.h"
#include "./World/World.h"
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

int main()
{
	//Atom a{1, 1, 2, 3};
	//Atom* a1;
	//a1 = &a;
	//Atom b{ 2, 0 , 0, 0 };
	//Atom* b1;
	//b1 = &b;
	//Material anotherMaterial("BCC", 1, 1, 2, 4.5, 55);
	//Parameters myParameters(2, 1, 1, 100, 1, true, false, anotherMaterial);


	//World myWorld(myParameters);

	//
	//myWorld.addAtomToAtomList(&a);
	//myWorld.addAtomToAtomList(&b);
	//a.addToNeighbourList(&b);

	//myWorld.calcPotentialAndForce();

	//cout << myWorld.getAtomInAtomList(1)->getPotential() << endl;
	//cout << myWorld.getAtomInAtomList(0)->getPotential() << endl;
	//cout << myWorld.getAtomInAtomList(0)->getNeighbourList().at(0)->getPotential() << endl;


	//cout << myWorld.getAtomInAtomList(1) << endl;
	//cout << myWorld.getAtomInAtomList(0)->getNeighbourList().at(0) << endl;

	Material anotherMaterial("fcc", 2, 1, 1, 2, 100);
	Parameters myParameters(1, 1, 10, 10, 10, 10000, 10, false, false, anotherMaterial);


	World myWorld(myParameters);

	double Ken1{ 0 }, Ken2;

	ofstream myfile;
	myfile.open("erik.txt");

	for (unsigned int i = 0; i < myParameters.getNumberOfAtoms(); i++)
	{
	/*	cout << " id: " << myworld.getatominatomlist(i)->getid();
		cout <<
			" x: " << myworld.getatominatomlist(i)->getx() <<
			" y: " << myworld.getatominatomlist(i)->getvelocityy() <<
			" z: " << myworld.getatominatomlist(i)->getvelocityz() << endl;*/

		//cout << 0.5 * myParameters.getChosenMaterial().getMass() * (pow(myWorld.getAtomInAtomList(i)->getVelocityX(), 2) + pow(myWorld.getAtomInAtomList(i)->getVelocityX(), 2) + pow(myWorld.getAtomInAtomList(i)->getVelocityX(), 2)) << endl;

		Ken1 +=  0.5 * myParameters.getChosenMaterial().getMass() * (pow(myWorld.getAtomInAtomList(i)->getVelocityX(), 2) +pow(myWorld.getAtomInAtomList(i)->getVelocityY(), 2) +pow(myWorld.getAtomInAtomList(i)->getVelocityZ(), 2));
		
		myfile << myWorld.getAtomInAtomList(i)->getVelocityX() << endl;
		myfile << myWorld.getAtomInAtomList(i)->getVelocityY() << endl;
		myfile << myWorld.getAtomInAtomList(i)->getVelocityZ() << endl;

		cout << " nid: ";
		for (unsigned int j = 0; j < myWorld.getAtomInAtomList(i)->getNeighbourList().size(); j++)
		{
			cout << myWorld.getAtomInAtomList(i)->getNeighbourList()[j]->getID() << ", ";
		}
		cout << endl;
	}

	myfile.close();

	Ken1 = Ken1 / myParameters.getNumberOfAtoms();
	Ken2 = 1.5 * 1.38064852E-23 * myParameters.getTemperature();

	cout << "==========================" << endl;
	cout << "Ken1: " << Ken1 << " Ken2: " << Ken2 << endl;
	cout << "Ken2/Ken1 " << Ken2 / Ken1 << endl;


	cout << "====================" << endl;
	cout << "Total: " << myParameters.getNumberOfAtoms() << endl;


	char exit;

	cin >> exit;

	return 0;
}
