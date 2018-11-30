#include "Results.h"

using namespace std;

/* PUBLIC */

Results::Results(double simulationTime, double timeStep, unsigned int numberOfParticles)
{
	cout << "Results ctor " << this << endl;
	_arrayLength = (int) round(simulationTime/timeStep);
	_numberOfAtoms = numberOfParticles;

	_totalEnergy = new double[_arrayLength];
	_potentialEnergy = new double[_arrayLength];
	_temperature = new double[_arrayLength];
	_kineticEnergy = new double[_arrayLength];
	_cohesiveEnergy = new double[_arrayLength];
	_debyeTemperature = new double[_arrayLength];
	_diffusionConstant = new double[_arrayLength];
	_internalPressure = new double[_arrayLength];
	_meanSquareDisplacement = new double[_arrayLength];
	_specificHeat = new double[_arrayLength];

	//Create a 3D array for positions for each atom over time.
	_positions = new double**[_arrayLength];
	for (unsigned int i = 0; i < _arrayLength; i++)
	{
		_positions[i] = new double*[numberOfParticles];
		for (unsigned int j = 0; j < numberOfParticles; j++)
		{
			_positions[i][j] = new double[3];
		}
	}
}

//Copy constructor, e.g. r2{r].
Results::Results(Results const& other)
{
	cout << "Results copy constructor " << this << endl;
	_arrayLength = other.getArrayLength();
	_numberOfAtoms = other.getNumberOfAtoms();


	_totalEnergy = new double[_arrayLength];
	_potentialEnergy = new double[_arrayLength];
	_temperature = new double[_arrayLength];
	_kineticEnergy = new double[_arrayLength];
	_cohesiveEnergy = new double[_arrayLength];
	_debyeTemperature = new double[_arrayLength];
	_diffusionConstant = new double[_arrayLength];
	_internalPressure = new double[_arrayLength];
	_meanSquareDisplacement = new double[_arrayLength];
	_specificHeat = new double[_arrayLength];

	//Create a 3D array for positions for each atom over time.
	_positions = new double**[_arrayLength];
	for (unsigned int i = 0; i < _arrayLength; i++)
	{
		_positions[i] = new double*[_numberOfAtoms];
		for (unsigned int j = 0; j < _numberOfAtoms; j++)
		{
			_positions[i][j] = new double[3];
		}
	}

	for (int t = 0; t < _arrayLength; t++)
	{
		_totalEnergy[t] = other.getTotalEnergy()[t];
		_potentialEnergy[t] = other.getPotentialEnergy()[t];
		_temperature[t] = other.getTemperature()[t];
		_kineticEnergy[t] = other.getKineticEnergy()[t];
		_cohesiveEnergy[t] = other.getCohesiveEnergy()[t];
		_debyeTemperature[t] = other.getDebyeTemperature()[t];
		_diffusionConstant[t] = other.getDiffusionConstant()[t];
		_internalPressure[t] = other.getInternalPressure()[t];
		_meanSquareDisplacement[t] = other.getMeanSquareDisplacement()[t];
		_specificHeat[t] = other.getSpecificHeat()[t];
		for (int n = 0; n < _numberOfAtoms; n++)
		{
			_positions[t][n][0] = other.getPositions()[t][n][0];
			_positions[t][n][1] = other.getPositions()[t][n][1];
			_positions[t][n][2] = other.getPositions()[t][n][2];
		}
	}
}

//Copy allocation, t.ex. r = r2{other}.
Results & Results::operator = (Results const& other)
{
	//Delete everything.
	if (_cohesiveEnergy != nullptr && sizeof(_cohesiveEnergy) / sizeof(_cohesiveEnergy[0]) > 0)
	{
		delete[] _cohesiveEnergy;
		_cohesiveEnergy = nullptr;
	}
	if (_debyeTemperature != nullptr && sizeof(_debyeTemperature)/sizeof(_debyeTemperature[0]) > 0)
	{
		delete[] _debyeTemperature;
		_debyeTemperature = nullptr;
	}

	if (_diffusionConstant != nullptr && sizeof(_diffusionConstant) / sizeof(_diffusionConstant[0]) > 0)
	{
		delete[] _diffusionConstant;
		_diffusionConstant = nullptr;
	}
	if (_internalPressure != nullptr && sizeof(_internalPressure) / sizeof(_internalPressure[0]) > 0)
	{
		delete[] _internalPressure;
		_internalPressure = nullptr;
	}
	if (_kineticEnergy != nullptr && sizeof(_kineticEnergy) / sizeof(_kineticEnergy[0]) > 0)
	{
		delete[] _kineticEnergy;
		_kineticEnergy = nullptr;
	}
	if (_meanSquareDisplacement != nullptr && sizeof(_meanSquareDisplacement) / sizeof(_meanSquareDisplacement[0]) > 0)
	{
		delete[] _meanSquareDisplacement;
		_meanSquareDisplacement = nullptr;
	}
	if (_potentialEnergy != nullptr && sizeof(_potentialEnergy) / sizeof(_potentialEnergy[0]) > 0)
	{
		delete[] _potentialEnergy;
		_potentialEnergy = nullptr;
	}
	if (_specificHeat != nullptr && sizeof(_specificHeat) / sizeof(_specificHeat[0]) > 0)
	{
		delete[] _specificHeat;
		_specificHeat = nullptr;
	}
	if (_temperature != nullptr && sizeof(_temperature) / sizeof(_temperature[0]) > 0)
	{
		delete[] _temperature;
		_temperature = nullptr;
	}
	if (_totalEnergy != nullptr && sizeof(_totalEnergy) / sizeof(_totalEnergy[0]) > 0)
	{
		delete[] _totalEnergy;
		_totalEnergy = nullptr;
	}

	cout << "Results copy allocator " << this << ": size of vectors is " << sizeof(_positions) / sizeof(_positions[0]) << endl;
	if (_positions != nullptr & sizeof(_positions)/sizeof(_positions[0]) > 1)
	{
		for (int t = 0; t < _arrayLength; t++)
		{
			if (_positions[t] != nullptr && sizeof(_positions[t]) / sizeof(_positions[t][0]) > 0)
			{
				for (int n = 0; n < _numberOfAtoms; n++)
				{
					if (_positions[t][n] != nullptr && sizeof(_positions[t][n]) / sizeof(_positions[t][n][0]) > 0)
					{
						delete[] _positions[t][n];
						_positions[t][n] = nullptr;
					}
				}
				delete[] _positions[t];
				_positions[t] = nullptr;
			}
		}
		delete[] _positions;
		_positions = nullptr;
	}

	//Same as the copy constructor.
	_arrayLength = other.getArrayLength();
	_numberOfAtoms = other.getNumberOfAtoms();
	
	_totalEnergy = new double[_arrayLength];
	_potentialEnergy = new double[_arrayLength];
	_temperature = new double[_arrayLength];
	_kineticEnergy = new double[_arrayLength];
	_cohesiveEnergy = new double[_arrayLength];
	_debyeTemperature = new double[_arrayLength];
	_diffusionConstant = new double[_arrayLength];
	_internalPressure = new double[_arrayLength];
	_meanSquareDisplacement = new double[_arrayLength];
	_specificHeat = new double[_arrayLength];

	//Create a 3D array for positions for each atom over time.
	_positions = new double**[_arrayLength];
	for (unsigned int i = 0; i < _arrayLength; i++)
	{
		_positions[i] = new double*[_numberOfAtoms];
		for (unsigned int j = 0; j < _numberOfAtoms; j++)
		{
			_positions[i][j] = new double[3];
		}
	}

	for (int t = 0; t < _arrayLength; t++)
	{
		_totalEnergy[t] = other.getTotalEnergy()[t];
		_potentialEnergy[t] = other.getPotentialEnergy()[t];
		_temperature[t] = other.getTemperature()[t];
		_kineticEnergy[t] = other.getKineticEnergy()[t];
		_cohesiveEnergy[t] = other.getCohesiveEnergy()[t];
		_debyeTemperature[t] = other.getDebyeTemperature()[t];
		_diffusionConstant[t] = other.getDiffusionConstant()[t];
		_internalPressure[t] = other.getInternalPressure()[t];
		_meanSquareDisplacement[t] = other.getMeanSquareDisplacement()[t];
		_specificHeat[t] = other.getSpecificHeat()[t];
		for (int n = 0; n < _numberOfAtoms; n++)
		{
			_positions[t][n][0] = other.getPositions()[t][n][0];
			_positions[t][n][1] = other.getPositions()[t][n][1];
			_positions[t][n][2] = other.getPositions()[t][n][2];
		}
	}
	
	return *this;
}

Results::~Results()
{
	cout << "Results dtor " << this << endl;
	delete [] _cohesiveEnergy;
	delete [] _debyeTemperature;
	delete [] _diffusionConstant;
	delete [] _internalPressure;
	delete [] _kineticEnergy;
	delete [] _meanSquareDisplacement;
	delete [] _potentialEnergy;
	delete [] _specificHeat;
	delete [] _temperature;
	delete [] _totalEnergy;

	for (int i = 0; i < _arrayLength; i++)
	{
		for (int j = 0; j < _numberOfAtoms; j++)
		{
			delete [] _positions[i][j];
		}
		delete [] _positions[i];
	}

	delete [] _positions;
}

//For testing the 3D array. Delete if unnecessary.
void Results::printPositions()
{
	for (unsigned int t = 0; t < _arrayLength; t++)
	{
		cout << "At time step " << t << ":" << endl;
		for (unsigned int n = 0; n < _numberOfAtoms; n++)
		{
			cout << "   Atom " << n << " has position: (" << _positions[t][n][0] << ", " << _positions[t][n][1] << ", " << _positions[t][n][2] << ")!" << endl;
		}
		cout << endl;
	}
}


//getters
unsigned int Results::getArrayLength() const
{
	return _arrayLength;
}

unsigned int Results::getNumberOfAtoms() const
{
	return _numberOfAtoms;
}

double* Results::getCohesiveEnergy() const
{
	return _cohesiveEnergy;
}

double* Results::getDebyeTemperature() const
{
	return _debyeTemperature;
}

double* Results::getDiffusionConstant() const
{
	return _diffusionConstant;
}

double* Results::getInternalPressure() const
{
	return _internalPressure;
}

double* Results::getKineticEnergy() const
{
	return _kineticEnergy;
}

double* Results::getMeanSquareDisplacement() const
{
	return _meanSquareDisplacement;
}

double*** Results::getPositions() const
{
	return _positions;
}

double* Results::getPotentialEnergy() const
{
	return _potentialEnergy;
}

double* Results::getSpecificHeat() const
{
	return _specificHeat;
}

double* Results::getTemperature() const
{
	return _temperature;
}

double* Results::getTotalEnergy() const
{
	return _totalEnergy;
}

//setters
void Results::setCohesiveEnergy(int index)
{
	_cohesiveEnergy[index] = _potentialEnergy[index]/_numberOfAtoms;
}

void Results::setDebyeTemperature(double value, int index)
{
	_debyeTemperature[index] = value;
}

void Results::setDiffusionConstant(double value, int index)
{
	_diffusionConstant[index] = value;
}

void Results::setInternalPressure(double value, int index)
{
	_internalPressure[index] = value;
}

void Results::setKineticEnergy(double value, int index)
{
	_kineticEnergy[index] = value;
}

void Results::setMeanSquareDisplacement(double value, int index)
{
	_meanSquareDisplacement[index] = value;
}

void Results::setPositions(double x, double y, double z, int t, int n)
{
	_positions[t][n][0] = x;
	_positions[t][n][1] = y;
	_positions[t][n][2] = z;
}

void Results::setPotentialEnergy(double value, int index)
{
	_potentialEnergy[index] = value;
}

void Results::setSpecificHeat(double value, int index)
{
	_specificHeat[index] = value;
}

void Results::setTemperature(double value, int index)
{
	_temperature[index] = value;
}

void Results::setTotalEnergy(int index)
{
	_totalEnergy[index] = _potentialEnergy[index] + _kineticEnergy[index];
}