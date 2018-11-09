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
	Material anotherMaterial("sc", 2, 1, 1, 2.1, 100);
	Parameters myParameters(1, 1, 5, 5, 5, 10000, 10, false, true, anotherMaterial);


	World myWorld(myParameters);



	int numberOfNeighbours{ 0 };
	for (unsigned int i = 0; i < myParameters.getNumberOfAtoms(); i++)
	{
		numberOfNeighbours = 0;
		cout << " id: " << myWorld.getAtomInAtomList(i)->getID();

		cout << " nid: ";
		for (unsigned int j = 0; j < myWorld.getAtomInAtomList(i)->getNeighbourList().size(); j++)
		{
			cout << myWorld.getAtomInAtomList(i)->getNeighbourList()[j]->getID() << ", ";
			numberOfNeighbours++;

		}
		cout << "number: "<< numberOfNeighbours;
		cout << endl;
	}





	cout << "====================" << endl;
	cout << "Total: " << myParameters.getNumberOfAtoms() << endl;
	
	char exit;

	cin >> exit;

	return 0;
}
