#include "World.h"

using namespace std;

/* PRIVATE */
//Append atom to the atom list
void World::addAtomToAtomList(Atom* a)
{
	_atomList.push_back(a);
}

//Append cell to the cell list
void World::addCellToCellList(Cell* c)
{
	_cellList.push_back(c);
}

//Help function to step 2 of the Velocity Verlet algorithm.
//Distributes new Gaussian velocities with a probability based on the collision frequency.
void World::andersenThermostat(double T)
{
	omp_set_num_threads(numberOfThreads);

	Atom* thisAtom;
	array<double, 3> newV;

	random_device rd;
	mt19937 generator(rd());
	uniform_real_distribution<double> distribution(0, 1);
	double randomValue;
	double variance = _myParameters.getBoltzmann() * _myParameters.getTemperature() / _myParameters.getChosenMaterial().getMass();

	#pragma omp parallel private(newV, randomValue, thisAtom) shared(variance, distribution, generator)
	{
		//Andersen thermostat.
		#pragma omp for schedule(static)
		for (int i{ 0 }; i < _atomList.size(); i++)
		{
			thisAtom = _atomList.at(i);
			randomValue = distribution(generator);

			if (randomValue < _myParameters.getCollisionFrequency()*_myParameters.getTimeStep())
			{
				newV = _mySimulation.generateGaussianVelocity(variance);
				thisAtom->setVelocity(newV);
			}
		}
	}
}

//	Calculates the force and potential and stores them in the atoms. The force is directional but the potential is not.
void World::calcPotentialAndForce(double elapsedTime)
{
	//Reset before the next iteration.
	resetAllPotentialsAndForces();
	_pressureRFSum = 0;

	double potential{ 0 }, force{ 0 };// , totalPotential{ 0 };
	Atom* a1;
	Atom* a2;
	array<double, 4> r;
	int index = (int)(elapsedTime / _myParameters.getTimeStep());
	int N = _myParameters.getNumberOfAtoms();

	for (int i{ 0 }; i < (int)N - 1; i++)
	{
		a1 = _atomList.at(i);
		// For all atoms sufficiently close to a1.
		for (int j{ 0 }; j < a1->getNeighbourList().size(); j++)
		{
			a2 = a1->getNeighbourList().at(j);

			// Returns the distance as a homogeneous vector
			r = _mySimulation.calcDistance(a1, a2, _myParameters.getLengthX(), _myParameters.getLengthY(), _myParameters.getLengthZ(), _myParameters.getIs2D());

			
			force = _mySimulation.calcForce(r[0]);
			potential = _mySimulation.calcLJPotential(r[0]);
			
			a1->setPotential(a1->getPotential() + potential);
			a2->setPotential(a2->getPotential() + potential);

			a1->setForceX(a1->getForceX() + force * r[1]);
			a1->setForceY(a1->getForceY() + force * r[2]);
			a1->setForceZ(a1->getForceZ() + force * r[3]);

			a2->setForceX(a2->getForceX() - force * r[1]);
			a2->setForceY(a2->getForceY() - force * r[2]);
			a2->setForceZ(a2->getForceZ() - force * r[3]);

			//Accumulate for internal pressure calculation
			_pressureRFSum += r[0] * force;

			//totalPotential += potential;
		}
	}

	//_myResults.setPotentialEnergy(totalPotential, index);
	//_myResults.setCohesiveEnergy(index);
}

//Calculate the internal pressure at a certain time
void World::calcPressure(double elapsedTime)
{
	int timeSteps = (int)round(elapsedTime / _myParameters.getTimeStep());
	int N = _myParameters.getNumberOfAtoms();
	double V = getWorldVolume();
	double T = (*_myResults.getTemperature())[timeSteps];
	double meanRF = _pressureRFSum / (timeSteps);
	double pressure = (N*_myParameters.getBoltzmann()*T / V) + meanRF / (6 * V);

	_myResults.setInternalPressure(pressure, timeSteps);
}

//Function for checking whether the while loop in the setupNeighbourLists function should execute.
//Checks if we are to skip adding neighbour atoms in certain cells due to the simulation being in 2D.
bool World::checkM(int m, bool is2D, unsigned int maxK, unsigned int lowerK, unsigned int upperK)
{
	double m_check = m;
	if (is2D)
	{
		if (lowerK > maxK)
		{
			if (m/3 == m_check/3)
			{
				return false;
			}
		}
		else if (upperK > maxK)
		{
			if ((m + 1) / 3 == (m_check + 1) / 3)
			{
				return false;
			}
		}
	}
	return true;
}

//Correct positions according to periodic boundary conditions
void World::correctPositions(array<double, 3>& r)
{
	double factorX = abs(r[0] / _myParameters.getLengthX());
	double factorY = abs(r[1] / _myParameters.getLengthY());
	double factorZ = abs(r[2] / _myParameters.getLengthZ());

	if (r[0] < 0)
	{
		r[0] += ceil(factorX)*_myParameters.getLengthX();
	}
	else if (r[0] > _myParameters.getLengthX())
	{
		r[0] -= floor(factorX)*_myParameters.getLengthX();
	}
	if (r[1] < 0)
	{
		r[1] += ceil(factorY)*_myParameters.getLengthY();
	}
	else if (r[1] > _myParameters.getLengthY())
	{
		r[1] -= floor(factorY)*_myParameters.getLengthY();
	}
	if (!_myParameters.getIs2D())
	{
		if (r[2] < 0)
		{
			r[2] += ceil(factorZ)*_myParameters.getLengthZ();
		}
		else if (r[2] > _myParameters.getLengthZ())
		{
			r[2] -= floor(factorZ)*_myParameters.getLengthZ();
		}
	}
}

//Set the atoms' velocities from temperature according to Maxwell-Boltzmann distribution 
void World::distributeInitialVelocities(double desiredTemperature)
{
	//omp_set_num_threads(numberOfThreads);
	//double sigma = sqrt(_myParameters.getBoltzmann() * _myParameters.getTemperature() / _myParameters.getChosenMaterial().getMass());
	double variance = _myParameters.getBoltzmann() * _myParameters.getTemperature() / _myParameters.getChosenMaterial().getMass();
	double K{ 0 }, T{ 0 };

	array<double, 3> v;
	double totalVelocityX{ 0 }, totalVelocityY{ 0 }, totalVelocityZ{ 0 };

	#pragma omp parallel private(v) shared(variance) reduction(+: K, totalVelocityX, totalVelocityY, totalVelocityZ)
	{
		#pragma omp for
		for (int atomId = 0; atomId < (int)_myParameters.getNumberOfAtoms() - 1; atomId++)
		{
			v = _mySimulation.generateGaussianVelocity(variance);
			_atomList.at(atomId)->setVelocityX(v[0]);
			_atomList.at(atomId)->setVelocityY(v[1]);
			_atomList.at(atomId)->setVelocityZ(v[2]);
			K += _mySimulation.calcKineticEnergy(v[0], v[1], v[2]);
			totalVelocityX += v[0];
			totalVelocityY += v[1];
			totalVelocityZ += v[2];
		}
	}
	
	Atom* lastAtom = _atomList.at(_myParameters.getNumberOfAtoms() - 1);
	lastAtom->setVelocityX(-totalVelocityX);
	lastAtom->setVelocityY(-totalVelocityY);
	lastAtom->setVelocityZ(-totalVelocityZ);
	//cout << "Last atom gets velocity " << -totalVelocityX << " " << -totalVelocityY << " " << - totalVelocityZ << endl;
	K += _mySimulation.calcKineticEnergy(-totalVelocityX, -totalVelocityY, -totalVelocityZ);

	T = _mySimulation.calcTemperature(K, _myParameters.getBoltzmann(), _myParameters.getNumberOfAtoms());
	_myResults.setKineticEnergy(K, 0);
	_myResults.setTemperature(T, 0);
}

//Populate an FCC lattice with atoms.
void World::generateAtomsAtFccLattice(double latticeConstant, unsigned int nOfUnitCellsX, unsigned int nOfUnitCellsY, unsigned int nOfUnitCellsZ)
{
	unsigned int atomId{ 0 };
	for (unsigned int z = 0; z < nOfUnitCellsZ; z++)
	{
		for (unsigned int y = 0; y < nOfUnitCellsY; y++)
		{
			for (unsigned int x = 0; x < nOfUnitCellsX; x++)
			{
				//Place atoms in the corner and in the middle of the neighbouring sides of the "dice."
				Atom* a0 = new Atom(atomId++, x*latticeConstant, y*latticeConstant, z*latticeConstant);
				Atom* ax = new Atom(atomId++, x*latticeConstant, (y + 0.5)*latticeConstant, (z + 0.5)*latticeConstant);
				Atom* ay = new Atom(atomId++, (x + 0.5)*latticeConstant, y*latticeConstant, (z + 0.5)*latticeConstant);
				Atom* az = new Atom(atomId++, (x + 0.5)*latticeConstant, (y + 0.5)*latticeConstant, z*latticeConstant);
				addAtomToAtomList(a0);
				addAtomToAtomList(ax);
				addAtomToAtomList(ay);
				addAtomToAtomList(az);
				_myResults.setPositions(a0->getPositionX(), a0->getPositionY(), a0->getPositionZ(), 0, atomId - 4);
				_myResults.setPositions(ax->getPositionX(), ax->getPositionY(), ax->getPositionZ(), 0, atomId - 3);
				_myResults.setPositions(ay->getPositionX(), ay->getPositionY(), ay->getPositionZ(), 0, atomId - 2);
				_myResults.setPositions(az->getPositionX(), az->getPositionY(), az->getPositionZ(), 0, atomId - 1);

			}
		}
	}
}

//Populate an SC lattice with atoms.
void World::generateAtomsAtScLattice(double latticeConstant, unsigned int nOfUnitCellsX, unsigned int nOfUnitCellsY, unsigned int nOfUnitCellsZ)
{
	unsigned int atomId{ 0 };
	double xPos{ 0 }, yPos{ 0 }, zPos{ 0 };
	for (unsigned int z = 0; z < nOfUnitCellsZ; z++)
	{
		for (unsigned int y = 0; y < nOfUnitCellsY; y++)
		{
			for (unsigned int x = 0; x < nOfUnitCellsX; x++)
			{
				xPos = x * latticeConstant;
				yPos = y * latticeConstant;
				zPos = z * latticeConstant;
				Atom* a = new Atom(atomId, xPos, yPos, zPos);
				addAtomToAtomList(a);
				_myResults.setPositions(xPos, yPos, zPos, 0, atomId);
				atomId++;
			}
		}
	}
}

void World::initializeAtoms()
{
	//omp_set_num_threads(numberOfThreads);
	distributeInitialVelocities(_myParameters.getTemperature());
	calcPotentialAndForce(0);
	calcPressure(0);
	Atom* thisAtom;
	array<double, 3> newA;
	for (int i = 0; i < _atomList.size(); i++)
	{
		thisAtom = _atomList.at(i);
		newA = _mySimulation.calcAcceleration(thisAtom->getForceX(), thisAtom->getForceY(), thisAtom->getForceZ());
		thisAtom->setAcceleration(newA);
	}
}

void World::initializeResults()
{
	_myResults.setTotalEnergy(0);
	double T = (*_myResults.getTemperature())[0];
	updateResults(0, T);
}

//Creates cells for faster neighbourlist setup
void World::generateCells()
{
	unsigned int nOfUnitCellsX{ _myParameters.getNumberOfUnitCellsX() };
	unsigned int nOfUnitCellsY{ _myParameters.getNumberOfUnitCellsY() };
	unsigned int nOfUnitCellsZ{ _myParameters.getNumberOfUnitCellsZ() };

	double latticeConstant{ _myParameters.getChosenMaterial().getLatticeConstant() };
	double cellSize{ _myParameters.getChosenMaterial().getCellSize() };

	unsigned int numberOfCellsI{ _myParameters.getNumberOfCellsI() };
	unsigned int numberOfCellsJ{ _myParameters.getNumberOfCellsJ() };
	unsigned int numberOfCellsK{ _myParameters.getNumberOfCellsK() };

	for (unsigned int k = 0; k < numberOfCellsK; k++)
	{
		for (unsigned int j = 0; j < numberOfCellsJ; j++)
		{
			for (unsigned int i = 0; i < numberOfCellsI; i++)
			{
				Cell* c = new Cell(i, j, k);
				addCellToCellList(c);
			}
		}
	}
}

//Add atoms to the cells
void World::populateCells()
{
	omp_set_num_threads(numberOfThreads);

	unsigned int i;
	unsigned int j;
	unsigned int k;

	double cellSize{ _myParameters.getChosenMaterial().getCellSize() };
	Atom* a;


	#pragma omp parallel shared(cellSize) private(a, i, j, k)
	{
		#pragma omp for schedule(static)
		for (int atomId = 0; atomId < (int)_myParameters.getNumberOfAtoms(); atomId++)
		{
			a = getAtomInAtomList(atomId);

			//Find index of the cell tha atom is currently in
			i = (unsigned int)floor(a->getPositionX() / cellSize);
			j = (unsigned int)floor(a->getPositionY() / cellSize);
			k = (unsigned int)floor(a->getPositionZ() / cellSize);

			if (_myParameters.getIs2D())
			{
				if (k >= _myParameters.getNumberOfCellsK())
				{
					k = (unsigned int)_myParameters.getNumberOfCellsK() - 1;
				}
				else if (k < 0)
				{
					k = 0;
				}
			}

			#pragma omp critical
			{
				getCellInCellList(i, j, k)->addAtomToCellList(a);
			}
			a->setCellIndex(i, j, k);
		}
	}
}



void World::resetAllPotentialsAndForces()
{
	omp_set_num_threads(numberOfThreads);
	Atom* a;
#pragma omp parallel private(a)
	{
#pragma omp for schedule(static)
		for (int i{ 0 }; i < _atomList.size(); i++)
		{
			a = getAtomInAtomList(i);
			a->setPotential(0);
			a->setForceX(0);
			a->setForceY(0);
			a->setForceZ(0);
		}
	}
}

//Function for setting up the list of atoms that will contribute to a certain atom's potential.
void World::setupNeighbourLists(bool is2D)
{
	//Distance between two atoms for the control if they are neighbours.
	double atomDistance{ 0 };
	double cutOffDistance{ _myParameters.getChosenMaterial().getCutOffDistance() };

	//The maximum cell index in each dimension.
	unsigned int maxI = _myParameters.getNumberOfCellsI() - 1;
	unsigned int maxJ = _myParameters.getNumberOfCellsJ() - 1;
	unsigned int maxK = _myParameters.getNumberOfCellsK() - 1;

	//The length of the simulation box in each dimension.
	double lengthX = _myParameters.getLengthX();
	double lengthY = _myParameters.getLengthY();
	double lengthZ = _myParameters.getLengthZ();

	//The cell index.
	unsigned int i, j, k;
	//The cell indices of a certain cell's neighbours.
	unsigned int lowerNeighbourI, upperNeighbourI;
	unsigned int lowerNeighbourJ, upperNeighbourJ;
	unsigned int lowerNeighbourK, upperNeighbourK;
	//Array containing all neighbouring cells' indices.
	array<array<unsigned int, 3>, 27> index;
	Atom* neighbourAtom;

	#pragma omp parallel shared(index, cutOffDistance, i, j, k, lowerNeighbourI, lowerNeighbourJ, lowerNeighbourK, upperNeighbourI, upperNeighbourJ, upperNeighbourK, lengthX, lengthY, lengthZ, maxI, maxJ, maxK) private(atomDistance, neighbourAtom)
	{
		for (unsigned int atomId = 0; atomId < _myParameters.getNumberOfAtoms(); atomId++)
		{
			#pragma omp single
			{
				i = getAtomInAtomList(atomId)->getCellIndex()[0];
				j = getAtomInAtomList(atomId)->getCellIndex()[1];
				k = getAtomInAtomList(atomId)->getCellIndex()[2];

				//Standard case.
				lowerNeighbourI = i - 1;
				upperNeighbourI = i + 1;
				lowerNeighbourJ = j - 1;
				upperNeighbourJ = j + 1;
				lowerNeighbourK = k - 1;
				upperNeighbourK = k + 1;

				//At sides, edges and corners, there is a special case for periodic boundary conditions.
				if (i == 0)
				{
					lowerNeighbourI = maxI;
					upperNeighbourI = i + 1;
				}
				else if (i == maxI)
				{
					upperNeighbourI = 0;
					lowerNeighbourI = i - 1;
				}
				if (j == 0)
				{
					lowerNeighbourJ = maxJ;
					upperNeighbourJ = j + 1;
				}
				else if (j == maxJ)
				{
					upperNeighbourJ = 0;
					lowerNeighbourJ = j - 1;
				}
				//Except if it's 2D, then the periodic boundary conditions in the z-direction are removed.
				if (!is2D)
				{
					if (k == 0)
					{
						lowerNeighbourK = maxK;
						upperNeighbourK = k + 1;
					}
					else if (k == maxK)
					{
						lowerNeighbourK = k - 1;
						upperNeighbourK = 0;
					}
				}

				//Fill the array of neighouring cell indices
				index.at(0) = { lowerNeighbourI, lowerNeighbourJ, lowerNeighbourK };
				index.at(1) = { lowerNeighbourI, lowerNeighbourJ, k };
				index.at(2) = { lowerNeighbourI, lowerNeighbourJ, upperNeighbourK };
				index.at(3) = { lowerNeighbourI, j, lowerNeighbourK };
				index.at(4) = { lowerNeighbourI, j, k };
				index.at(5) = { lowerNeighbourI, j, upperNeighbourK };
				index.at(6) = { lowerNeighbourI, upperNeighbourJ, lowerNeighbourK };
				index.at(7) = { lowerNeighbourI, upperNeighbourJ, k };
				index.at(8) = { lowerNeighbourI, upperNeighbourJ, upperNeighbourK };

				index.at(9) = { i, lowerNeighbourJ, lowerNeighbourK };
				index.at(10) = { i, lowerNeighbourJ, k };
				index.at(11) = { i, lowerNeighbourJ, upperNeighbourK };
				index.at(12) = { i, j, lowerNeighbourK };
				index.at(13) = { i, j, k };
				index.at(14) = { i, j, upperNeighbourK };
				index.at(15) = { i, upperNeighbourJ, lowerNeighbourK };
				index.at(16) = { i, upperNeighbourJ, k };
				index.at(17) = { i, upperNeighbourJ, upperNeighbourK };

				index.at(18) = { upperNeighbourI, lowerNeighbourJ, lowerNeighbourK };
				index.at(19) = { upperNeighbourI, lowerNeighbourJ, k };
				index.at(20) = { upperNeighbourI, lowerNeighbourJ, upperNeighbourK };
				index.at(21) = { upperNeighbourI, j, lowerNeighbourK };
				index.at(22) = { upperNeighbourI, j, k };
				index.at(23) = { upperNeighbourI, j, upperNeighbourK };
				index.at(24) = { upperNeighbourI, upperNeighbourJ, lowerNeighbourK };
				index.at(25) = { upperNeighbourI, upperNeighbourJ, k };
				index.at(26) = { upperNeighbourI, upperNeighbourJ, upperNeighbourK };
			}

			int startValue{ 0 };
			//Special case for 2D not handled by the function incrementM.
			if (k == 0 && is2D)
			{
				startValue = 1;
			}
			//Find the neighbouring atoms.
			#pragma omp for schedule(static)
			for (int m = startValue; m < 27; m++)
			{
				//If it's 2D, some of the neighbouring cells should be discarded.
				if (checkM(m, is2D, maxK, lowerNeighbourK, upperNeighbourK))
				{
					//cout << "Thread " << omp_get_thread_num() << " takes loop number " << m << "!" << endl;
					for (int n{ 0 }; (int) n < getCellInCellList(index.at(m)[0], index.at(m)[1], index.at(m)[2])->getAtomsInCellList().size(); n++)
					{
						atomDistance = _mySimulation.calcDistance(getAtomInAtomList(atomId),
							getCellInCellList(index.at(m)[0], index.at(m)[1], index.at(m)[2])->getAtomsInCellList().at(n),
							lengthX, lengthY, lengthZ, is2D)[0];

						//Add neighbour to a certain atom if distance < cut-off and the neighbour's index is larger than its own.
						if (atomDistance < cutOffDistance && atomId < getCellInCellList(index.at(m)[0], index.at(m)[1], index.at(m)[2])->getAtomsInCellList().at(n)->getID())
						{
							neighbourAtom = getAtomInAtomList(atomId);
							#pragma omp critical
							{
								neighbourAtom->addToNeighbourList(getCellInCellList(index.at(m)[0], index.at(m)[1], index.at(m)[2])->getAtomsInCellList().at(n));
							}
						}
					}
				}
			}
		}
	}
}

//Function to solve the equations of motion. Finds new velocities and positions of atoms and calculates their kinetic energy and temperature.
void World::solveEquationsOfMotion(double elapsedTime)
{
	//Go through the atom list and assign new positions and velocities using the Velocity Verlet Algorithm.
	velocityVerletStep1(elapsedTime);

	calcPotentialAndForce(elapsedTime);

	velocityVerletStep2(elapsedTime);
}

//Initialize the MD software by setting up the system and creating atoms, cells and neighbour lists.
void World::setupSystem(Parameters p)
{
	_myParameters = p;
	_myResults = Results{ p.getSimulationTime(), p.getTimeStep(), p.getNumberOfAtoms() };
	_mySimulation = Simulation(p.getChosenMaterial());

	unsigned int nOfUnitCellsX{ _myParameters.getNumberOfUnitCellsX() };
	unsigned int nOfUnitCellsY{ _myParameters.getNumberOfUnitCellsY() };
	unsigned int nOfUnitCellsZ{ _myParameters.getNumberOfUnitCellsZ() };
	double latticeConstant{ _myParameters.getChosenMaterial().getLatticeConstant() };
	string crystalStructure{ _myParameters.getChosenMaterial().getCrystalStructure() };

	if (crystalStructure == "fcc")
	{
		generateAtomsAtFccLattice(latticeConstant, nOfUnitCellsX, nOfUnitCellsY, nOfUnitCellsZ);
	}
	else if (crystalStructure == "sc")
	{
		generateAtomsAtScLattice(latticeConstant, nOfUnitCellsX, nOfUnitCellsY, nOfUnitCellsZ);
	}
	generateCells();
	populateCells();

	setupNeighbourLists(_myParameters.getIs2D());

	initializeAtoms();
	initializeResults();
}

//Update the results arrays.
void World::updateResults(double elapsedTime, double T)
{
	double**** positionsArray = _myResults.getPositions();
	double*** positions = *positionsArray;
	int index = (int)round(elapsedTime / _myParameters.getTimeStep());
	int N = _myParameters.getNumberOfAtoms();

	//The mean square displacement.
	double MSD = _mySimulation.calcMeanSquareDisplacement(positions[index], positions[0],
		_myParameters.getNumberOfAtoms(),
		_myParameters.getLengthX(),
		_myParameters.getLengthY(),
		_myParameters.getLengthZ(),
		_myParameters.getIs2D());
	_myResults.setMeanSquareDisplacement(MSD, index);

	//The Debye Temperature.
	double debyeTemperature = _mySimulation.calcDebyeTemperature(_myParameters.getNormalizedPlanck(), T,
		_myParameters.getChosenMaterial().getMass(),
		_myParameters.getBoltzmann(), MSD);
	_myResults.setDebyeTemperature(debyeTemperature, index);

	//Self diffusion coefficient
	double selfDiffusionCoefficient = _mySimulation.calcSelfDiffusionCoefficient(*(_myResults.getPositions()), 0, elapsedTime, _myParameters.getTimeStep(),
		N, _myParameters.getLengthX(), _myParameters.getLengthY(),
		_myParameters.getLengthZ(), _myParameters.getIs2D());
	_myResults.setDiffusionConstant(selfDiffusionCoefficient, index);

	//Specific heat.
	double specificHeat = _mySimulation.calcSpecificHeat(N, _myParameters.getBoltzmann(), index, *(_myResults.getTemperature()));
	_myResults.setSpecificHeat(specificHeat, index);
}


void World::velocityVerletStep1(double elapsedTime)
{
	omp_set_num_threads(numberOfThreads);
	Atom* thisAtom;
	array<double, 3> oldR, oldV, oldA, newR;
	double timeStep = _myParameters.getTimeStep();
	int index = (int)round(elapsedTime / timeStep);
	#pragma omp parallel private(thisAtom, oldR, oldV, oldA, newR) shared(timeStep)
	{
		#pragma omp for 
		for (int i{ 0 }; i < _atomList.size(); i++)
		{
			thisAtom = _atomList.at(i);
			oldR = { thisAtom->getPositionX(), thisAtom->getPositionY(), thisAtom->getPositionZ() };
			oldV = { thisAtom->getVelocityX(), thisAtom->getVelocityY(), thisAtom->getVelocityZ() };
			oldA = { thisAtom->getAccelerationX(), thisAtom->getAccelerationY(), thisAtom->getAccelerationZ() };

			newR = _mySimulation.calcPosition(oldR, oldV, oldA, timeStep);
			correctPositions(newR);
			thisAtom->setPosition(newR);
		}
	}

	for (int i{ 0 }; i < _atomList.size(); i++)
	{
		thisAtom = _atomList.at(i);
		_myResults.setPositions(thisAtom->getPositionX(), thisAtom->getPositionY(), thisAtom->getPositionZ(), index, i);
	}
}

//The second step of the velocity verlet algorithm, where the new velocities and accelerations are found.
void World::velocityVerletStep2(double elapsedTime)
{
	omp_set_num_threads(numberOfThreads);

	Atom* thisAtom;
	array<double, 3> oldV, oldA, newV, newA;
	double m = _myParameters.getChosenMaterial().getMass();
	double K{ 0 }; //Kinetic energy.
	double U{ 0 }; //Potential energy.
	double T{ 0 }; //Instantenous temperature
	double px{ 0 }, py{ 0 }, pz{ 0 }; //Momentum.

	double timeStep = _myParameters.getTimeStep();
	int index = (int)round(elapsedTime / timeStep);

	#pragma omp parallel private(thisAtom, oldV, oldA, newV, newA) shared(m, timeStep) reduction(+: K, U)//reduction(+: K, U, px, py, pz)
	{
	#pragma omp for 
		for (int i{ 0 }; i < _atomList.size(); i++)
		{
			thisAtom = _atomList.at(i);
			oldV = { thisAtom->getVelocityX(), thisAtom->getVelocityY(), thisAtom->getVelocityZ() };
			oldA = { thisAtom->getAccelerationX(), thisAtom->getAccelerationY(), thisAtom->getAccelerationZ() };
			newA = _mySimulation.calcAcceleration(thisAtom->getForceX(), thisAtom->getForceY(), thisAtom->getForceZ());
			newV = _mySimulation.calcVelocity(oldV, oldA, newA, timeStep);

			thisAtom->setVelocity(newV);
			thisAtom->setAcceleration(newA);

			U += thisAtom->getPotential();
			K += _mySimulation.calcKineticEnergy(newV[0], newV[1], newV[2]);
			//px += m * newV[0];
			//py += m * newV[1];
			//pz += m * newV[2];
		}
	}

	T = _mySimulation.calcTemperature(K, _myParameters.getBoltzmann(), _myParameters.getNumberOfAtoms());

	//Save the energies, temperature and momentum (?) for the results presentation.
	_myResults.setPotentialEnergy(U, index);
	_myResults.setCohesiveEnergy(index);
	_myResults.setKineticEnergy(K, index);
	cout << "U is set to " << U << " at index " << index << "!" << endl;
	//_myResults.setMomentum(px, py, pz, index);
	_myResults.setTemperature(T, index);
	_myResults.setTotalEnergy(index);

	//Update some of the results that are to be plotted.
	updateResults(elapsedTime, T);

	if (T > 0 && _myParameters.getIsThermostatOn())
	{
		andersenThermostat(_myParameters.getTemperature());
	}
}

/* PUBLIC */
//Constructor.
World::World(Parameters p)
{
	setupSystem(p);
}

//Get atom from the atom list at index
Atom* World::getAtomInAtomList(unsigned int index)
{
	return _atomList.at(index);
}

//Get cell from the cell list at index i, j, k
Cell* World::getCellInCellList(unsigned int i, unsigned int j, unsigned int k)
{
	unsigned int index = i + j * _myParameters.getNumberOfCellsI() + k * _myParameters.getNumberOfCellsI()*_myParameters.getNumberOfCellsJ();
	return _cellList.at(index);
}

//Returns results object
Results World::getResults()
{
	return _myResults;
}

//Gets the volume of the simulation box
double World::getWorldVolume() const
{
	double numberOfUnitCells = _myParameters.getNumberOfUnitCellsX()*_myParameters.getNumberOfUnitCellsY()*_myParameters.getNumberOfUnitCellsZ();
	return numberOfUnitCells*pow(_myParameters.getChosenMaterial().getLatticeConstant(), 3);
}

//Main function to perform simulations.
void World::performSimulation(double elapsedTime, int iterationsBetweenCellUpdate)
{
	int index = (int)round(elapsedTime / _myParameters.getTimeStep());
	if (index % iterationsBetweenCellUpdate == 0)
	{
		updateCells();
		updateNeighbourList();
	}

	solveEquationsOfMotion(elapsedTime);
	calcPressure(elapsedTime);
	//updateMSDAndDebyeTemperature(elapsedTime, );
	//updateSelfDiffusionConstantAndSpecificHeat(elapsedTime);
}

void World::updateCells()
{
	for (int i{0}; i < _cellList.size(); i++)
	{
		Cell* c = _cellList.at(i);
		c->clearAtomsInCellList();
	}

	populateCells();
}

void World::updateNeighbourList()
{
	for (int i{ 0 }; i < _atomList.size(); i++)
	{
		Atom* a = _atomList.at(i);
		a->clearNeighbourList();
	}

	setupNeighbourLists(_myParameters.getIs2D());
	_myParameters.getIs2D();
}
