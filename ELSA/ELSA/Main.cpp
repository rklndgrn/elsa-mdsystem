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
	//Material anotherMaterial("fcc", 408.53e-12, 0.34*(1.6021766208E-19), 2.65e-10, 1.5*408.53e-12, 39.948*(1.660539040e-27));
	//Parameters myParameters(1e-14, 20e-14, 5, 5, 5, 10, 10, false, false, anotherMaterial);
	//World myWorld(myParameters);
	visual myVis;
	bool myWorldDefined{ false };
	Gui myGui;
	myGui.setupGui(myVis.getWindow());

	ofstream myFilePos;
	myFilePos.open("Erik.txt");
	while ((glfwGetKey(myVis.getWindow(), GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(myVis.getWindow()) == 0))// || myGui.exitPressed() )
	{
		myGui.handleFrame();

		if (myGui.VisualVisible())
			myVis.setAtomsVisible(true);
		else
			myVis.setAtomsVisible(false);

		myVis.mainLoopVisual(myGui._pos);

		// -------------------------------- MENU -----------------------------------------

		myGui.handleMenu(0,1); //1,1 does nothing
		

		if (myGui.simulate())
		{
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
				myGui.getCollisionFrequency(),
				myGui.isThermostat(),
				myGui.is2D(),
				myMaterial);

			World myWorld(myParameters); myWorldDefined = true;

			

			double deltaT = myParameters.getTimeStep();
			double** potArray = myWorld.getResults().getPotentialEnergy();
			double** kinArray = myWorld.getResults().getKineticEnergy();
			double** totArray = myWorld.getResults().getTotalEnergy();
			double** tempArray = myWorld.getResults().getTemperature();
			

			int index{ 0 };

			myGui._initializing = false;

			for (double t = deltaT; t < myParameters.getSimulationTime() - 0.5*deltaT; t += deltaT)
			{
				if (myGui.simulate())
				{
					glUseProgram(0);
					ImGui::Render();

					int display_w, display_h;
					glfwMakeContextCurrent(myVis.getWindow());
					glfwGetFramebufferSize(myVis.getWindow(), &display_w, &display_h);
					glViewport(0, 0, display_w, display_h);
					ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

					glfwPollEvents();
					glfwSwapBuffers(myVis.getWindow());
					if (deltaT > 0)
					{
						myWorld.updateCells();
						myWorld.updateNeighbourList();
					}

					myWorld.calcPotentialAndForce(t);
					myWorld.solveEquationsOfMotion(t);
					myWorld.calcPressure(t);

					myGui.handleFrame();
					myGui.handleMenu(t, myParameters.getSimulationTime());
				}

			}
			double**** positions = myWorld.getResults().getPositions();

			double*** pos = *positions;
			for (double t{ 0 }; t < myParameters.getSimulationTime() - 0.5*deltaT; t += deltaT) {
				for (int i{ 0 }; i < 27; i++) {
					index = (int)round(t / deltaT);
					myFilePos << pos[index][i][0] << " " << pos[index][i][1] << " " << pos[index][i][2] << " ";
					printf("Pos x: %f y: %f z: %f at time index %i\n", pos[index][i][0], pos[index][i][1], pos[index][i][2], index);
				}
			}
			myGui.stopSimulate();
			myFilePos.close();
			myGui._kineticEnergy = *kinArray;
			myGui._totalEnergy = *totArray;
			myGui._potentialEnergy = *potArray;
			myGui._temp = *tempArray;
			myGui._pos = *positions;
		}
		//myVis.mainLoopVisual();
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

	myFilePos.close();

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(myVis.getWindow());
	glfwTerminate();

	return 0;
}
