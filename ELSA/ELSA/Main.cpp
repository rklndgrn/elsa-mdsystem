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
	Material anotherMaterial("fcc", 408.53e-12, 0.34*(1.6021766208E-19), 2.65e-10, 4*408.53e-12, 39.948*(1.660539040e-27));
	Parameters myParameters(1e-15, 1e-14, 5, 5, 5, 10, 10, false, true, anotherMaterial);
	World myWorld(myParameters);

	double** potArray = myWorld.getResults().getPotentialEnergy();
	double** kinArray = myWorld.getResults().getKineticEnergy();
	double** energyArray = myWorld.getResults().getTotalEnergy();
	double** tempArray = myWorld.getResults().getTemperature();
	double* U = *potArray;
	double* K = *kinArray;
	double* E = *energyArray;
	double* T = *tempArray;
	int index{ 0 };
	double deltaT = myParameters.getTimeStep();

	
	//ofstream myFilePos;
	/*
	ofstream myTempFile, myEnergyFile, myPosFile;
	myTempFile.open("temperatur.txt");
	myEnergyFile.open("energi.txt");
	myPosFile.open("positions.txt");
	*/
	
	Atom* a = myWorld.getAtomInAtomList(0);

	cout << "Time 0: " << endl;
	//cout << "   Potential energy: " << U[index] << endl;
	//cout << "   Kinetic energy: " << K[index] << endl;
	cout << "   Total energy: " << U[index] + K[index] << endl;
	cout << "   Temperature: " << T[index] << endl;
	//cout << "   Position for atom 0: " << a->getPositionX() << " " << a->getPositionY() << " " << a->getPositionZ() << endl << endl;;
	/*
	myTempFile << T[index] << " ";
	myEnergyFile << U[index] << " ";
	myPosFile << a->getPositionX() << " " << a->getPositionY() << " " << a->getPositionZ() << " ";
	*/
	
	
	//Atom* a = myWorld.getAtomInAtomList(62);
	//Atom* b = myWorld.getAtomInAtomList(63);
	//myFilePos << a->getPositionX() << " " << a->getPositionY() << " " << a->getPositionZ() << " " << b->getPositionX() << " " << b->getPositionY() << " " << b->getPositionZ() << " ";

	
	for (double t = deltaT; t < myParameters.getSimulationTime() - 0.5*deltaT; t += deltaT)
	{
		if (deltaT > 0)
		{
			myWorld.updateCells();
			myWorld.updateNeighbourList();
		}

		myWorld.performSimulation(t);
		//U = *potArray;
		//K = *kinArray;
		//T = *tempArray;
		index = (int)round(t / deltaT);


		//myFilePos << a->getPositionX() << " " << a->getPositionY() << " " << a->getPositionZ() << " " << b->getPositionX() << " " << b->getPositionY() << " " << b->getPositionZ() << " ";
		
		
		cout << "Time " << t << ": " << endl;
		//cout << "   Potential energy: " << U[index] << endl;
		//cout << "   Kinetic energy: " << K[index] << endl;
		cout << "   Total energy: " << U[index] + K[index] << endl;
		cout << "   Temperature: " << T[index] << endl;
		//cout << "   Position for atom 0: " << a->getPositionX() << " " << a->getPositionY() << " " << a->getPositionZ() << endl << endl;;
		//myTempFile << T[index] << " ";
		//myEnergyFile << U[index] << " ";
		//myPosFile << a->getPositionX() << " " << a->getPositionY() << " " << a->getPositionZ() << " ";
	}
	
	//myFilePos.close();
	//myTempFile.close();
	//myEnergyFile.close();
	//myPosFile.close();
	//cout << "Hello there!" << endl;

	char exit;

	cin >> exit;

	return 0;
}
