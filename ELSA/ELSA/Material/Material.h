/*
	File name: material.h
	Date: 2018-11-06
	This file contains the definition of the class Material
*/

#pragma once
#include <string>

class Material
{
private:
	double _cutOffDistance;
	double _cellSize;
	double _epsilon;
	double _latticeConstant;
	double _mass;
	double _sigma;

	std::string _crystalStructure;

public:
	Material();
	Material(std::string, double, double, double, double, double);
	~Material() = default;

	//getters
	double getCellSize() const;
	double getCutOffDistance() const;
	double getEpsilon() const;
	double getLatticeConstant() const;
	double getMass() const;
	double getSigma() const;

	std::string getCrystalStructure() const;

	//setters
	void setCellSize(double);
	void setCrystalStructure(std::string str);
	void setCutOffDistance(double cod);
	void setEpsilon(double eps);
	void setLatticeConstant(double lc);
	void setMass(double m);
	void setSigma(double sig);



};