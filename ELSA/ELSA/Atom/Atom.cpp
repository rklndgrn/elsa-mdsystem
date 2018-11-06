#include "Atom.h"

using namespace std;

//Constructor
Atom::Atom(size_t newID, double newPosX, double newPosY, double newPosZ)
{
	id = newID;
	position[0] = newPosX;
	position[1] = newPosY;
	position[2] = newPosZ;
}


//Getters
double Atom::getAccX() const
{
	return acceleration[0];
}

double Atom::getAccY() const
{
	return acceleration[1];
}

double Atom::getAccZ() const
{
	return acceleration[2];
}

int* Atom::getCellIndex() const
{
	int arr[3] = { cellIndex[0], cellIndex[1], cellIndex[2]};
	return arr;
}

vector<Atom*> Atom::getNeighbourList() const
{
	return neighbourList;
}

size_t Atom::getID() const
{
	return id;
}

double Atom::getPosX() const
{
	return position[0];
}

double Atom::getPosY() const
{
	return position[1];
}

double Atom::getPosZ() const
{
	return position[2];
}

double Atom::getPotential() const
{
	return potential;
}

double Atom::getForce() const
{
	return force;
}

double Atom::getVelocityX() const
{
	return velocity[0];
}

double Atom::getVelocityY() const
{
	return velocity[1];
}

double Atom::getVelocityZ() const
{
	return velocity[2];
}



//Setters
void Atom::addToNeighbourList(Atom a)
{
	neighbourList.push_back(&a);
}

void Atom::setAccX(double newPos)
{
	acceleration[0] = newPos;
}

void Atom::setAccY(double newPos)
{
	acceleration[1] = newPos;
}

void Atom::setAccZ(double newPos)
{
	acceleration[2] = newPos;
}

void Atom::setAcceleration(double x, double y, double z)
{
	setAccX(x);
	setAccY(y);
	setAccZ(z);
}

void Atom::setCellIndex(int i, int j, int k)
{
	cellIndex[0] = i;
	cellIndex[1] = j;
	cellIndex[2] = k;
}

void Atom::setPosX(double newPos)
{
	position[0] = newPos;
}

void Atom::setPosY(double newPos)
{
	position[1] = newPos;
}

void Atom::setPosZ(double newPos)
{
	position[2] = newPos;
}

void Atom::setPosition(double x, double y, double z)
{
	setPosX(x);
	setPosY(y);
	setPosZ(z);
}

void Atom::setPotential(double u)
{
	potential = u;
}

void Atom::setForce(double F)
{
	force = F;
}

void Atom::setVelocityX(double newV)
{
	velocity[0] = newV;
}

void Atom::setVelocityY(double newV)
{
	velocity[1] = newV;
}

void Atom::setVelocityZ(double newV)
{
	velocity[2] = newV;
}

void Atom::setVelocity(double vx, double vy, double vz)
{
	setVelocityX(vx);
	setVelocityY(vy);
	setVelocityZ(vz);
}