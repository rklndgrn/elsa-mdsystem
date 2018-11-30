#include "./Atom/Atom.h"
#include "./Material/Material.h"
#include "./Parameters/Parameters.h"
#include "./Cell/Cell.h"
#include "./Simulation/Simulation.h"
#include "./World/World.h"
#include "visual.h"

#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

int main()
{
	cout << "Hello!" << endl;

	if (true)
	{
		Atom r1(0, 0, 0, 0);
		Atom r2{ r1 };
		Atom r3{ 1, 5, 5, 5 };
		r1.addToNeighbourList(&r3);
		r2.addToNeighbourList(&r3);
		Atom r4 = r3;

		cout << "ID for r1: " << r1.getID() << endl;
		cout << "ID for r2: " << r2.getID() << endl;

		cout << "Neighbour list size for r1: " << r1.getNeighbourList().size() << endl;
		cout << "Neighbour list size for r2: " << r2.getNeighbourList().size() << endl;

		cout << "Array length for r3: " << r3.getID() << endl;
		cout << "Array length for r4: " << r4.getID() << endl;

		cout << "Neighbour list size for r3: " << r3.getNeighbourList().size() << endl;
		cout << "Neighbour list size for r4: " << r4.getNeighbourList().size() << endl;

		cout << "ID for element in r1s neighbour list: " << &r1.getNeighbourList().at(0) << endl;
		cout << "ID for element in r2s neighbour list: " << &r2.getNeighbourList().at(0) << endl;
	}

	char c;

	cin >> c;


	/*
	visual myVis;

	Gui myGui;

	myGui.setupGui(myVis.getWindow());

	int count = 0;
	int visualTime = 0;
	int maxVisualTime = 1;
	int speed = 8;
	double latticeConstant = 1.0;
	int unitCellsX = 1;
	int unitCellsY = 1;
	int unitCellsZ = 1;
	int hello{ 0 };
	while ((glfwGetKey(myVis.getWindow(), GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(myVis.getWindow()) == 0))// || myGui.exitPressed() )
	{
		myGui.handleFrame();

		if (myGui.VisualVisible())
		{
			myVis.setAtomsVisible(true);
			count++;
			if (speed == 0)
			{

			}
			else if (count % (9 - speed) == 0)
			{
				visualTime++;
			}
			if (visualTime >= maxVisualTime)
			{
				visualTime = 0;
			}
		}
		else
		{
			myVis.setAtomsVisible(false);
			visualTime = 0;
		}
		myVis.mainLoopVisual(myGui._pos, visualTime, maxVisualTime, latticeConstant, unitCellsX, unitCellsY, unitCellsZ);

		
		// -------------------------------- MENU -----------------------------------------

		myGui.handleMenu(0, 1, &visualTime, maxVisualTime, &speed); //1,1 does nothing
		

		if (myGui.simulate())
		{
			myGui.setMainVisible(false);
			myGui._initializing = true;
			Material myMaterial(
				myGui.getCrystalType(),
				myGui.getLatticeConstant(),
				myGui.getEpsilon(),
				myGui.getSigma(),
				myGui.getCutOffDistance(),
				myGui.getMass());

			Parameters myParameters(
				myGui.getTimeStep(),
				myGui.getSimulationTime(),
				myGui.getNumberOfUnitCellsX(),
				myGui.getNumberOfUnitCellsY(),
				myGui.getNumberOfUnitCellsZ(),
				myGui.getTemperature(),
				myGui.getCollisionPercentage(),
				myGui.isThermostat(),
				myGui.isUseLastSimulationState(),
				myGui.is2D(),
				myGui.getLastStateFileName(),
				myMaterial);

			//Results myResults( myParameters.getSimulationTime(), myParameters.getTimeStep(), myParameters.getNumberOfAtoms() );
			Simulation mySimulation(myParameters.getChosenMaterial());

			cout << "Loop iteration " << hello << "!" << endl;
			World myWorld(myParameters, mySimulation, myGui.getNumberOfThreads());
			cout << "Iteration " << hello << " made it!" << endl;
			hello++;

			double deltaT = myParameters.getTimeStep();

			int index{ 0 };

			myGui._initializing = false;

			for (double t = deltaT; t < myParameters.getSimulationTime() - 0.5*deltaT; t += deltaT)
			{
				if (myGui.simulate())
				{
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					glUseProgram(0);
					ImGui::Render();

					//myGui.handleProgressBar(t,myParameters.getSimulationTime());

					int display_w, display_h;
					glfwMakeContextCurrent(myVis.getWindow());
					glfwGetFramebufferSize(myVis.getWindow(), &display_w, &display_h);
					glViewport(0, 0, display_w, display_h);
					ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

					glfwPollEvents();
					glfwSwapBuffers(myVis.getWindow());

					//Here's where the magic happens
					myWorld.performSimulation(t, 10);
					//---//
					//for each (Atom* a in myWorld.getAtomInAtomList(103)->getNeighbourList())
					//{
					//	printf("%d ", a->getID());
					//}
					//printf("\n");

					myGui.handleFrame();
					myGui.handleMenu(t, myParameters.getSimulationTime(), &visualTime, maxVisualTime, &speed);
				}

			}

			myGui.stopSimulate();
			myGui.setMainVisible(true);

			Atom* a;
			ofstream saveLastState;
			saveLastState.open("./SaveData/lastState.txt");

			for(unsigned int i = 0; i < myParameters.getNumberOfAtoms(); i++)
			{
				a = myWorld.getAtomInAtomList(i);

				saveLastState << a->getID() << " ";
				saveLastState << a->getPositionX() << " " << a->getPositionY() << " " << a->getPositionZ() << " ";
				saveLastState << a->getVelocityX() << " " << a->getVelocityY() << " " << a->getVelocityZ() << " ";
			}

			saveLastState.close();

			myGui._cohesiveEnergy = *(myWorld.getResults().getCohesiveEnergy());
			myGui._debyeTemperature = *(myWorld.getResults().getDebyeTemperature());
			myGui._kineticEnergy = *(myWorld.getResults().getKineticEnergy());
			myGui._meanSquareDisplacement = *(myWorld.getResults().getMeanSquareDisplacement());
			myGui._pos = *(myWorld.getResults().getPositions());
			myGui._potentialEnergy = *(myWorld.getResults().getPotentialEnergy());
			myGui._pressure = *(myWorld.getResults().getInternalPressure());
			myGui._selfDiffusionCoeff = *(myWorld.getResults().getDiffusionConstant());
			myGui._specificHeat = *(myWorld.getResults().getSpecificHeat());
			myGui._temp = *(myWorld.getResults().getTemperature());
			myGui._totalEnergy = *(myWorld.getResults().getTotalEnergy());
			myGui.setNumberOfTimeStepsPlot((int)round(myParameters.getSimulationTime()/myParameters.getTimeStep()));

			maxVisualTime = (int)round(myParameters.getSimulationTime() / myParameters.getTimeStep());
			latticeConstant = myParameters.getChosenMaterial().getLatticeConstant();
			unitCellsX = myParameters.getNumberOfUnitCellsX();
			unitCellsY = myParameters.getNumberOfUnitCellsY();
			unitCellsZ = myParameters.getNumberOfUnitCellsZ();

			myVis.setNumberOfParticles(myParameters.getNumberOfAtoms());
			cout << "WE MADE IT!" << endl;
		}

		//myGui.handlePlots();// , kinenen, totenen, tempen);

		//bool tesr = false;
		//ImGui::ShowDemoWindow(&tesr);

		// Rendering
		glUseProgram(0);
		ImGui::Render();

		int display_w, display_h;
		glfwMakeContextCurrent(myVis.getWindow());
		glfwGetFramebufferSize(myVis.getWindow(), &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwPollEvents();
		glfwSwapBuffers(myVis.getWindow());
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(myVis.getWindow());
	glfwTerminate();
	*/

	return 0;
}
