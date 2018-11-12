#include "World.h"

using namespace std;

/* PRIVATE */
//Function for incrementing the while loop in the setupNeighbourLists function.
//Checks if we are to skip adding neighbour atoms in certain cells due to the simulation being in 2D.
/*
int World::incrementM(int m, unsigned int maxK, unsigned int lowerK, unsigned int upperK)
{
	double m_check = m;
	if (upperK <= maxK && lowerK <= maxK)
	{
		return m + 1;
	}
	else if (upperK > maxK)
	{
		if ((m - 1) / 3 == (m_check - 1) / 3)
		{
			return m + 2;
		}
		else
		{
			return m + 1;
		}
	}
	else
	{
		if ((m + 1) / 3 == (m_check + 1) / 3)
		{
			return m + 2;
		}
		else
		{
			return m + 1;
		}
	}
}
*/

//Replacement for the function incrementM.
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
	while (r[0] < 0)
	{
		r[0] += _myParameters.getLengthX();
	}
	while (r[0] > _myParameters.getLengthX())
	{
		r[0] -= _myParameters.getLengthX();
	}
	while (r[1] < 0)
	{
		r[1] += _myParameters.getLengthY();
	}
	while (r[1] > _myParameters.getLengthY())
	{
		r[1] -= _myParameters.getLengthY();
	}
	if (!_myParameters.getIs2D())
	{
		while (r[2] < 0)
		{
			r[2] += _myParameters.getLengthZ();
		}
		while (r[2] > _myParameters.getLengthZ())
		{
			r[2] -= _myParameters.getLengthZ();
		}
	}
}

void World::initializeAtoms()
{
	omp_set_num_threads(numberOfThreads);
	distributeInitialVelocities();
	calcPotentialAndForce(0);
	double K{0};
	Atom* thisAtom;
	#pragma omp parallel private(thisAtom) reduction (+: K)
	{
		#pragma omp for
		for (int i = 0; i < (int) _myParameters.getNumberOfAtoms(); i++)
		{
			thisAtom = getAtomInAtomList(i);

			K += _mySimulation.calcKineticEnergy(thisAtom->getVelocityX(), thisAtom->getVelocityY(), 
												thisAtom->getVelocityZ());
		}
	}
	_myResults.setKineticEnergy(K, 0);
	calcPressure(0);
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


#pragma omp critical
			{
				getCellInCellList(i, j, k)->addAtomToCellList(a);
				a->setCellIndex(i, j, k);
			}
		}
	}
}

/* PUBLIC */
//Constructor.
World::World(Parameters p)
{
	setupSystem(p);
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
				_myResults.setPositions(a0->getPositionX(), a0->getPositionY(), a0->getPositionY(), 0, atomId - 4);
				_myResults.setPositions(ax->getPositionX(), ax->getPositionY(), ax->getPositionY(), 0, atomId - 3);
				_myResults.setPositions(ay->getPositionX(), ay->getPositionY(), ay->getPositionY(), 0, atomId - 2);
				_myResults.setPositions(az->getPositionX(), az->getPositionY(), az->getPositionY(), 0, atomId - 1);

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
	
	#pragma omp parallel shared(index, cutOffDistance, i, j, k, lowerNeighbourI, lowerNeighbourJ, lowerNeighbourK, upperNeighbourI, upperNeighbourJ, upperNeighbourK, lengthX, lengthY, lengthZ, maxI, maxJ, maxK) private(atomDistance)
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
					for (unsigned int n{ 0 }; n < getCellInCellList(index.at(m)[0], index.at(m)[1], index.at(m)[2])->getAtomsInCellList().size(); n++)
					{
						atomDistance = _mySimulation.calcDistance(getAtomInAtomList(atomId),
						getCellInCellList(index.at(m)[0], index.at(m)[1], index.at(m)[2])->getAtomsInCellList().at(n),
						lengthX, lengthY, lengthZ, is2D)[0];
						//Add neighbour to a certain atom if distance < cut-off and
						//the neighbour's index is larger than its own.
						if (atomDistance < cutOffDistance && atomId < getCellInCellList(index.at(m)[0], index.at(m)[1], index.at(m)[2])->getAtomsInCellList().at(n)->getID())
						{
							#pragma omp critical
							{
								getAtomInAtomList(atomId)->addToNeighbourList(getCellInCellList(index.at(m)[0], index.at(m)[1], index.at(m)[2])->getAtomsInCellList().at(n));
							}
						}
					}
				}
				/*
				else
				{
					cout << "Hello! k = " << k << " from thread " << omp_get_thread_num() << " yields that m = " << m << " is not okay!" << endl;
				}
				*/
			} 
		}
	}
}

//Set the atoms' velocities from temperature according to Maxwell-Boltzmann distribution 
void World::distributeInitialVelocities()
{
	omp_set_num_threads(numberOfThreads);
	//double sigma = sqrt(_myParameters.getBoltzmann() * _myParameters.getTemperature() / _myParameters.getChosenMaterial().getMass());
	double variance = _myParameters.getBoltzmann() * _myParameters.getTemperature() / _myParameters.getChosenMaterial().getMass();

	array<double, 3> v;

	#pragma omp parallel private(v) shared(variance)
	{
		#pragma omp for
		for (int atomId = 0; atomId < (int) _myParameters.getNumberOfAtoms(); atomId++)
		{
			v = _mySimulation.generateGaussianVelocity(variance);
			_atomList.at(atomId)->setVelocityX(v[0]);
			_atomList.at(atomId)->setVelocityY(v[1]);
			_atomList.at(atomId)->setVelocityZ(v[2]);
		}
	}
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

//	Calculates the force and potential and stores them in the atoms. The force is directional but the potential is not.
void World::calcPotentialAndForce(double elapsedTime)
{
	double potential{0};
	double force{0};
	//array<double, 3> force;
	Atom* a1;
	Atom* a2;
	array<double, 4> r;
	for (int i{ 0 }; i < (int) _myParameters.getNumberOfAtoms() - 1; i++)
	{
		a1 = _atomList.at(i);
		// For all atoms close to a1
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
			a2->setForceX(a2->getForceX() - force * r[1]);
			a1->setForceY(a1->getForceY() + force * r[2]);
			a2->setForceY(a2->getForceY() - force * r[2]);
			a1->setForceZ(a1->getForceZ() + force * r[3]);
			a2->setForceZ(a2->getForceZ() - force * r[3]);

			//Accumulate for internal pressure calculation
			_pressureRFSum += r[0] * force;
		}
	}

	_myResults.setPotentialEnergy(potential, (int)round(elapsedTime / _myParameters.getTimeStep()));
	
}

// Calculate the potential energy as the sum of the potential of all the atoms.
void World::calcPotentialEnergy(double elapsedTime)
{
	Atom* a1;
	for (unsigned int i{ 0 }; i < _myParameters.getNumberOfAtoms() - 1; i++)
	{
		a1 = _atomList.at(i);
		_myResults.setPotentialEnergy(**_myResults.getPotentialEnergy() + a1->getPotential(),(int)round(elapsedTime/_myParameters.getTimeStep()));
	}
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

//Function to solve the equations of motion. Finds new velocities and positions of atoms and calculates their kinetic energy and temperature.
void World::solveEquationsOfMotion(double elapsedTime)
{
	omp_set_num_threads(numberOfThreads);

	Atom* thisAtom;
	array<double, 3> oldR;
	array<double, 3> oldV;
	array<double, 3> oldA;
	array<double, 3> newR;
	array<double, 3> newV;
	array<double, 3> newA;

	double K{0}; //Kinetic energy.
	double T{0}; //Instantenous temperature
	double timeStep = _myParameters.getTimeStep();
	_myParameters.setTemperature(T);

	//Stuff related to the Gaussian velocity for the Anderson thermostat.
	random_device rd;
	mt19937 generator(rd());
	uniform_real_distribution<double> distribution(0, 1);
	double randomValue;

	//Go through the atom list and assign new positions and velocities using the Velocity Verlet Algorithm.
	#pragma omp parallel private(thisAtom, oldR, oldV, oldA, newR, newV) shared(timeStep) reduction(+: K)
	{
		#pragma omp for 
		for (int i{ 0 }; i < _atomList.size(); i++)
		{
			thisAtom = _atomList.at(i);
			oldR = { thisAtom->getPositionX(), thisAtom->getPositionY(), thisAtom->getPositionZ() };
			oldV = { thisAtom->getVelocityX(), thisAtom->getVelocityY(), thisAtom->getVelocityZ() };
			oldA = { thisAtom->getPreviousAccelerationX(), thisAtom->getPreviousAccelerationY(), thisAtom->getPreviousAccelerationZ() };
			newA = _mySimulation.calcAcceleration(thisAtom->getForceX(), thisAtom->getForceY(), thisAtom->getForceZ());

			newR = _mySimulation.calcPosition(oldR, oldV, oldA, timeStep);
			correctPositions(newR);

			#pragma omp critical
			{
				_myResults.setPositions(newR[0], newR[1], newR[2], (int)round(elapsedTime / timeStep), i);
			}

			newV = _mySimulation.calcVelocity(oldV, oldA, timeStep);

			thisAtom->setPosition(newR);
			thisAtom->setVelocity(newV);

			K += _mySimulation.calcKineticEnergy(newV[0], newV[1], newV[2]);
		}
	}

	T = _mySimulation.calcTemperature(K, _myParameters.getBoltzmann(), _myParameters.getNumberOfAtoms());


	//Save the kinetic energy and temperature for the results presentation.
	_myResults.setKineticEnergy(K, (int) round(elapsedTime / timeStep));
	_myResults.setTemperature(T, (int) round(elapsedTime / timeStep));

	if (T > 0 && _myParameters.getIsThermostatOn())
	{
		#pragma omp parallel private(newV, randomValue, thisAtom) shared(distribution, generator)
		{
			//Anderson thermostat.
			#pragma omp for schedule(static)
			for (int i{ 0 }; i < _atomList.size(); i++)
			{
				thisAtom = _atomList.at(i);
				randomValue = distribution(generator);

				if (randomValue < _myParameters.getCollisionFrequency()*elapsedTime)
				{
					newV = _mySimulation.generateGaussianVelocity(T);
					thisAtom->setVelocity(newV);
				}
			}
		}
	}
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
}
