#ifndef WORLD_H
#define WORLD_H

#include "../Atom/Atom.h"
#include "../Cell/Cell.h"
#include "../Parameters/Parameters.h"
#include "../Results/Results.h"
#include "../Simulation/Simulation.h"

#include <cmath>
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

	double getWorldVolume() const;

	void addAtomToAtomList(Atom*);
	void addCellToCellList(Cell*);
	void andersonThermostat(double, double);
	void calcPotentialAndForce(double);
	void calcPressure(double);
	void correctPositions(std::array<double, 3>&);
	void distributeInitialVelocities(double);
	void generateAtomsAtFccLattice(double, unsigned int, unsigned int, unsigned int);
	void generateAtomsAtScLattice(double, unsigned int, unsigned int, unsigned int);
	void generateCells();
	void initializeAtoms();
	void populateCells();
	void resetAllPotentialsAndForces();
	void setupNeighbourLists(bool);
	void setupSystem(Parameters);
	void solveEquationsOfMotion(double);
	void updateMSDAndDebyeTemperature(double, double);
	void updateSelfDiffusionConstantAndSpecificHeat(double);
	void velocityVerletStep1(double);
	void velocityVerletStep2(double);

public:
	World() = default;
	World(Parameters);
	~World() = default;

	Results getResults();

	Atom* getAtomInAtomList(unsigned int);

	Cell* getCellInCellList(unsigned int, unsigned int, unsigned int);

	void performSimulation(double);
	void updateCells();
	void updateNeighbourList();

};

#endif