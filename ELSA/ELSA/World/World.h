#ifndef WORLD_H
#define WORLD_H

#include "../Atom/Atom.h"
#include "../Cell/Cell.h"
#include "../Parameters/Parameters.h"
#include "../Results/Results.h"
#include "../Simulation/Simulation.h"
#include <vector>

class World
{
private:
	std::vector<Atom*> _atomList;
	std::vector<Cell> _cellList;
	Parameters _myParameters;
	Results _myResults;
	Simulation _mySimulation;
	
public:
	World(Parameters);
	~World() = default;

	Results getResults();

	Atom* getAtomInAtomList(unsigned int);

	double getWorldVolume() const;

	void addAtomToAtomList(Atom*);
	void calcPotentialAndForce();
	void calcPotentialEnergy();
	void calcPressure(double);
	void solveEquationsOfMotion(double);
};

#endif