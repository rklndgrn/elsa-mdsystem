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
	std::string crystalStructure;
	double latticeConstant;
	double epsilon;
	double sigma;
	double cutOffDistance;
	double mass;

public:
	std::string getCrystalStructure();
	double getLatticeConstant();
	double getEpsilon();
	double getSigma();
	double getCutOffDistance();
	double getMass();

	void setCrystalStructure(std::string str);
	void setLatticeConstant(double lc);
	void setEpsilon(double eps);
	void setSigma(double sig);
	void setCutOffDistance(double cod);
	void setMass(double m);

	Material();
	~Material();
};