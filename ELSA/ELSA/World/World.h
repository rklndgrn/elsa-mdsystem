#ifndef WORLD_H
#define WORLD_H

#include "../Atom/Atom.h"
#include "../Cell/Cell.h"
#include "../Parameters/Parameters.h"
#include "../Results/Results.h"
#include "../Simulation/Simulation.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <omp.h>
#include <random>
#include <vector>


class World
{
private:
	//Member objects
	std::vector<Atom*> _atomList;
	std::vector<Cell*> _cellList;

	Parameters _myParameters;
	Results _myResults;
	Simulation _mySimulation;

	double _pressureRFSum{ 0 };

	int numberOfThreads{omp_get_max_threads()};

	//Member functions
	bool checkM(int, bool, unsigned int, unsigned int, unsigned int);

	double getWorldVolume() const;

	void addAtomToAtomList(Atom*);
	void addCellToCellList(Cell*);
	void andersenThermostat(double);
	void calcPotentialAndForce(double);
	void calcPressure(double);
	void correctPositions(std::array<double, 3>&);
	void distributeInitialVelocities(double);
	void generateAtomsAtFccLattice(double, unsigned int, unsigned int, unsigned int);
	void generateAtomsAtScLattice(double, unsigned int, unsigned int, unsigned int);
	void generateCells();
	void initializeAtoms();
	void initializeResults();
	void populateCells();
	void resetAllPotentialsAndForces();
	void setupNeighbourLists(bool);
	void setupSystem(Parameters);
	void solveEquationsOfMotion(double);
	void updateResults(double, double);
	void velocityVerletStep1(double);
	void velocityVerletStep2(double);

public:
	World() = default;
	World(Parameters, int);
	~World();

	Results getResults();

	Atom* getAtomInAtomList(unsigned int);

	Cell* getCellInCellList(unsigned int, unsigned int, unsigned int);

	int getNumberOfThreads() const;

	void performSimulation(double, int);
	void setNumberOfThreads(int);
	void updateCells();
	void updateNeighbourList();

};

#endif