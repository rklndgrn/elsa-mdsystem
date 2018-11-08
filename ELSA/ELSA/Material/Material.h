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
	std::string _crystalStructure;
	double _latticeConstant;
	double _epsilon;
	double _sigma;
	double _cutOffDistance;
	double _cellSize;
	double _mass;

public:
	std::string getCrystalStructure() const;
	double getLatticeConstant() const;
	double getEpsilon() const;
	double getSigma() const;
	double getCutOffDistance() const;
	double getCellSize() const;
	double getMass() const;

	void setCrystalStructure(std::string str);
	void setLatticeConstant(double lc);
	void setEpsilon(double eps);
	void setSigma(double sig);
	void setCutOffDistance(double cod);
	void setCellSize(double);
	void setMass(double m);

	Material();
	Material(
		std::string crystalStructure,
		double latticeConstant,
		double epsilon,
		double sigma,
		double cutOffDistance,
		double mass);
	~Material();
};