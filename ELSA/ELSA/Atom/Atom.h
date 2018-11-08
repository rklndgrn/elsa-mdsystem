#ifndef ATOM_H
#define ATOM_H

#include <cstddef>
#include <vector>
#include <array>

class Atom
{
private:
	unsigned int _id{};
	std::array<unsigned int, 3> _cellIndex = { 0, 0, 0 };
	std::array<double, 3> _position = { 0, 0, 0 };
	std::array<double, 3> _velocity = { 0, 0, 0 };
	std::array<double, 3> _acceleration = { 0, 0, 0 };
	double _potential{};
	std::array<double, 3> _force{};
	std::vector<Atom*> _neighbourList;

public:
	Atom(int, double, double, double);
	Atom() = default;
	~Atom() = default;

	int getID() const;

	double getAccX() const;
	double getAccY() const;
	double getAccZ() const;
	double getPosX() const;
	double getPosY() const;
	double getPosZ() const;
	double getPotential() const;
	double getForceX() const;
	double getForceY() const;
	double getForceZ() const;
	double getVelocityX() const;
	double getVelocityY() const;
	double getVelocityZ() const;


	std::array<unsigned int, 3> getCellIndex() const;
	std::vector<Atom*> getNeighbourList() const;

	void addToNeighbourList(Atom*);
	void setAccX(double);
	void setAccY(double);
	void setAccZ(double);
	void setAcceleration(std::array<double, 3>);
	void setCellIndex(int, int, int);
	void setPosX(double);
	void setPosY(double);
	void setPosZ(double);
	void setPosition(std::array<double, 3>);
	void setPotential(double);
	void setForceX(double);
	void setForceY(double);
	void setForceZ(double);
	void setForce(std::array<double, 3>);
	void setVelocityX(double);
	void setVelocityY(double);
	void setVelocityZ(double);
	void setVelocity(std::array<double, 3>);
};

#endif