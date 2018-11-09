#ifndef WORLD_H
#define WORLD_H

#include "../Atom/Atom.h"
#include "../Cell/Cell.h"
#include "../Parameters/Parameters.h"
#include "../Results/Results.h"
#include "../Simulation/Simulation.h"

#include <cmath>
#include <iostream>
#include <vector>
#include <random>

class World
{
private:
	std::vector<Atom*> _atomList;
	std::vector<Cell*> _cellList;
	Parameters _myParameters;
	Results _myResults;
	Simulation _mySimulation;

	int incrementM(int, unsigned int, unsigned int, unsigned int);
	
	void setupSystem(Parameters);
	void generateAtomsAtScLattice(double, unsigned int, unsigned int, unsigned int);
	void generateAtomsAtFccLattice(double, unsigned int, unsigned int, unsigned int);
	void setupNeighbourLists(bool);
	void distributeInitialVelocities();
	void generateCells();
	void populateCells();

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

	void calcPotentialAndForce();
	void calcPotentialEnergy();

	void calcPressure(double);
	void solveEquationsOfMotion(double);

};

#endif