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
	Material anotherMaterial("fcc", 408.53e-12, 0.34*(1.6021766208E-19), 2.65e-10, 1.5*408.53e-12, 39.948*(1.660539040e-27));
	Parameters myParameters(1e-14, 200e-14, 5, 5, 5, 10, 10, false, false, anotherMaterial);
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
	ofstream myTempFile, myEnergyFile;
	myTempFile.open("temperatur.txt");
	myEnergyFile.open("energi.txt");
	//myFilePos.open("BengtPos.txt");
	


	cout << "Time 0: " << endl;
	//cout << "   Potential energy: " << U[index] << endl;
	//cout << "   Kinetic energy: " << K[index] << endl;
	cout << "   Total energy: " << U[index] + K[index] << endl;
	cout << "   Temperature: " << T[index] << endl;
	myTempFile << T[index] << " ";
	myEnergyFile << U[index] << " ";
	
	
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

		myWorld.calcPotentialAndForce(t);
		myWorld.solveEquationsOfMotion(t);
		myWorld.calcPressure(t);
		//U = *potArray;
		//K = *kinArray;
		//T = *tempArray;
		index = (int)round(t / deltaT);


		//myFilePos << a->getPositionX() << " " << a->getPositionY() << " " << a->getPositionZ() << " " << b->getPositionX() << " " << b->getPositionY() << " " << b->getPositionZ() << " ";
		
		
		cout << "Time " << t << ": " << endl;
		//cout << "   Potential energy: " << U[index] << endl;
		//cout << "   Kinetic energy: " << K[index] << endl;
		cout << "   Total energy: " << U[index] + K[index] << endl;
		cout << "   Temperature: " << T[index] << endl << endl;
		myTempFile << T[index] << " ";
		myEnergyFile << U[index] << " ";
	}
	
	//myFilePos.close();
	myTempFile.close();
	myEnergyFile.close();
	//cout << "Hello there!" << endl;

	char exit;

	cin >> exit;

	return 0;
}
