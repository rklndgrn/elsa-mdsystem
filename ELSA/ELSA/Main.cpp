#include "./Atom/Atom.h"
#include "./Material/Material.h"
#include "./Parameters/Parameters.h"
#include "./Cell/Cell.h"
#include "./Simulation/Simulation.h"
#include "./World/World.h"
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

int main()
{
	Material anotherMaterial("fcc", 2, 1, 1, 2.1, 100);
	Parameters myParameters(1, 1, 10, 10, 10, 10000, 10, false, true, anotherMaterial);
	World myWorld(myParameters);

	char exit;

	cin >> exit;

	return 0;
}
