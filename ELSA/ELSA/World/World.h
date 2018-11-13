#ifndef WORLD_H
#define WORLD_H

#include "../Atom/Atom.h"
#include "../Cell/Cell.h"
#include "../Parameters/Parameters.h"
#include "../Results/Results.h"
#include "../Simulation/Simulation.h"

#include <cmath>
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

	int numberOfThreads{4};

	//Member functions
	bool checkM(int, bool, unsigned int, unsigned int, unsigned int);

	void correctPositions(std::array<double, 3>&);
	void distributeInitialVelocities();
	void generateAtomsAtFccLattice(double, unsigned int, unsigned int, unsigned int);
	void generateAtomsAtScLattice(double, unsigned int, unsigned int, unsigned int);
	void generateCells();
	void initializeAtoms();
	void populateCells();
	void setupNeighbourLists(bool);
	void setupSystem(Parameters);

public:
	World() = default;
	World(Parameters);
	~World() = default;

	Results getResults();

	Atom* getAtomInAtomList(unsigned int);

	Cell* getCellInCellList(unsigned int, unsigned int, unsigned int);

	double getWorldVolume() const;

	void addAtomToAtomList(Atom*);
	void addCellToCellList(Cell*);
	void calcPotentialAndForce(double);
	void calcPotentialAndForcePerAtom(Atom*, double);
	void calcPressure(double);
	void resetAllPotentialsAndForces();
	void solveEquationsOfMotion(double);
	void updateCells();
	void updateNeighbourList();

};

#endif