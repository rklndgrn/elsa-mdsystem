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
	Parameters myParameters(1e-15, 10e-14, 10, 10, 10, 10, 2e13, false, false, anotherMaterial);
	World myWorld(myParameters);

	double** energyArray = myWorld.getResults().getTotalEnergy();
	double** tempArray = myWorld.getResults().getTemperature();
	double** cohArr = myWorld.getResults().getCohesiveEnergy();
	double* E = *energyArray;
	double* T = *tempArray;
	double* cE = *cohArr;
	int index{ 0 };
	double simulationTime = myParameters.getSimulationTime();
	double deltaT = myParameters.getTimeStep();
	Atom* a = myWorld.getAtomInAtomList(0);

	cout << "Time 0: " << endl;
	cout << "   Total energy: " << E[index] << endl;
	cout << "   Temperature: " << T[index] << endl;
	

	
	for (double t = deltaT; t < simulationTime - 0.5*deltaT; t += deltaT)
	{

		myWorld.performSimulation(t, 10);
		index = (int)round(t / deltaT);


		cout << "Time " << t << ": " << endl;
		cout << "   Total energy: " << E[index] << endl;
		cout << "   Temperature: " << T[index] << endl;
	}

	double cohesiveEnergy = cE[99];

	cout << "Cohesive energy: " << cohesiveEnergy << " eV!" << endl;

	char exit;

	cin >> exit;

	return 0;
}
