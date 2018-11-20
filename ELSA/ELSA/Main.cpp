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
	Material anotherMaterial("fcc", 408.53e-12, 0.34*(1.6021766208E-19), 2.65e-10, 1.75*408.53e-12, 39.948*(1.660539040e-27));
	Parameters myParameters(1e-15, 100e-15, 10, 10, 10, 100, 2e13, false, false, anotherMaterial);
	World myWorld(myParameters);

	double** energyArray = myWorld.getResults().getTotalEnergy();
	double** tempArray = myWorld.getResults().getTemperature();
	//double** cohArr = myWorld.getResults().getCohesiveEnergy();
	double** msdArr = myWorld.getResults().getMeanSquareDisplacement();
	double** debArr = myWorld.getResults().getDebyeTemperature();
	//double**** posArr = myWorld.getResults().getPositions();
	double* E = *energyArray;
	double* T = *tempArray;
	//double* cE = *cohArr;
	double* dT = *debArr;
	double* msd = *msdArr;
	//double*** pos = *posArr;
	int index{ 0 };
	double simulationTime = myParameters.getSimulationTime();
	double deltaT = myParameters.getTimeStep();
	double vx{ 0 }, vy{ 0 }, vz{ 0 };
	//int numberOfTimeSteps = (int)round(simulationTime / deltaT);
	Atom* a = myWorld.getAtomInAtomList((int)myParameters.getNumberOfAtoms() - 1);

	cout << "Time 0: " << endl;
	cout << "   Total energy: " << E[index] << endl;
	cout << "   Temperature: " << T[index] << endl;
	cout << "   Total velocity: (" << vx << ", " << vy << ", " << vz << ")!" << endl;
	cout << "   Last atom's velocity: (" << a->getVelocityX() << ", " << a->getVelocityY() << ", " << a->getVelocityZ() << ")!" << endl;
	cout << "   MSD: " << msd[index] << endl;
	cout << "   Debye Temperature: " << dT[index] << endl << endl;
	//cout << "   Position from atom: " << a->getPositionX() << " " << a->getPositionY() << " " << a->getPositionZ() << " " << endl;
	//cout << "   Position from list: " << pos[0][0][0] << " " << pos[0][0][1] << " " << pos[0][0][2] << endl;

	
	for (double t = deltaT; t < simulationTime - 0.5*deltaT; t += deltaT)
	{

		myWorld.performSimulation(t, 10);
		index = (int)round(t / deltaT);

		for (int i = 0; i < (int) myParameters.getNumberOfAtoms(); i++)
		{
			a = myWorld.getAtomInAtomList(i);
			vx += a->getVelocityX();
			vy += a->getVelocityY();
			vz += a->getVelocityZ();
		}


		
		cout << "Time " << t << ": " << endl;
		cout << "   Total energy: " << E[index] << endl;
		cout << "   Temperature: " << T[index] << endl;
		cout << "   Total velocity: (" << vx << ", " << vy << ", " << vz << ")!" << endl;
		cout << "   Last atom's velocity: (" << a->getVelocityX() << ", " << a->getVelocityY() << ", " << a->getVelocityZ() << ")!" << endl;
		cout << "   MSD: " << msd[index] << endl;
		cout << "   Debye Temperature: " << dT[index]<< endl << endl;
		//cout << "   Position from atom: " << a->getPositionX() << " " << a->getPositionY() << " " << a->getPositionZ() << " " << endl;
		//cout << "   Position from list: " << pos[index][0][0] << " " << pos[index][0][1] << " " << pos[index][0][2] << endl;


		vx = 0;
		vy = 0;
		vz = 0;
	}

	char exit;

	cin >> exit;

	return 0;
}
