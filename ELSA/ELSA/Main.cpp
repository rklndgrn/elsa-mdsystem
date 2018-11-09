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
	Material anotherMaterial("sc", 2, 1, 1, 2, 100);
	Parameters myParameters(1, 1, 3, 3, 3, 10000, 10, false, false, anotherMaterial);


	World myWorld(myParameters);

	double Ken1{ 0 }, Ken2;

	
	for (unsigned int i = 0; i < myParameters.getNumberOfAtoms(); i++)
	{
		cout << " id: " << myWorld.getAtomInAtomList(i)->getID();
		cout << " nid: ";
		for (unsigned int j = 0; j < myWorld.getAtomInAtomList(i)->getNeighbourList().size(); j++)
		{
			cout << myWorld.getAtomInAtomList(i)->getNeighbourList()[j]->getID() << ", ";
		}
		cout << endl;
	}





	cout << "====================" << endl;
	cout << "Total: " << myParameters.getNumberOfAtoms() << endl;
	
	char exit;

	cin >> exit;

	return 0;
}
