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
	//Member objects
	std::vector<Atom*> _atomList;
	std::vector<Cell*> _cellList;

	Parameters _myParameters;
	Results _myResults;
	Simulation _mySimulation;

	double _pressureRFSum{ 0 };

	//Member functions
	int incrementM(int, unsigned int, unsigned int, unsigned int);
	void correctPositions(std::array<double, 3>&);

	void distributeInitialVelocities();
	void generateAtomsAtFccLattice(double, unsigned int, unsigned int, unsigned int);
	void generateAtomsAtScLattice(double, unsigned int, unsigned int, unsigned int);
	void generateCells();
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
	void calcPotentialAndForce();
	void calcPotentialEnergy(double);
	void calcPressure(double);
	void solveEquationsOfMotion(double);

};

#endif