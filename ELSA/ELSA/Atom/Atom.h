#ifndef ATOM_H
#define ATOM_H

#include <cstddef>
#include <vector>
#include <array>

class Atom
{
private:
	unsigned int _id{};

	double _potential{};

	std::array<double, 3> _acceleration = { 0, 0, 0 };
	std::array<double, 3> _force{};
	std::array<double, 3> _position = { 0, 0, 0 };
	std::array<double, 3> _velocity = { 0, 0, 0 };

	std::array<unsigned int, 3> _cellIndex = { 0, 0, 0 };

	std::vector<Atom*> _neighbourList;

public:
	Atom(int, double, double, double);
	Atom() = default;
	~Atom() = default;

	void addToNeighbourList(Atom*);
	void clearNeighbourList();

	//getters
	unsigned int getID() const;

	double getAccelerationX() const;
	double getAccelerationY() const;
	double getAccelerationZ() const;
	double getForceX() const;
	double getForceY() const;
	double getForceZ() const;
	double getPositionX() const;
	double getPositionY() const;
	double getPositionZ() const;
	double getPotential() const;
	double getVelocityX() const;
	double getVelocityY() const;
	double getVelocityZ() const;


	std::array<unsigned int, 3> getCellIndex() const;
	std::vector<Atom*> getNeighbourList() const;

	//setters
	void setAccelerationX(double);
	void setAccelerationY(double);
	void setAccelerationZ(double);
	void setAcceleration(std::array<double, 3>);
	void setCellIndex(int, int, int);
	void setForceX(double);
	void setForceY(double);
	void setForceZ(double);
	void setForce(std::array<double, 3>);
	void setPositionX(double);
	void setPositionY(double);
	void setPositionZ(double);
	void setPosition(std::array<double, 3>);
	void setPotential(double);
	void setVelocityX(double);
	void setVelocityY(double);
	void setVelocityZ(double);
	void setVelocity(std::array<double, 3>);
};

#endif