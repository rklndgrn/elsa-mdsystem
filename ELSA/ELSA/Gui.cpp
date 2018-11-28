#include "Gui.h"

Gui::Gui()
{
	_name = "ELSA";
	_plotVisible = false;
	_visualVisible = false;
	_mainVisible = true;
	_exitPressed = false;
	_crystalType = "fcc";
	_numberOfThreads = omp_get_max_threads();
	_latticeConstant = 408.53e-12;
	_epsilon = 0.34*(1.6021766208E-19);
	_sigma = 2.65e-10;
	_cutOffDistance = 1.5*408.53e-12;
	_mass = 39.948*(1.660539040e-27);
	_timeStep = 1e-15;
	_simulationTime = 20e-15;
	_numberOfUnitCellsX = 10;
	_numberOfUnitCellsY = 10;
	_numberOfUnitCellsZ = 10;
	_temperature = 10;
	_collisionPercentage = 0.2;
	_thermostat = false;
	_useLastSimulationState = false;
	_lastStateFileName = "lastState.txt";
	_2D = false;
}

void Gui::setupGui(GLFWwindow *window)
{

	const char* glsl_version = "#version 330";

	//Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	ImGui::StyleColorsLight();

	io.Fonts->AddFontFromFileTTF("imgui-master/misc/fonts/Roboto-Medium.ttf", 16.0f);
}

void Gui::show()
{
	_visible = true;
}

bool Gui::visible() const
{
	return _visible;
}

bool Gui::simulate() const
{
	return _simulate;
}

void Gui::stopSimulate()
{
	_simulate = false;
	_simulationWindow = false;
	_plotVisible = true;
}

void Gui::hide()
{
	_visible = false;
}

void Gui::showHelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

void Gui::printStatistics(double min, double max, double sumLast10)
{
	char buf[50];
	double avgLast10 = sumLast10 / 10.0;
	sprintf(buf, "Min: %e", min);
	ImGui::Text(buf);
	sprintf(buf, "Max: %e", max);
	ImGui::SameLine();
	ImGui::Text(buf);
	sprintf(buf, "Average last 10: %e", avgLast10);
	ImGui::SameLine();
	ImGui::Text(buf);
}

std::string Gui::getCrystalType()
{
	return _crystalType;
}

double Gui::getTemperature()
{
	return _temperature;
}

double Gui::getTimeStep()
{
	return _timeStep;
}

double Gui::getSimulationTime()
{
	return _simulationTime;
}

bool Gui::isThermostat()
{
	return _thermostat;
}

bool Gui::isUseLastSimulationState()
{
	return _useLastSimulationState;
}

void Gui::setMainVisible(bool vis)
{
	_mainVisible = vis;
}

bool Gui::is2D()
{
	return _2D;
}

int Gui::getNumberOfUnitCellsX()
{
	return _numberOfUnitCellsX;
}

int Gui::getNumberOfUnitCellsY()
{
	return _numberOfUnitCellsY;
}

int Gui::getNumberOfUnitCellsZ()
{
	return _numberOfUnitCellsZ;
}

double Gui::getLatticeConstant()
{
	return _latticeConstant;
}

double Gui::getEpsilon()
{
	return _epsilon;
}

double Gui::getSigma()
{
	return _sigma;
}

double Gui::getCutOffDistance()
{
	return _cutOffDistance;
}

double Gui::getMass()
{
	return _mass;
}

int Gui::getNumberOfThreads()
{
	return _numberOfThreads;
}

double Gui::getCollisionPercentage()
{
	return _collisionPercentage;
}

char* Gui::getLastStateFileName()
{
	return _lastStateFileName;
}

bool Gui::showCrystalSelector(const char* label)
{
	static int style_idx = 1;
	const char* items[] = { "SC", "FCC" };
	if (ImGui::Combo(label, &style_idx, items, IM_ARRAYSIZE(items)))
	{
		switch (style_idx)
		{
		case 0: _crystalType = "sc"; break;
		case 1: _crystalType = "fcc"; break;
		}
		return true;
	}
	return false;
}

bool Gui::showMaterialSelector(const char* label)
{
	static int style_idx = 0;
	const char* items[] = { "Ag: Silver", "Ar: Argon", "He: Helium", "Kr: Krypton", "Ne: Neon", "Xe: Xenon"};
	double boltzmann = 1.38064852e-23;
	double unitMass = (1.660539040e-27);
	if (ImGui::Combo(label, &style_idx, items, IM_ARRAYSIZE(items)))
	{
		switch (style_idx)
		{
			// Data for Silver
		case 0: _latticeConstant = 408.53e-12;
			_epsilon = 0.34*_elementaryCharge;
			_sigma = 2.65e-10;
			_mass = 107.8682*unitMass;
			break;
			// Data for Argon
		case 1: _latticeConstant = 525.6e-12;
			_epsilon = 0.0104*_elementaryCharge;
			_sigma = 3.40e-10;
			_mass = 39.948*unitMass;
			break;
			// Data for Helium (source: https://www.slideshare.net/TzuPingChen1/helium-gas-with-lennardjones-potential-in-mcmd-70967262)
		case 2: _latticeConstant = 424.4e-12;
			_epsilon = 10.22*_elementaryCharge;
			_sigma = 2.556e-10;
			_mass = 4.0026002*unitMass;
			break;
			//Data for Krypton
		case 3: _latticeConstant = 570.6e-12;
			_epsilon = 0.0140*_elementaryCharge;
			_sigma = 3.65e-10;
			_mass = 83.798*unitMass;
			break;
			//Data for Neon
		case 4: _latticeConstant = 442.9e-12;
			_epsilon = 0.0031*_elementaryCharge;
			_sigma = 2.74e-10;
			_mass = 20.1797*unitMass;
			break;
			//Data for Xenon.
		case 5: _latticeConstant = 620.23e-12;
			_epsilon = 0.02*_elementaryCharge;
			_sigma = 3.98e-10;
			_mass = 131.293*unitMass;
			break;
		}

		return true;
	}
	return false;
}

bool Gui::showThreadSelector(const char* label)
{
	static int style_idx = _numberOfThreads;
	const char* items[] = {"1", "2", "3", "4", "5", "6", "7", "8"};
	//const char* items = items_tmp;
	if (ImGui::Combo(label, &style_idx, items, IM_ARRAYSIZE(items)))
	{
		switch (style_idx)
		{
		case 1: _numberOfThreads = 1;
				break;
		case 2: _numberOfThreads = 2;
			break;
		case 3: _numberOfThreads = 3;
			break;
		case 4: _numberOfThreads = 4;
			break;
		case 5: _numberOfThreads = 5;
			break;
		case 6: _numberOfThreads = 6;
			break;
		case 7: _numberOfThreads = 7;
			break;
		case 8: _numberOfThreads = 8;
			break;
		}

		return true;
	}
	return false;
}

void Gui::handleFrame()
{
	//Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Gui::handleMenu(double elapsedTime, double totalTime, int* visualisationTime, int maxVisualTime, int* speed)
{
	//Menu
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("Open result file", NULL, &_loadResultWindow);
			ImGui::MenuItem("Save result file", NULL, &_saveResultWindow);
	
			if (ImGui::MenuItem("Exit", NULL)) { _exitPressed = true; }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			ImGui::MenuItem("Main window", NULL, &_mainVisible);
			//ImGui::MenuItem("Visualization window", NULL, &show_visual_window);
			ImGui::MenuItem("Plot window", NULL, &_plotVisible);
			ImGui::MenuItem("Visualization", NULL, &_visualVisible);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::BeginMenu("Style"))
			{
				if (ImGui::MenuItem("Light", "Ctrl+L"))
				{
					ImGui::StyleColorsLight();
					glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
				}
				if (ImGui::MenuItem("Dark", "Ctrl+D"))
				{
					ImGui::StyleColorsDark();
					glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
				}
				if (ImGui::MenuItem("Classic", "Ctrl+C"))
				{
					ImGui::StyleColorsClassic();
					glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	ImGuiWindowFlags window_flags = 0;
	//window_flags |= ImGuiWindowFlags_MenuBar;
	if (_mainVisible && !simulate())
	{
		ImGui::Begin("ELSA MD-system", &_mainVisible, window_flags);
		ImGui::SetWindowSize(ImVec2(560, 640));
		ImGui::SetWindowPos(ImVec2(60, 140));
		handleCollapsingHeaders();
		simulateButtonHandler();
		//stopButtonHandler();
		
		//if (_simulate)
		//{
			//ImGui::Text("Simulating...");// , _counter);
			//handleProgressBar(elapsedTime, totalTime);
			
		//}
		ImGui::End();
	}
	handlePlots();
	saveResultsWindow();
	loadResultsWindow();
	visualWindow(visualisationTime, maxVisualTime, speed);
	simulationWindow(elapsedTime, totalTime);
}

void Gui::visualWindow(int* visualisationTime, int maxVisualTime, int* speed)
{
	bool auto_resize = false;
	//ImGuiWindowFlags window_flags = 0;
	ImGuiWindowFlags flags = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : 0;
	flags |= ImGuiWindowFlags_NoResize;
	if (_visualVisible)
	{
		ImGui::Begin("Visualization", &_visualVisible, flags);
		ImGui::SetWindowSize(ImVec2(1200, 150));
		ImGui::Text("Strafe by using w,a,s & d");
		ImGui::Text("Zoom by using q & e");
		ImGui::Text("Rotate by using i, j, k & l");
		ImGui::SliderInt(" ", visualisationTime, 0, maxVisualTime, "Time: %d");
		ImGui::SliderInt("", speed, 0, 8, "Speed: %d");
		ImGui::SameLine();

		//if (ImGui::Button("Pause"))
		//{
		//	_isVisualisationPause = !_isVisualisationPause;
		//}
		//if (_isVisualisationPause)
		//{
		//	_saveSpeed = *speed;
		//	*speed = 1000;
		//}
		//else
		//{
		//	*speed = _saveSpeed;
		//}
		ImGui::End();
	}


}

void Gui::simulationWindow(double elapsedTime, double totalTime)
{
	bool auto_resize = false;
	//ImGuiWindowFlags window_flags = 0;
	ImGuiWindowFlags flags = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : 0;
	flags |= ImGuiWindowFlags_NoResize;
	if (_simulationWindow)
	{
		ImGui::Begin("Simulation running...", &_simulationWindow, flags);
		ImGui::SetWindowSize(ImVec2(900, 60));
		handleProgressBar(elapsedTime, totalTime);
		stopButtonHandler();
		//if (!_simulate) { _simulationWindow = false; _simulate = false; }
		ImGui::End();
	}
}

void Gui::loadResultsWindow()
{
	bool auto_resize = false;
	//ImGuiWindowFlags window_flags = 0;
	ImGuiWindowFlags flags = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : 0;
	flags |= ImGuiWindowFlags_NoResize;
	if (_loadResultWindow)
	{
		ImGui::Begin("Load results file...", &_loadResultWindow, flags);
		ImGui::SetWindowSize(ImVec2(500, 110));
		ImGui::Text("Enter the name of the file you want to open:");
		static char buf1[64] = "./SaveData/"; ImGui::InputText(".txt", buf1, 64);
		if (ImGui::Button("Load")) {
			strcat(buf1, ".txt");
			std::ifstream myFile;
			myFile.open(buf1); 
			if (!myFile.is_open()) { _unableToOpenFile = true; }
			else {
				std::string dummy;
				int sizeOfFile{ 0 }, i{ 0 };
				while (!myFile.eof()) { getline(myFile, dummy); sizeOfFile++; }
				myFile.close(); myFile.open(buf1);
				if (_temp == NULL) { _temp = new double[sizeOfFile]; }
				if (_totalEnergy == NULL) { _totalEnergy = new double[sizeOfFile]; }
				if (_potentialEnergy == NULL) { _potentialEnergy = new double[sizeOfFile]; }
				if (_kineticEnergy == NULL) { _kineticEnergy = new double[sizeOfFile]; }
				std::getline(myFile, dummy);
				while (!myFile.eof())
				{
					myFile >> _temp[i];
					myFile >> _totalEnergy[i];
					myFile >> _potentialEnergy[i];
					myFile >> _kineticEnergy[i];
					i++;
				}
				myFile.close();
				_loadResultWindow = false;
				_unableToOpenFile = false;
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel")) { _loadResultWindow = false; };
		if(_unableToOpenFile) { ImGui::Text("Unable to open file..."); }
		ImGui::End();
	}
}

void Gui::saveResultsWindow()
{
	bool auto_resize = false;
	//ImGuiWindowFlags window_flags = 0;
	ImGuiWindowFlags flags = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : 0;
	flags |= ImGuiWindowFlags_NoResize;
	if (_saveResultWindow)
	{
		ImGui::Begin("Save results file...", &_saveResultWindow, flags);
		ImGui::SetWindowSize(ImVec2(500, 110));
		ImGui::Text("Enter the name of the file you want to save to:");
		static char buf1[64] = "./SaveData/"; ImGui::InputText(".txt", buf1, 64);
		if (ImGui::Button("Save")) {
			strcat(buf1, ".txt");
			std::ofstream myFile;
			myFile.open(buf1);

			myFile << "Temperature [K] " << "Total energy [J] " << "Potential energy [J] " << "Kinetic energy [J] " << std::endl;
				for (int i = 0; i < (int)(floor(_simulationTime / _timeStep)); i++)
				{
					myFile << _temp[i] << " " << _totalEnergy[i] << " " << _potentialEnergy[i] << " " << _kineticEnergy[i] << std::endl;
				}
				myFile.close();
				_saveResultWindow = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel")) { _saveResultWindow = false; };
		ImGui::End();
	}
}

void Gui::handlePlots()
{
	bool auto_resize = false;
	int numberOfTimeSteps = (int)round(_simulationTime / _timeStep);
	//ImGuiWindowFlags window_flags = 0;
	ImGuiWindowFlags flags = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : 0;
	flags |= ImGuiWindowFlags_NoResize;

	if (_plotVisible)
	{
		ImGui::Begin("Results", &_plotVisible, flags);
		ImGui::SetWindowSize(ImVec2(glfwGetVideoMode(glfwGetPrimaryMonitor())->width, glfwGetVideoMode(glfwGetPrimaryMonitor())->height - 22));
		ImGui::SetWindowPos(ImVec2(0, 22));


		ImGui::Text("Potential energy: \n %E", _potentialEnergy[numberOfTimeSteps - 1]);
		ImGui::SameLine();
		ImGui::Text("Kinetic energy: \n %E", _kineticEnergy[numberOfTimeSteps - 1]);
		ImGui::SameLine();
		ImGui::Text("Total energy: \n %E", _totalEnergy[numberOfTimeSteps - 1]);
		ImGui::SameLine();
		ImGui::Text("Temperature: \n %E", _temp[numberOfTimeSteps - 1]);

		if (ImGui::CollapsingHeader("Cohesive energy"))
		{
			float cohEnEnD[_maxTimeSteps];
			float max = -1E15;
			float min = 1E15;
			float sumLast10 = 0;

			for (int i = 0; i < numberOfTimeSteps - 1; i++)
			{
				cohEnEnD[i] = static_cast<float>(_cohesiveEnergy[i + 1]) / _elementaryCharge;
				if (cohEnEnD[i] > max) { max = cohEnEnD[i]; }
				else if (cohEnEnD[i] < min) { min = cohEnEnD[i]; }
				if (i >= numberOfTimeSteps - 11 && i < numberOfTimeSteps - 1)
				{
					sumLast10 += cohEnEnD[i];
				}
			}

			ImGui::PlotLines("", cohEnEnD, numberOfTimeSteps - 1, 0, "Cohesive energy [eV]", min, max, ImVec2(1700, 480));
			printStatistics(min, max, sumLast10);
		}
		if (ImGui::CollapsingHeader("Debye Temperature"))
		{
			float debyeTempEnD[_maxTimeSteps];
			float max = -1E15;
			float min = 1E15;
			float sumLast10 = 0;

			for (int i = 0; i < numberOfTimeSteps - 1; i++)
			{
				debyeTempEnD[i] = static_cast<float>(_debyeTemperature[i + 1]);
				if (debyeTempEnD[i] > max) { max = debyeTempEnD[i]; }
				else if (debyeTempEnD[i] < min) { min = debyeTempEnD[i]; }
				if (i >= numberOfTimeSteps - 11 && i < numberOfTimeSteps - 1)
				{
					sumLast10 += debyeTempEnD[i];
				}
			}

			ImGui::PlotLines("", debyeTempEnD, numberOfTimeSteps - 1, 0, "Debye Temperature [K]", min, max, ImVec2(1700, 480));
			printStatistics(min, max, sumLast10);

		}
		if (ImGui::CollapsingHeader("Internal pressure"))
		{
			float pressureEnD[_maxTimeSteps];
			float max = -1E15;
			float min = 1E15;
			float sumLast10 = 0;

			for (int i = 0; i < numberOfTimeSteps - 1; i++)
			{
				pressureEnD[i] = static_cast<float>(_pressure[i + 1]);
				if (pressureEnD[i] > max) { max = pressureEnD[i]; }
				else if (pressureEnD[i] < min) { min = pressureEnD[i]; }
				if (i >= numberOfTimeSteps - 11 && i < numberOfTimeSteps - 1)
				{
					sumLast10 += pressureEnD[i];
				}
			}

			ImGui::PlotLines("", pressureEnD, numberOfTimeSteps - 1, 0, "Internal pressure [Pa]", min, max, ImVec2(1700, 480));
			printStatistics(min, max, sumLast10);

		}
		if (ImGui::CollapsingHeader("Kinetic energy"))
		{
			float kinEnD[_maxTimeSteps];
			float max = -1E15;
			float min = 1E15;
			float sumLast10 = 0;

			for (int i = 0; i < numberOfTimeSteps - 1; i++)
			{
				kinEnD[i] = static_cast<float>(_kineticEnergy[i + 1]) / _elementaryCharge;
				if (kinEnD[i] > max) { max = kinEnD[i]; }
				else if (kinEnD[i] < min) { min = kinEnD[i]; }
				if (i >= numberOfTimeSteps - 11 && i < numberOfTimeSteps - 1)
				{
					sumLast10 += kinEnD[i];
				}
			}
			ImGui::PlotLines("", kinEnD, numberOfTimeSteps - 1, 0, "Kinetic energy [eV]", min, max, ImVec2(1700, 480));
			printStatistics(min, max, sumLast10);
		}
		if (ImGui::CollapsingHeader("Mean square displacement"))
		{
			float msdEnD[_maxTimeSteps];
			float max = -1E15;
			float min = 1E15;
			float sumLast10 = 0;

			for (int i = 0; i < numberOfTimeSteps - 1; i++)
			{
				msdEnD[i] = 1e20*static_cast<float>(_meanSquareDisplacement[i + 1]);
				if (msdEnD[i] > max) { max = msdEnD[i]; }
				else if (msdEnD[i] < min) { min = msdEnD[i]; }
				if (i >= numberOfTimeSteps - 11 && i < numberOfTimeSteps - 1)
				{
					sumLast10 += msdEnD[i];
				}
			}

			ImGui::PlotLines("", msdEnD, numberOfTimeSteps - 1, 0, "Mean square displacement [Angstrom^2]", min, max, ImVec2(1700, 480));
			printStatistics(min, max, sumLast10);

		}
		if (ImGui::CollapsingHeader("Potential energy"))
		{
			float potEnD[_maxTimeSteps];
			float max = -1E15;
			float min = 1E15;
			float sumLast10 = 0;

			for (int i = 0; i < numberOfTimeSteps - 1; i++)
			{
				potEnD[i] = static_cast<float>(_potentialEnergy[i + 1]) / _elementaryCharge;
				if (potEnD[i] > max) { max = potEnD[i]; }
				else if (potEnD[i] < min) { min = potEnD[i]; }

				if (i >= numberOfTimeSteps - 11 && i < numberOfTimeSteps - 1)
				{
					sumLast10 += potEnD[i];
				}
			}

			ImGui::PlotLines("", potEnD, numberOfTimeSteps - 1, 0, "Potential energy [eV]", min, max, ImVec2(1700, 480));

			printStatistics(min, max, sumLast10);
		}
		if (ImGui::CollapsingHeader("Self diffusion coefficient"))
		{
			float selfDiffEnD[_maxTimeSteps];
			float max = -1E15;
			float min = 1E15;
			float sumLast10 = 0;

			for (int i = 0; i < numberOfTimeSteps - 1; i++)
			{
				selfDiffEnD[i] = static_cast<float>(_selfDiffusionCoeff[i + 1]);
				if (selfDiffEnD[i] > max) { max = selfDiffEnD[i]; }
				else if (selfDiffEnD[i] < min) { min = selfDiffEnD[i]; }
				if (i >= numberOfTimeSteps - 11 && i < numberOfTimeSteps - 1)
				{
					sumLast10 += selfDiffEnD[i];
				}
			}

			ImGui::PlotLines("", selfDiffEnD, numberOfTimeSteps - 1, 0, "Self diffusion coefficient [m^2 s^-1]", min, max, ImVec2(1700, 480));
			printStatistics(min, max, sumLast10);

		}
		if (ImGui::CollapsingHeader("Specific heat"))
		{
			float specHeatEnD[_maxTimeSteps];
			float max = -1E15;
			float min = 1E15;
			float sumLast10 = 0;

			for (int i = 0; i < numberOfTimeSteps - 1; i++)
			{
				specHeatEnD[i] = static_cast<float>(_specificHeat[i + 1]);
				if (specHeatEnD[i] > max) { max = specHeatEnD[i]; }
				else if (specHeatEnD[i] < min) { min = specHeatEnD[i]; }
				if (i >= numberOfTimeSteps - 11 && i < numberOfTimeSteps - 1)
				{
					sumLast10 += specHeatEnD[i];
				}
			}

			ImGui::PlotLines("", specHeatEnD, numberOfTimeSteps - 1, 0, "Specific heat [J K^-1]", min, max, ImVec2(1700, 480));
			printStatistics(min, max, sumLast10);

		}

		if (ImGui::CollapsingHeader("Temperature"))
		{
			float temp[_maxTimeSteps];
			float max = -1E15;
			float min = 1E15;
			float sumLast10 = 0;

			for (int i = 0; i < numberOfTimeSteps - 1; i++)
			{
				temp[i] = static_cast<float>(_temp[i + 1]);
				if (temp[i] > max) { max = temp[i]; }
				else if (temp[i] < min) { min = temp[i]; }
				if (i >= numberOfTimeSteps - 11 && i < numberOfTimeSteps - 1)
				{
					sumLast10 += temp[i];
				}
			}
			ImGui::PlotLines("", temp, numberOfTimeSteps - 1, 0, "Temperature [K]", min, max, ImVec2(1700, 480));
			printStatistics(min, max, sumLast10);
		}
		if (ImGui::CollapsingHeader("Total energy"))
		{
			float totEn[_maxTimeSteps];
			float max = -1E5;
			float min = 1E5;
			float sumLast10 = 0;

			for (int i = 0; i < numberOfTimeSteps - 1; i++)
			{
				totEn[i] = static_cast<float>(_totalEnergy[i + 1]) /  _elementaryCharge;
				if (totEn[i] > max) { max = totEn[i]; }
				else if (totEn[i] < min) { min = totEn[i]; }
				if (i >= numberOfTimeSteps - 11 && i < numberOfTimeSteps - 1)
				{
					sumLast10 += totEn[i];
				}
			}
			//printf("Min: %f Max: %f", min, max);

			ImGui::PlotLines("", totEn, numberOfTimeSteps - 1, 0, "Total energy [eV]", min, max, ImVec2(1700, 480));
			printStatistics(min, max, sumLast10);
		}


		ImGui::End();
	}
}

void Gui::handleConfigurationHeader()
{

	if (ImGui::CollapsingHeader("Simulation parameters"))
	{
		ImGuiIO& io = ImGui::GetIO();

		if (showThreadSelector("Number of threads"))
			showThreadSelector("th");

		//if (showCrystalSelector("CrystalSelector"))
		//	showCrystalSelector("cr");*/

		//ImGui::Text("Temperature: ");
		//ImGui::SameLine();
		ImGui::InputDouble("Temperature [K]", &_temperature, 0.0f, 0.0f, "%e");
		ImGui::SameLine();
		showHelpMarker("You can input value using the scientific notation,\n  e.g. \"1e+8\" becomes \"100000000\".\n");

		//ImGui::Text("Time step length: ");
		//ImGui::SameLine();
		ImGui::InputDouble("Time step length [s]", &_timeStep, 0.0f, 0.0f, "%e");
		ImGui::SameLine();
		showHelpMarker("You can input value using the scientific notation,\n  e.g. \"1e+8\" becomes \"100000000\".\n");

		//ImGui::Text("Simulation time: ");
		//ImGui::SameLine();
		ImGui::InputDouble("Simulation time [s]", &_simulationTime, 0.0f, 0.0f, "%e");
		ImGui::SameLine();
		showHelpMarker("You can input value using the scientific notation,\n  e.g. \"1e+8\" becomes \"100000000\".\n");

		//ImGui::Text("Collision frequency: ");
		//ImGui::SameLine();
		ImGui::InputDouble("Collision probabilty [1]", &_collisionPercentage, 0.0f, 0.0f, "%e");
		ImGui::SameLine();
		showHelpMarker("You can input value using the scientific notation,\n  e.g. \"1e+8\" becomes \"100000000\".\n");

		//Cut off distance
		ImGui::InputDouble("Cut off distance [m]", &_cutOffDistance, 0.0f, 0.0f, "%e");
		ImGui::SameLine();
		showHelpMarker("You can input value using the scientific notation,\n  e.g. \"1e+8\" becomes \"100000000\".\n");

		//ImGui::Text("Number of unit cells x: ");
		//ImGui::SameLine();
		ImGui::InputInt("Number of unit cells x [1]", &_numberOfUnitCellsX);
		/*ImGui::SameLine();
		showHelpMarker("You can input value using the scientific notation,\n  e.g. \"1e+8\" becomes \"100000000\".\n");*/

		//ImGui::Text("Number of unit cells y: ");
		//ImGui::SameLine();
		ImGui::InputInt("Number of unit cells y [1]", &_numberOfUnitCellsY);
		/*ImGui::SameLine();
		showHelpMarker("You can input value using the scientific notation,\n  e.g. \"1e+8\" becomes \"100000000\".\n");*/

		//ImGui::Text("Number of unit cells z: ");
		//ImGui::SameLine();
		ImGui::InputInt("Number of unit cells z [1]", &_numberOfUnitCellsZ);
		/*ImGui::SameLine();
		showHelpMarker("You can input value using the scientific notation,\n  e.g. \"1e+8\" becomes \"100000000\".\n");*/

		//ImGui::Checkbox("Visualization", &_visualization);      // Edit bools storing our window open/close state
											
		ImGui::Checkbox("Anderson thermostat", &_thermostat);
		ImGui::Checkbox("2D simulation", &_2D);
		ImGui::Checkbox("Init from end of last simulation", &_useLastSimulationState);
		ImGui::SameLine();
		ImGui::InputText("", _lastStateFileName, 100);
	}

}
void Gui::handleSettingsHeader()
{
	if (ImGui::CollapsingHeader("Material parameters"))
	{
		ImGuiIO& io = ImGui::GetIO();

		if (showMaterialSelector("MaterialSelector"))
			showMaterialSelector("mt");

		if (showCrystalSelector("CrystalSelector"))
			showCrystalSelector("cr");


		//ImGui::Text("Lattice constant: ");
		//ImGui::SameLine();
		ImGui::InputDouble("Lattice constant [m]", &_latticeConstant, 0.0f, 0.0f, "%e");
		ImGui::SameLine();
		showHelpMarker("You can input value using the scientific notation,\n  e.g. \"1e+8\" becomes \"100000000\".\n");

		//ImGui::Text("Epsilon: ");
		//ImGui::SameLine();
		ImGui::InputDouble("Epsilon [m]", &_epsilon, 0.0f, 0.0f, "%e");
		ImGui::SameLine();
		showHelpMarker("You can input value using the scientific notation,\n  e.g. \"1e+8\" becomes \"100000000\".\n");

		//ImGui::Text("Sigma: ");
		//ImGui::SameLine();
		ImGui::InputDouble("Sigma [J]", &_sigma, 0.0f, 0.0f, "%e");
		ImGui::SameLine();
		showHelpMarker("You can input value using the scientific notation,\n  e.g. \"1e+8\" becomes \"100000000\".\n");

		//ImGui::Text("Cut off distance: ");
		//ImGui::SameLine();
		/*ImGui::InputDouble("Cut off distance [m]", &_cutOffDistance, 0.0f, 0.0f, "%e");
		ImGui::SameLine();
		showHelpMarker("You can input value using the scientific notation,\n  e.g. \"1e+8\" becomes \"100000000\".\n");*/

		//ImGui::Text("Mass: ");
		//ImGui::SameLine();
		ImGui::InputDouble("Mass [kg]", &_mass, 0.0f, 0.0f, "%e");
		ImGui::SameLine();
		showHelpMarker("You can input value using the scientific notation,\n  e.g. \"1e+8\" becomes \"100000000\".\n");
	}
}

void Gui::handleProgressBar(double elapsedTime, double totalTime)
{
	// Animate a simple progress bar
	static float progress = 0.0f, progress_dir = 1.0f;

	float quota = static_cast<float>(elapsedTime / totalTime);
	//printf("Progress: %f \n", quota);

	progress = progress_dir * quota;// *ImGui::GetIO().DeltaTime;

	// Typically we would use ImVec2(-1.0f,0.0f) to use all available width, or ImVec2(width,0.0f) for a specified width. ImVec2(0.0f,0.0f) uses ItemWidth.
	ImGui::ProgressBar(progress, ImVec2(0.0f, 0.0f));
	ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
	if (_simulate && !_initializing) { ImGui::Text("Simulating..."); }
	else { ImGui::Text("Initializing..."); }

}

void Gui::simulateButtonHandler()
{
	
	if (ImGui::Button("Simulate"))
	{
		_simulationWindow = true;
		_simulate = true;
	}

}

void Gui::stopButtonHandler()
{
	ImGui::SameLine();
	ImGui::PushID(1);
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV( 0.0f, 1.f, 1.f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f, 1.0f, 0.9f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f, 1.0f, 0.8f));
	if (ImGui::Button("Stop Simulation")) { _simulate = false; }
	ImGui::PopStyleColor(3);
	ImGui::PopID();
}


bool Gui::VisualVisible() const
{
	return _visualVisible;
}

bool Gui::exitPressed() const
{
	return _exitPressed;
}

void Gui::handleCollapsingHeaders()
{
	handleConfigurationHeader();
	handleSettingsHeader();
}

