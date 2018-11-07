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

	cout << myWorld.getAtomInAtomList(1) << endl;
	cout << myWorld.getAtomInAtomList(0)->getNeighbourList().at(0) << endl;

	char exit;

	cin >> exit;

	return 0;
}
