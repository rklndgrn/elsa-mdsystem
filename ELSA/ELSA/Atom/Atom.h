#ifndef ATOM_H
#define ATOM_H

#include <cstddef>
#include <vector>
#include <array>

class Atom
{
private:
	unsigned int id{};
	std::array<unsigned int, 3> cellIndex = { 0, 0, 0 };
	double position[3] = {0, 0, 0};
	double velocity[3] = {0, 0, 0};
	double acceleration[3] = {0, 0, 0};
	double potential{};
	double force{};
	std::vector<Atom*> neighbourList;

public:
	Atom(int, double, double, double);
	Atom() = default;
	~Atom() = default;

	int getID() const;
	double getAccX() const;
	double getAccY() const;
	double getAccZ() const;
	std::array<unsigned int, 3> getCellIndex() const;
	std::vector<Atom*> getNeighbourList() const;
	double getPosX() const;
	double getPosY() const;
	double getPosZ() const;
	double getPotential() const;
	double getForce() const;
	double getVelocityX() const;
	double getVelocityY() const;
	double getVelocityZ() const;

	void addToNeighbourList(Atom);
	void setAccX(double);
	void setAccY(double);
	void setAccZ(double);
	void setAcceleration(double, double, double);
	void setCellIndex(int, int, int);
	void setID(size_t);
	void setPosX(double);
	void setPosY(double);
	void setPosZ(double);
	void setPosition(double, double, double);
	void setPotential(double);
	void setForce(double);
	void setVelocityX(double);
	void setVelocityY(double);
	void setVelocityZ(double);
	void setVelocity(double, double, double);
};

#endif