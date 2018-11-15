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
	Material anotherMaterial("fcc", 408.53e-12, 0.34*(1.6021766208E-19), 2.65e-10, 1.5*408.53e-12, 39.948*(1.660539040e-27));
	//Parameters(timeStep, simulationTime, numberOfUnitCellsX, numberOfUnitCellsY, numberOfUnitCellsZ,
	//			temperature, collisionFrequency, isThermostatOn, is2D, chosenMaterial)
	Parameters myParameters(1e-14, 200e-14, 5, 5, 5, 10, 10, false, false, anotherMaterial);
	World myWorld(myParameters);

	double** potArray = myWorld.getResults().getPotentialEnergy();
	double** kinArray = myWorld.getResults().getKineticEnergy();
	double** tempArray = myWorld.getResults().getTemperature();
	double* U = *potArray;
	double* K = *kinArray;
	double* T = *tempArray;
	int index{ 0 };
	double deltaT = myParameters.getTimeStep();

	
	//ofstream myFileP, myFileV;
	ofstream myFilePos;
	myFilePos.open("BengtPos.txt");
	//myFileP.open("BengtP.txt");
	//myFileV.open("BengtV.txt");
	

	//array<double, 3> p = myWorld.getResults().getMomentumVector(index);

	cout << "Time 0: " << endl;
	//cout << "   Position of atom 555: (" << myWorld.getAtomInAtomList(555)->getPositionX() << ", " << myWorld.getAtomInAtomList(555)->getPositionY() << ", " << myWorld.getAtomInAtomList(555)->getPositionZ() << ")" << endl;
	//cout << "   Velocity of atom 555: (" << myWorld.getAtomInAtomList(555)->getVelocityX() << ", " << myWorld.getAtomInAtomList(555)->getVelocityY() << ", " << myWorld.getAtomInAtomList(555)->getVelocityZ() << ")" << endl;
	//cout << "   Magnitude of velocity of atom 555: " << sqrt(pow(myWorld.getAtomInAtomList(555)->getVelocityX(), 2) + pow(myWorld.getAtomInAtomList(555)->getVelocityY(), 2) + pow(myWorld.getAtomInAtomList(555)->getVelocityZ(), 2)) << "!" << endl;
	//cout << "   Acceleration of atom 555: (" << myWorld.getAtomInAtomList(555)->getAccelerationX() << ", " << myWorld.getAtomInAtomList(555)->getAccelerationY() << ", " << myWorld.getAtomInAtomList(555)->getAccelerationZ() << ")" << endl;
	//cout << "   Potential of atom 555: " << myWorld.getAtomInAtomList(555)->getPotential() << endl << endl;;
	//cout << "   Position of atom 556: (" << myWorld.getAtomInAtomList(556)->getPositionX() << ", " << myWorld.getAtomInAtomList(556)->getPositionY() << ", " << myWorld.getAtomInAtomList(556)->getPositionZ() << ")" << endl;
	//cout << "   Velocity of atom 556: (" << myWorld.getAtomInAtomList(556)->getVelocityX() << ", " << myWorld.getAtomInAtomList(556)->getVelocityY() << ", " << myWorld.getAtomInAtomList(556)->getVelocityZ() << ")" << endl;
	//cout << "   Acceleration of atom 556: (" << myWorld.getAtomInAtomList(556)->getAccelerationX() << ", " << myWorld.getAtomInAtomList(556)->getAccelerationY() << ", " << myWorld.getAtomInAtomList(556)->getAccelerationZ() << ")" << endl;
	//cout << "   Potential of atom 556: " << myWorld.getAtomInAtomList(556)->getPotential() << endl;
	//cout << "   Force on atom 555: (" << myWorld.getAtomInAtomList(555)->getForceX() << ", " << myWorld.getAtomInAtomList(555)->getForceY() << ", " << myWorld.getAtomInAtomList(555)->getForceZ() << ")" << endl;
	//cout << "   Total momentum: (" << p[0] << ", " << p[1] << ", " << p[2] << ")" << endl;
	cout << "   Potential energy: " << U[index] << endl;
	cout << "   Kinetic energy: " << K[index] << endl;
	cout << "   Total energy: " << U[index] + K[index] << endl;
	cout << "   Temperature: " << T[index] << endl;
	
	
	Atom* a = myWorld.getAtomInAtomList(62);
	Atom* b = myWorld.getAtomInAtomList(63);
	myFilePos << a->getPositionX() << " " << a->getPositionY() << " " << a->getPositionZ() << " " << b->getPositionX() << " " << b->getPositionY() << " " << b->getPositionZ() << " ";
	//myFileV << a->getVelocityX() << " " << a->getVelocityY() << " " << a->getVelocityZ() << " ";
	//myFileP << endl;
	//myFileV << endl;

	
	for (double t = deltaT; t < myParameters.getSimulationTime() - 0.5*deltaT; t += deltaT)
	{
		/*
		if (t > deltaT)
		{
			myWorld.updateCells();
			myWorld.updateNeighbourList();
		}
		*/

		myWorld.calcPotentialAndForce(t);
		myWorld.solveEquationsOfMotion(t);
		myWorld.calcPressure(t);
		U = *potArray;
		K = *kinArray;
		//p = myWorld.getResults().getMomentumVector(index);
		T = *tempArray;
		index = (int)round(t / deltaT);


		myFilePos << a->getPositionX() << " " << a->getPositionY() << " " << a->getPositionZ() << " " << b->getPositionX() << " " << b->getPositionY() << " " << b->getPositionZ() << " ";
		
		
		cout << "Time " << t << ": " << endl;
		//cout << "   Position of atom 555: (" << myWorld.getAtomInAtomList(555)->getPositionX() << ", " << myWorld.getAtomInAtomList(555)->getPositionY() << ", " << myWorld.getAtomInAtomList(555)->getPositionZ() << ")" << endl;
		//cout << "   Velocity of atom 555: (" << myWorld.getAtomInAtomList(555)->getVelocityX() << ", " << myWorld.getAtomInAtomList(555)->getVelocityY() << ", " << myWorld.getAtomInAtomList(555)->getVelocityZ() << ")" << endl;
		//cout << "   Magnitude of velocity of atom 555: " << sqrt(pow(myWorld.getAtomInAtomList(555)->getVelocityX(), 2) + pow(myWorld.getAtomInAtomList(555)->getVelocityY(), 2) + pow(myWorld.getAtomInAtomList(555)->getVelocityZ(), 2)) << "!" << endl;
		//cout << "   Acceleration of atom 555: (" << myWorld.getAtomInAtomList(555)->getAccelerationX() << ", " << myWorld.getAtomInAtomList(555)->getAccelerationY() << ", " << myWorld.getAtomInAtomList(555)->getAccelerationZ() << ")" << endl;
		//cout << "   Potential of atom 555: " << myWorld.getAtomInAtomList(555)->getPotential() << endl << endl;
		//cout << "   Force on atom 555: (" << myWorld.getAtomInAtomList(555)->getForceX() << ", " << myWorld.getAtomInAtomList(555)->getForceY() << ", " << myWorld.getAtomInAtomList(555)->getForceZ() << ")" << endl;
		//cout << "   Position of atom 556: (" << myWorld.getAtomInAtomList(556)->getPositionX() << ", " << myWorld.getAtomInAtomList(556)->getPositionY() << ", " << myWorld.getAtomInAtomList(556)->getPositionZ() << ")" << endl;
		//cout << "   Velocity of atom 556: (" << myWorld.getAtomInAtomList(556)->getVelocityX() << ", " << myWorld.getAtomInAtomList(556)->getVelocityY() << ", " << myWorld.getAtomInAtomList(556)->getVelocityZ() << ")" << endl;
		//cout << "   Acceleration of atom 556: (" << myWorld.getAtomInAtomList(556)->getAccelerationX() << ", " << myWorld.getAtomInAtomList(556)->getAccelerationY() << ", " << myWorld.getAtomInAtomList(556)->getAccelerationZ() << ")" << endl;
		//cout << "   Potential of atom 556: " << myWorld.getAtomInAtomList(556)->getPotential() << endl;
		//
		cout << "   Potential energy: " << U[index] << endl;
		cout << "   Kinetic energy: " << K[index] << endl;
		//cout << "   Total momentum: (" << p[0] << ", " << p[1] << ", " << p[2] << ")" << endl;
		cout << "   Total energy: " << U[index] + K[index] << endl;
		cout << "   Temperature: " << T[index] << endl << endl;
	}
	
	myFilePos.close();
	/*
	myFileP.close();
	myFileV.close();
	*/
	//cout << "Hello there!" << endl;

	char exit;

	cin >> exit;

	return 0;
}
