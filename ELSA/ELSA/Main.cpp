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
	Material myMaterial;
	Parameters myParameters;
	World myWorld;

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
	while ((glfwGetKey(myVis.getWindow(), GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(myVis.getWindow()) == 0) && !myGui.exitPressed())
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
			myMaterial = Material(
				myGui.getCrystalType(),
				myGui.getLatticeConstant(),
				myGui.getEpsilon(),
				myGui.getSigma(),
				myGui.getCutOffDistance(),
				myGui.getMass());

			myParameters = Parameters(
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

			myWorld = World(myParameters, myGui.getNumberOfThreads());
			//cout << "Here we go!" << endl;

			double deltaT = myParameters.getTimeStep();

			int index{ 0 };

			myGui._initializing = false;

			for (double t = deltaT; t < myParameters.getSimulationTime() - 0.5*deltaT; t += deltaT)
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
				//cout << "Z-position of some random particle is " << myWorld.getResults().getPositions()[15][676][2] << endl;
					//---//
					//for each (Atom* a in myWorld.getAtomInAtomList(103)->getNeighbourList())
					//{
					//	printf("%d ", a->getID());
					//}
					//printf("\n");

				myGui.handleFrame();
				myGui.handleMenu(t, myParameters.getSimulationTime(), &visualTime, maxVisualTime, &speed);
				/*
				if (t >= myParameters.getSimulationTime() - deltaT && t < myParameters.getSimulationTime())
				{
					Results r = myWorld.getResults();
					int aL = r.getArrayLength();
					int N = myParameters.getNumberOfAtoms();
					myGui.updateArrays(aL, N);
					for (int x = 0; x < aL; x++)
					{
						myGui._cohesiveEnergy[x] = r.getCohesiveEnergy()[x];
						myGui._debyeTemperature[x] = r.getDebyeTemperature()[x];
						myGui._kineticEnergy[x] = r.getKineticEnergy()[x];
						myGui._meanSquareDisplacement[x] = r.getMeanSquareDisplacement()[x];
						myGui._potentialEnergy[x] = r.getPotentialEnergy()[x];
						myGui._pressure[x] = r.getInternalPressure()[x];
						myGui._selfDiffusionCoeff[x] = r.getDiffusionConstant()[x];
						myGui._specificHeat[x] = r.getSpecificHeat()[x];
						myGui._temp[x] = r.getTemperature()[x];
						myGui._totalEnergy[x] = r.getTotalEnergy()[x];
						for (int n = 0; n < N; n++)
						{
							myGui._pos[x][n][0] = r.getPositions()[x][n][0];
							myGui._pos[x][n][1] = r.getPositions()[x][n][1];
							myGui._pos[x][n][2] = r.getPositions()[x][n][2];
						}
					}
					myGui.setNumberOfTimeStepsPlot((int)round(myParameters.getSimulationTime() / myParameters.getTimeStep()));
				}
				*/


			}
			myGui.stopSimulate();
			myGui.setMainVisible(true);

			Results r = myWorld.getResults();
			int aL = r.getArrayLength();
			int N = myParameters.getNumberOfAtoms();
			myGui.updateArrays(aL, N);
			for (int x = 0; x < aL; x++)
			{
				myGui._cohesiveEnergy[x] = r.getCohesiveEnergy()[x];
				myGui._debyeTemperature[x] = r.getDebyeTemperature()[x];
				myGui._kineticEnergy[x] = r.getKineticEnergy()[x];
				myGui._meanSquareDisplacement[x] = r.getMeanSquareDisplacement()[x];
				myGui._potentialEnergy[x] = r.getPotentialEnergy()[x];
				myGui._pressure[x] = r.getInternalPressure()[x];
				myGui._selfDiffusionCoeff[x] = r.getDiffusionConstant()[x];
				myGui._specificHeat[x] = r.getSpecificHeat()[x];
				myGui._temp[x] = r.getTemperature()[x];
				myGui._totalEnergy[x] = r.getTotalEnergy()[x];
				for (int n = 0; n < N; n++)
				{
					myGui._pos[x][n][0] = r.getPositions()[x][n][0];
					myGui._pos[x][n][1] = r.getPositions()[x][n][1];
					myGui._pos[x][n][2] = r.getPositions()[x][n][2];
				}
			}
			myGui.setNumberOfTimeStepsPlot((int)round(myParameters.getSimulationTime() / myParameters.getTimeStep()));

			Atom* a;
			ofstream saveLastState;
			saveLastState.open("./SaveData/lastState.txt");

			for (unsigned int i = 0; i < myParameters.getNumberOfAtoms(); i++)
			{
				a = myWorld.getAtomInAtomList(i);

				saveLastState << a->getID() << " ";
				saveLastState << a->getPositionX() << " " << a->getPositionY() << " " << a->getPositionZ() << " ";
				saveLastState << a->getVelocityX() << " " << a->getVelocityY() << " " << a->getVelocityZ() << " ";
			}

			saveLastState.close();

			/*
			//cout << "RESULTS MUST NOT BE DESTROYED!" << endl;
			myGui._cohesiveEnergy = myWorld.getResults().getCohesiveEnergy();
			myGui._debyeTemperature = myWorld.getResults().getDebyeTemperature();
			myGui._kineticEnergy = myWorld.getResults().getKineticEnergy();
			myGui._meanSquareDisplacement = myWorld.getResults().getMeanSquareDisplacement();
			myGui._pos = myWorld.getResults().getPositions();
			//cout << "Z-position of some random particle is " << myGui._pos[15][676][2] << endl;
			myGui._potentialEnergy = myWorld.getResults().getPotentialEnergy();
			myGui._pressure = myWorld.getResults().getInternalPressure();
			myGui._selfDiffusionCoeff = myWorld.getResults().getDiffusionConstant();
			myGui._specificHeat = myWorld.getResults().getSpecificHeat();
			myGui._temp = myWorld.getResults().getTemperature();
			myGui._totalEnergy = myWorld.getResults().getTotalEnergy();
			myGui.setNumberOfTimeStepsPlot((int)round(myParameters.getSimulationTime() / myParameters.getTimeStep()));
			*/

			maxVisualTime = (int)round(myParameters.getSimulationTime() / myParameters.getTimeStep());
			latticeConstant = myParameters.getChosenMaterial().getLatticeConstant();
			unitCellsX = myParameters.getNumberOfUnitCellsX();
			unitCellsY = myParameters.getNumberOfUnitCellsY();
			unitCellsZ = myParameters.getNumberOfUnitCellsZ();

			myVis.setNumberOfParticles(myParameters.getNumberOfAtoms());
			//cout << "WE MADE IT!" << endl;
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
	myGui.updateArrays(0, 0);
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(myVis.getWindow());
	glfwTerminate();

	return 0;
}