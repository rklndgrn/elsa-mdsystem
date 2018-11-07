#include "Atom.h"

using namespace std;

//Constructor
Atom::Atom(int newID, double newPosX, double newPosY, double newPosZ)
{
	_id = newID;
	_position[0] = newPosX;
	_position[1] = newPosY;
	_position[2] = newPosZ;
}

//Getters
double Atom::getAccX() const
{
	return _acceleration[0];
}

double Atom::getAccY() const
{
	return _acceleration[1];
}

double Atom::getAccZ() const
{
	return _acceleration[2];
}

std::array<unsigned int, 3> Atom::getCellIndex() const
{
	return _cellIndex;
}

vector<Atom*> Atom::getNeighbourList() const
{
	return _neighbourList;
}

int Atom::getID() const
{
	return _id;
}

double Atom::getPosX() const
{
	return _position[0];
}

double Atom::getPosY() const
{
	return _position[1];
}

double Atom::getPosZ() const
{
	return _position[2];
}

double Atom::getPotential() const
{
	return _potential;
}

double Atom::getForceX() const
{
	return _force[0];
}

double Atom::getForceY() const
{
	return _force[1];
}

double Atom::getForceZ() const
{
	return _force[2];
}

double Atom::getVelocityX() const
{
	return _velocity[0];
}

double Atom::getVelocityY() const
{
	return _velocity[1];
}

double Atom::getVelocityZ() const
{
	return _velocity[2];
}



//Setters
void Atom::addToNeighbourList(Atom a)
{
	_neighbourList.push_back(&a);
}

void Atom::setAccX(double newPos)
{
	_acceleration[0] = newPos;
}

void Atom::setAccY(double newPos)
{
	_acceleration[1] = newPos;
}

void Atom::setAccZ(double newPos)
{
	_acceleration[2] = newPos;
}

void Atom::setAcceleration(double x, double y, double z)
{
	setAccX(x);
	setAccY(y);
	setAccZ(z);
}

void Atom::setCellIndex(int i, int j, int k)
{
	_cellIndex[0] = i;
	_cellIndex[1] = j;
	_cellIndex[2] = k;
}

void Atom::setPosX(double newPos)
{
	_position[0] = newPos;
}

void Atom::setPosY(double newPos)
{
	_position[1] = newPos;
}

void Atom::setPosZ(double newPos)
{
	_position[2] = newPos;
}

void Atom::setPosition(double x, double y, double z)
{
	setPosX(x);
	setPosY(y);
	setPosZ(z);
}

void Atom::setPotential(double u)
{
	_potential = u;
}

void Atom::setForceX(double F)
{
	_force[0] = F;
}

void Atom::setForceY(double F)
{
	_force[1] = F;
}

void Atom::setForceZ(double F)
{
	_force[2] = F;
}

void Atom::setForce(double fx, double fy, double fz)
{
	setForceX(fx);
	setForceY(fy);
	setForceZ(fz);
}

void Atom::setVelocityX(double newV)
{
	_velocity[0] = newV;
}

void Atom::setVelocityY(double newV)
{
	_velocity[1] = newV;
}

void Atom::setVelocityZ(double newV)
{
	_velocity[2] = newV;
}

void Atom::setVelocity(double vx, double vy, double vz)
{
	setVelocityX(vx);
	setVelocityY(vy);
	setVelocityZ(vz);
}