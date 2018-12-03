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
	_lastStateFileName = "./SaveData/lastState.txt";
	_2D = false;
}

Gui::~Gui()
{
	delete[] _cohesiveEnergy;
	delete[] _debyeTemperature;
	delete[] _kineticEnergy;
	delete[] _meanSquareDisplacement;
	delete[]_potentialEnergy;
	delete[] _pressure;
	delete[] _selfDiffusionCoeff;
	delete[] _specificHeat;
	delete[] _temp;
	delete[] _totalEnergy;

	for (int t = 0; t < _arrayLength; t++)
	{
		for (int n = 0; n < _numberOfAtoms; n++)
		{
			delete[] _pos[t][n];
		}
		delete[] _pos[t];
	}
	delete[] _pos;
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
	if (_loadResultWindow && !simulate())
	{
		ImGui::Begin("Load results file...", &_loadResultWindow, flags);
		ImGui::SetWindowSize(ImVec2(500, 110));
		ImGui::Text("Enter the name of the file you want to open:");
		static char buf1[64] = "./SaveData/"; 
		ImGui::InputText(".txt", buf1, 64);
		if (ImGui::Button("Load")) 
		{
			strcat(buf1, ".txt");
			std::ifstream myFile;
			myFile.open(buf1); 
			if (!myFile.is_open()) { _unableToOpenFile = true; }
			else 
			{
				std::string dummy;
				int sizeOfFile{ 0 }, i{ 0 };
				while (!myFile.eof()) { getline(myFile, dummy); sizeOfFile++; }
				myFile.close(); myFile.open(buf1);

				 _cohesiveEnergy = new double[sizeOfFile];
				 _debyeTemperature = new double[sizeOfFile];
				 _kineticEnergy = new double[sizeOfFile];
				 _meanSquareDisplacement = new double[sizeOfFile];
				 _potentialEnergy = new double[sizeOfFile];
				 _pressure = new double[sizeOfFile];
				 _selfDiffusionCoeff = new double[sizeOfFile];
				 _specificHeat = new double[sizeOfFile];
				 _temp = new double[sizeOfFile];
				 _totalEnergy = new double[sizeOfFile];

				std::getline(myFile, dummy);
				std::getline(myFile, dummy);

				while (!myFile.eof())
				{
					myFile >>  _cohesiveEnergy[i];
					myFile >>  _debyeTemperature[i];
					myFile >>  _kineticEnergy[i];
					myFile >>  _meanSquareDisplacement[i];
					myFile >>  _potentialEnergy[i];
					myFile >>  _pressure[i];
					myFile >>  _selfDiffusionCoeff[i];
					myFile >>  _specificHeat[i];
					myFile >>  _temp[i];
					myFile >>  _totalEnergy[i];

					i++;
				}
				myFile.close();
				_loadResultWindow = false;
				_unableToOpenFile = false;
				_numberOfTimeStepsPlot = i-1;
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
	if (_saveResultWindow && !simulate())
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

			for (int i = 0; i < _numberOfTimeStepsPlot; i++)
			{

				myFile << _cohesiveEnergy[i] << " "
					<< _debyeTemperature[i] << " "
					<< _kineticEnergy[i] << " "
					<< _meanSquareDisplacement[i] << " "
					<< _potentialEnergy[i] << " "
					<< _pressure[i] << " "
					<< _selfDiffusionCoeff[i] << " "
					<< _specificHeat[i] << " "
					<< _temp[i] << " "
					<< _totalEnergy[i] << std::endl;
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
	int numberOfTimeSteps = _numberOfTimeStepsPlot;
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
			std::vector<float>* cohesiveVector = new std::vector<float>;
			float max = -1E15;
			float min = 1E15;
			float sumLast10 = 0;

			for (int i = 0; i < numberOfTimeSteps - 1; i++)
			{
				cohesiveVector->push_back(static_cast<float>(_cohesiveEnergy[i + 1]) / _elementaryCharge);

				if (cohesiveVector->at(i) > max) { max = cohesiveVector->at(i); }
				else if (cohesiveVector->at(i) < min) { min = cohesiveVector->at(i); }

				if (i >= numberOfTimeSteps - 11 && i < numberOfTimeSteps - 1)
				{
					sumLast10 += cohesiveVector->at(i);
				}
			}

			ImGui::PlotLines("", cohesiveVector, numberOfTimeSteps - 1, 0, "Cohesive energy [eV]", min, max, ImVec2(1700, 480));
			printStatistics(min, max, sumLast10);
			delete cohesiveVector;
		}
		if (ImGui::CollapsingHeader("Debye Temperature"))
		{
			std::vector<float>* debyeTempVector = new std::vector<float>;
			float max = -1E15;
			float min = 1E15;
			float sumLast10 = 0;

			for (int i = 0; i < numberOfTimeSteps - 1; i++)
			{
				debyeTempVector->push_back(static_cast<float>(_debyeTemperature[i + 1]));
				if (debyeTempVector->at(i) > max) { max = debyeTempVector->at(i); }
				else if (debyeTempVector->at(i) < min) { min = debyeTempVector->at(i); }
				if (i >= numberOfTimeSteps - 11 && i < numberOfTimeSteps - 1)
				{
					sumLast10 += debyeTempVector->at(i);
				}
			}

			ImGui::PlotLines("", debyeTempVector, numberOfTimeSteps - 1, 0, "Debye Temperature [K]", min, max, ImVec2(1700, 480));
			printStatistics(min, max, sumLast10);
			delete debyeTempVector;

		}
		if (ImGui::CollapsingHeader("Internal pressure"))
		{
			std::vector<float>* pressureVector = new std::vector<float>;
			float max = -1E15;
			float min = 1E15;
			float sumLast10 = 0;

			for (int i = 0; i < numberOfTimeSteps - 1; i++)
			{
				pressureVector->push_back(static_cast<float>(_pressure[i + 1]));
				if (pressureVector->at(i) > max) { max = pressureVector->at(i); }
				else if (pressureVector->at(i) < min) { min = pressureVector->at(i); }
				if (i >= numberOfTimeSteps - 11 && i < numberOfTimeSteps - 1)
				{
					sumLast10 += pressureVector->at(i);
				}
			}

			ImGui::PlotLines("", pressureVector, numberOfTimeSteps - 1, 0, "Internal pressure [Pa]", min, max, ImVec2(1700, 480));
			printStatistics(min, max, sumLast10);
			delete pressureVector;
		}
		if (ImGui::CollapsingHeader("Kinetic energy"))
		{
			std::vector<float>* kineticVector = new std::vector<float>;
			float max = -1E15;
			float min = 1E15;
			float sumLast10 = 0;

			for (int i = 0; i < numberOfTimeSteps - 1; i++)
			{
				kineticVector->push_back(static_cast<float>(_kineticEnergy[i + 1]) / _elementaryCharge);
				if (kineticVector->at(i) > max) { max = kineticVector->at(i); }
				else if (kineticVector->at(i) < min) { min = kineticVector->at(i); }
				if (i >= numberOfTimeSteps - 11 && i < numberOfTimeSteps - 1)
				{
					sumLast10 += kineticVector->at(i);
				}
			}
			ImGui::PlotLines("", kineticVector, numberOfTimeSteps - 1, 0, "Kinetic energy [eV]", min, max, ImVec2(1700, 480));
			printStatistics(min, max, sumLast10);
			delete kineticVector;
		}
		if (ImGui::CollapsingHeader("Mean square displacement"))
		{
			std::vector<float>* meanSquareVector = new std::vector<float>;
			float max = -1E15;
			float min = 1E15;
			float sumLast10 = 0;

			for (int i = 0; i < numberOfTimeSteps - 1; i++)
			{
				meanSquareVector->push_back(1e20*static_cast<float>(_meanSquareDisplacement[i + 1]));
				if (meanSquareVector->at(i) > max) { max = meanSquareVector->at(i); }
				else if (meanSquareVector->at(i) < min) { min = meanSquareVector->at(i); }
				if (i >= numberOfTimeSteps - 11 && i < numberOfTimeSteps - 1)
				{
					sumLast10 += meanSquareVector->at(i);
				}
			}

			ImGui::PlotLines("", meanSquareVector, numberOfTimeSteps - 1, 0, "Mean square displacement [Angstrom^2]", min, max, ImVec2(1700, 480));
			printStatistics(min, max, sumLast10);
			delete meanSquareVector;
		}
		if (ImGui::CollapsingHeader("Potential energy"))
		{
			std::vector<float>* potentialVector = new std::vector<float>;
			float max = -1E15;
			float min = 1E15;
			float sumLast10 = 0;

			for (int i = 0; i < numberOfTimeSteps - 1; i++)
			{
				potentialVector->push_back(static_cast<float>(_potentialEnergy[i + 1]) / _elementaryCharge);

				if(potentialVector->at(i) > max) { max = potentialVector->at(i); }
				else if (potentialVector->at(i) < min) { min = potentialVector->at(i); }

				if (i >= numberOfTimeSteps - 11 && i < numberOfTimeSteps - 1)
				{
					sumLast10 += potentialVector->at(i);
				}
			}

			ImGui::PlotLines("", potentialVector, numberOfTimeSteps - 1, 0, "Potential energy [eV]", min, max, ImVec2(1700, 480));
			printStatistics(min, max, sumLast10);
			delete potentialVector;
		}
		if (ImGui::CollapsingHeader("Self diffusion coefficient"))
		{
			std::vector<float>* selfDiffVector = new std::vector<float>;
			float max = -1E15;
			float min = 1E15;
			float sumLast10 = 0;

			for (int i = 0; i < numberOfTimeSteps - 1; i++)
			{
				selfDiffVector->push_back(static_cast<float>(_selfDiffusionCoeff[i + 1]));
				if (selfDiffVector->at(i) > max) { max = selfDiffVector->at(i); }
				else if (selfDiffVector->at(i) < min) { min = selfDiffVector->at(i); }
				if (i >= numberOfTimeSteps - 11 && i < numberOfTimeSteps - 1)
				{
					sumLast10 += selfDiffVector->at(i);
				}
			}

			ImGui::PlotLines("", selfDiffVector, numberOfTimeSteps - 1, 0, "Self diffusion coefficient [m^2 s^-1]", min, max, ImVec2(1700, 480));
			printStatistics(min, max, sumLast10);
			delete selfDiffVector;

		}
		if (ImGui::CollapsingHeader("Specific heat"))
		{
			std::vector<float>* specificHeatVector = new std::vector<float>;
			float max = -1E15;
			float min = 1E15;
			float sumLast10 = 0;

			for (int i = 0; i < numberOfTimeSteps - 1; i++)
			{
				specificHeatVector->push_back(static_cast<float>(_specificHeat[i + 1]));
				if (specificHeatVector->at(i) > max) { max = specificHeatVector->at(i); }
				else if (specificHeatVector->at(i) < min) { min = specificHeatVector->at(i); }
				if (i >= numberOfTimeSteps - 11 && i < numberOfTimeSteps - 1)
				{
					sumLast10 += specificHeatVector->at(i);
				}
			}

			ImGui::PlotLines("", specificHeatVector, numberOfTimeSteps - 1, 0, "Specific heat [J K^-1]", min, max, ImVec2(1700, 480));
			printStatistics(min, max, sumLast10);
			delete specificHeatVector;
		}

		if (ImGui::CollapsingHeader("Temperature"))
		{
			std::vector<float>* temperatureVector = new std::vector<float>;
			float max = -1E15;
			float min = 1E15;
			float sumLast10 = 0;

			for (int i = 0; i < numberOfTimeSteps - 1; i++)
			{
				temperatureVector->push_back(static_cast<float>(_temp[i + 1]));
				if (temperatureVector->at(i) > max) { max = temperatureVector->at(i); }
				else if (temperatureVector->at(i) < min) { min = temperatureVector->at(i); }
				if (i >= numberOfTimeSteps - 11 && i < numberOfTimeSteps - 1)
				{
					sumLast10 += temperatureVector->at(i);
				}
			}
			ImGui::PlotLines("", temperatureVector, numberOfTimeSteps - 1, 0, "Temperature [K]", min, max, ImVec2(1700, 480));
			printStatistics(min, max, sumLast10);
			delete temperatureVector;
		}
		if (ImGui::CollapsingHeader("Total energy"))
		{
			std::vector<float>* totalEnergyVector = new std::vector<float>;
			float max = -1E5;
			float min = 1E5;
			float sumLast10 = 0;

			for (int i = 0; i < numberOfTimeSteps - 1; i++)
			{
				totalEnergyVector->push_back(static_cast<float>(_totalEnergy[i + 1]) /  _elementaryCharge);
				if (totalEnergyVector->at(i) > max) { max = totalEnergyVector->at(i); }
				else if (totalEnergyVector->at(i) < min) { min = totalEnergyVector->at(i); }
				if (i >= numberOfTimeSteps - 11 && i < numberOfTimeSteps - 1)
				{
					sumLast10 += totalEnergyVector->at(i);
				}
			}
			//printf("Min: %f Max: %f", min, max);

			ImGui::PlotLines("", totalEnergyVector, numberOfTimeSteps - 1, 0, "Total energy [eV]", min, max, ImVec2(1700, 480));
			printStatistics(min, max, sumLast10);
			delete totalEnergyVector;
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
		showHelpMarker("You can input value using the scientific notation,\n  e.g. \"1e+8\" becomes \"100000000\".\n This should be greater than the lattice constant. \n");

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

void Gui::setNumberOfTimeStepsPlot(int number)
{
	_numberOfTimeStepsPlot = number;
}

void Gui::updateArrays(int newArrayLength, int N)
{
	if (_arrayLength > 0 && newArrayLength == 0 && N == 0)
	{
		delete[] _cohesiveEnergy;
		_cohesiveEnergy = nullptr;
		delete[] _debyeTemperature;
		_debyeTemperature = nullptr;
		delete[] _kineticEnergy;
		_kineticEnergy = nullptr;
		delete[] _meanSquareDisplacement;
		_meanSquareDisplacement = nullptr;
		delete[] _potentialEnergy;
		_potentialEnergy = nullptr;
		delete[] _pressure;
		_pressure = nullptr;
		delete[] _selfDiffusionCoeff;
		_selfDiffusionCoeff = nullptr;
		delete[] _specificHeat;
		_specificHeat = nullptr;
		delete[] _temp;
		_temp = nullptr;
		delete[] _totalEnergy;
		_totalEnergy = nullptr;
			   
		for (int t = 0; t < _arrayLength; t++)
		{
			for (int n = 0; n < _numberOfAtoms; n++)
			{
				delete[] _pos[t][n];
				_pos[t][n] = nullptr;
			}
			delete[] _pos[t];
			_pos[t] = nullptr;
		}
		delete[] _pos;
		_pos = nullptr;
	}

	if (newArrayLength > 0 && N > 0)
	{
		_totalEnergy = new double[newArrayLength];
		_potentialEnergy = new double[newArrayLength];
		_temp = new double[newArrayLength];
		_kineticEnergy = new double[newArrayLength];
		_cohesiveEnergy = new double[newArrayLength];
		_debyeTemperature = new double[newArrayLength];
		_selfDiffusionCoeff = new double[newArrayLength];
		_pressure = new double[newArrayLength];
		_meanSquareDisplacement = new double[newArrayLength];
		_specificHeat = new double[newArrayLength];

		_pos = new double**[newArrayLength];
		for (int t = 0; t < newArrayLength; t++)
		{
			_pos[t] = new double*[N];
			for (int n = 0; n < N; n++)
			{
				_pos[t][n] = new double[3];
			}
		}
	}

	_arrayLength = newArrayLength;
	_numberOfAtoms = N;
}