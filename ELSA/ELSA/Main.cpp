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
	//Material(crystalStructure, latticeConstant, epsilon, sigma, cutOffDistance, mass)
	Material anotherMaterial("fcc", 408.53e-12, 0.34*(1.6021766208E-19), 2.65e-10, 7e-10, 107.8682*(1.660539040e-27));
	//Parameters(timeStep, simulationTime, numberOfUnitCellsX, numberOfUnitCellsY, numberOfUnitCellsZ,
	//			temperature, collisionFrequency, isThermostatOn, is2D, chosenMaterial)
	Parameters myParameters(1e-12, 1e-10, 10, 10, 10, 10, 10, false, false, anotherMaterial);
	World myWorld(myParameters);

	double** potArray = myWorld.getResults().getPotentialEnergy();
	double** kinArray = myWorld.getResults().getKineticEnergy();
	double** tempArray = myWorld.getResults().getTemperature();
	double* U = *potArray;
	double* K = *kinArray;
	double* T = *tempArray;
	int index{ 0 };
	double deltaT = myParameters.getTimeStep();

	cout << "Time 0: " << endl;
	//cout << "   Velocity of atom 0: (" << myWorld.getAtomInAtomList(0)->getVelocityX() << ", " << myWorld.getAtomInAtomList(0)->getVelocityY() << ", " << myWorld.getAtomInAtomList(0)->getVelocityZ() << ")" << endl;
	cout << "   Potential energy: " << U[index] << endl;
	cout << "   Kinetic energy: " << K[index] << endl;
	//cout << "   Temperature: " << T[index] << endl;
	
	for (double t = deltaT; t <= myParameters.getSimulationTime() - deltaT; t += deltaT)
	{
		myWorld.calcPotentialAndForce(t);
		myWorld.solveEquationsOfMotion(t);
		myWorld.calcPressure(t);
		U = *potArray;
		K = *kinArray;
		T = *tempArray;
		index = (int)round(t / deltaT);
		
		cout << "Time " << t << ": " << endl;
		
		cout << "   Potential energy: " << U[index] << endl;
		cout << "   Kinetic energy: " << K[index] << endl;
		cout << "   Total energy: " << U[index] + K[index] << endl;
		//cout << "   Temperature: " << T[index] << endl;
		
		//cout << "   Velocity of atom 0: (" << myWorld.getAtomInAtomList(0)->getVelocityX() << ", " << myWorld.getAtomInAtomList(0)->getVelocityY() << ", " << myWorld.getAtomInAtomList(0)->getVelocityZ() << ")" << endl;
	}
	

	//cout << "Hello there!" << endl;

	char exit;

	cin >> exit;

	return 0;
}
