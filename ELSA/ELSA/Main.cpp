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
	Parameters myParameters(1e-12, 1e-11, 10, 10, 10, 10, 10, false, false, anotherMaterial);
	World myWorld(myParameters);

	double** potArray = myWorld.getResults().getPotentialEnergy();
	double** kinArray = myWorld.getResults().getKineticEnergy();
	double** tempArray = myWorld.getResults().getTemperature();
	double* U = *potArray;
	double* K = *kinArray;
	double* T = *tempArray;
	int index{ 0 };
	double deltaT = myParameters.getTimeStep();

	//array<double, 3> p = myWorld.getResults().getMomentumVector(index);

	//cout << "Time 0: " << endl;
	//cout << "   Potential energy: " << U[index] << endl;
	//cout << "   Kinetic energy: " << K[index] << endl;
	//cout << "   Temperature: " << T[index] << endl;
	//cout << "   Position of atom 555: (" << myWorld.getAtomInAtomList(555)->getPositionX() << ", " << myWorld.getAtomInAtomList(555)->getPositionY() << ", " << myWorld.getAtomInAtomList(555)->getPositionZ() << ")" << endl;
	//cout << "   Velocity of atom 555: (" << myWorld.getAtomInAtomList(555)->getVelocityX() << ", " << myWorld.getAtomInAtomList(555)->getVelocityY() << ", " << myWorld.getAtomInAtomList(555)->getVelocityZ() << ")" << endl;
	//cout << "   Total momentum: (" << p[0] << ", " << p[1] << ", " << p[2] << ")" << endl;
	//cout << "   Total energy: " << U[index] + K[index] << endl;

	
	for (double t = deltaT; t <= myParameters.getSimulationTime() - deltaT; t += deltaT)
	{
		if (t > deltaT)
		{
			myWorld.updateCells();
			myWorld.updateNeighbourList();
		}
		//myWorld.calcPotentialAndForce(t);
		myWorld.resetAllPotentialsAndForces();
		myWorld.solveEquationsOfMotion(t);
		myWorld.calcPressure(t);
		U = *potArray;
		K = *kinArray;
		//p = myWorld.getResults().getMomentumVector(index);
		//T = *tempArray;
		index = (int)round(t / deltaT);
		
		cout << "Time " << t << ": " << endl;
		//cout << "   Position of atom 555: (" << myWorld.getAtomInAtomList(555)->getPositionX() << ", " << myWorld.getAtomInAtomList(555)->getPositionY() << ", " << myWorld.getAtomInAtomList(555)->getPositionZ() << ")" << endl;
		//cout << "   Velocity of atom 555: (" << myWorld.getAtomInAtomList(555)->getVelocityX() << ", " << myWorld.getAtomInAtomList(555)->getVelocityY() << ", " << myWorld.getAtomInAtomList(555)->getVelocityZ() << ")" << endl;
		//cout << "   Acceleration of atom 555: (" << myWorld.getAtomInAtomList(555)->getAccelerationX() << ", " << myWorld.getAtomInAtomList(555)->getAccelerationY() << ", " << myWorld.getAtomInAtomList(555)->getAccelerationZ() << ")" << endl;
		//cout << "   Force on atom 555: (" << myWorld.getAtomInAtomList(555)->getForceX() << ", " << myWorld.getAtomInAtomList(555)->getForceY() << ", " << myWorld.getAtomInAtomList(555)->getForceZ() << ")" << endl;
		
		cout << "   Potential energy: " << U[index] << endl;
		cout << "   Kinetic energy: " << K[index] << endl;
		//cout << "   Total momentum: (" << p[0] << ", " << p[1] << ", " << p[2] << ")" << endl;
		cout << "   Total energy: " << U[index] + K[index] << endl;
		//cout << "   Temperature: " << T[index] << endl;
		
	}
	

	//cout << "Hello there!" << endl;

	char exit;

	cin >> exit;

	return 0;
}
