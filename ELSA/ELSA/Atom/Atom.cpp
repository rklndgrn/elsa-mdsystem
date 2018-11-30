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

//Copy constructor, e.g. r2{r}.
Atom::Atom(Atom const& other)
{
	_id = other.getID();
	_potential = other.getPotential();

	_acceleration[0] = other.getAccelerationX();
	_acceleration[1] = other.getAccelerationY();
	_acceleration[2] = other.getAccelerationZ();

	_cellIndex[0] = other.getCellIndex()[0];
	_cellIndex[1] = other.getCellIndex()[1];
	_cellIndex[2] = other.getCellIndex()[2];

	_force[0] = other.getForceX();
	_force[1] = other.getForceY();
	_force[2] = other.getForceZ();

	Atom* a;
	for (int i = 0; other.getNeighbourList().size(); i++)
	{
		a = new Atom;
		a = other.getNeighbourList().at(i);
		
		_neighbourList.push_back(a);
	}

	_position[0] = other.getPositionX();
	_position[1] = other.getPositionY();
	_position[2] = other.getPositionZ();

	_velocity[0] = other.getVelocityX();
	_velocity[1] = other.getVelocityY();
	_velocity[2] = other.getVelocityZ();
}

//Copy allocation, t.ex. r = r2{other}.
Atom & Atom::operator = (Atom const& other)
{
	for (Atom* a: _neighbourList)
	{
		delete a;
	}
	_neighbourList.clear();

	_id = other.getID();
	_potential = other.getPotential();

	_acceleration[0] = other.getAccelerationX();
	_acceleration[1] = other.getAccelerationY();
	_acceleration[2] = other.getAccelerationZ();

	_cellIndex[0] = other.getCellIndex()[0];
	_cellIndex[1] = other.getCellIndex()[1];
	_cellIndex[2] = other.getCellIndex()[2];

	_force[0] = other.getForceX();
	_force[1] = other.getForceY();
	_force[2] = other.getForceZ();

	Atom* a;
	for (int i = 0; other.getNeighbourList().size(); i++)
	{
		a = new Atom;
		a = other.getNeighbourList().at(i);

		_neighbourList.push_back(a);
	}

	_position[0] = other.getPositionX();
	_position[1] = other.getPositionY();
	_position[2] = other.getPositionZ();

	_velocity[0] = other.getVelocityX();
	_velocity[1] = other.getVelocityY();
	_velocity[2] = other.getVelocityZ();

	return *this;
}


//Append atom to neighbour list
void Atom::addToNeighbourList(Atom* a)
{
	_neighbourList.push_back(a);
}

//Clear the neighbour list.
void Atom::clearNeighbourList()
{
	_neighbourList.clear();
}

//Getters
unsigned int Atom::getID() const
{
	return _id;
}

double Atom::getAccelerationX() const
{
	return _acceleration[0];
}

double Atom::getAccelerationY() const
{
	return _acceleration[1];
}

double Atom::getAccelerationZ() const
{
	return _acceleration[2];
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

double Atom::getPositionX() const
{
	return _position[0];
}

double Atom::getPositionY() const
{
	return _position[1];
}

double Atom::getPositionZ() const
{
	   	return _position[2];
}

double Atom::getPotential() const
{
	return _potential;
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

std::array<unsigned int, 3> Atom::getCellIndex() const
{
	return _cellIndex;
}

vector<Atom*> Atom::getNeighbourList() const
{
	return _neighbourList;
}



//Setters

void Atom::setAccelerationX(double a)
{
	_acceleration[0] = a;
}

void Atom::setAccelerationY(double a)
{
	_acceleration[1] = a;
}

void Atom::setAccelerationZ(double a)
{
	_acceleration[2] = a;
}

void Atom::setAcceleration(array<double, 3> a)
{
	setAccelerationX(a[0]);
	setAccelerationY(a[1]);
	setAccelerationZ(a[2]);
}

void Atom::setCellIndex(int i, int j, int k)
{
	_cellIndex[0] = i;
	_cellIndex[1] = j;
	_cellIndex[2] = k;
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

void Atom::setForce(array<double, 3> F)
{
	setForceX(F[0]);
	setForceY(F[1]);
	setForceZ(F[2]);
}

void Atom::setPositionX(double newPos)
{
	_position[0] = newPos;
}

void Atom::setPositionY(double newPos)
{
	_position[1] = newPos;
}

void Atom::setPositionZ(double newPos)
{
	_position[2] = newPos;
}

void Atom::setPosition(array<double, 3> r)
{
	setPositionX(r[0]);
	setPositionY(r[1]);
	setPositionZ(r[2]);
}

void Atom::setPotential(double u)
{
	_potential = u;
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

void Atom::setVelocity(array<double, 3> v)
{
	setVelocityX(v[0]);
	setVelocityY(v[1]);
	setVelocityZ(v[2]);
}