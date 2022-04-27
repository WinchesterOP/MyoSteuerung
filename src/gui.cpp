#include <array>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <Windows.h>
#include "gui.h"

// In diesem Modus wird der Mauscursor bewegt und Tastendrücken simuliert
void gui::cursorMode()
{
	std::string gesture;
	std::array<int8_t, 8> emg_data;
		
	bool quit = false; //enters loop
	std::cout << std::endl << "\tGeste:\t" << "EMG-Daten: " << std::endl << std::endl;
	while (!quit)
	{
		
		hub.run(1000 / 50); //gibt die Anzahl der Messwerte pro Sekunde an
		emg_data = algo.getpositiv(myo_data.getEMG());

		std::cout << "\r"; // clear line

		// Gestenerkennung
		n_net.setInput(emg_data);
		gesture = algo.check_gesture(n_net.getOutput());
		std::cout << "\t[" << gesture << std::string(10 - gesture.size(), ' ') << "]";
		
		//EMG-Daten
		std::cout << "\t";
		for (int i = 0; i < 8; i++)
		{
			std::ostringstream oss;
			oss << static_cast<int>(emg_data[i]);
			std::string emgString = oss.str();
			std::cout << '[' << emgString << std::string(4 - emgString.size(), ' ') << ']';
		}
		std::cout << std::flush;

		//Inertial-Daten
		algo.cursor(myo_data.getInertial(), gesture, myo);

		//Beendet Schleife
		if (listenKeyPress(0x0D)) { // 0x0D = Enter
			quit = true;
		}
	}
}

// In diesem Modus werden
void gui::learnMode(const char* outputname, int gesten_id)
{
	int8_t fistdouble = 0;
	bool saveflag = false;
	bool quit = false; //enters loop
	int savecount = 0;
	std::array<int8_t, 8> emg_data;
	n_net.setlearnmode(8, 4, gesten_id);

	std::cout << std::endl << "Anzahl gespeicherter Werte: " << savecount;
	while (!quit)
	{
		hub.run(1000 / 50); //gibt die Anzahl der Messwerte pro Sekunde an
		// Wenn ein Wert hinzugefügt wird wird ein Counter hochgesetzt
		if (n_net.loadPattern(algo.getpositiv(myo_data.getEMG())))
		{
			savecount++;
			std::cout << "\r";
			std::cout << "Anzahl gespeicherter Werte: " << savecount;
		}

		//Beendet Schleife
		if (listenKeyPress(0x0D)) { // 0x0D = Enter
			quit = true;
		}
	}
	n_net.saveLession(outputname);
}

// Liest den Tastendruck zum beenden des Programmes
bool gui::listenKeyPress(short p_key)
{
	const unsigned short MSB = 0x8000;
	if (GetAsyncKeyState(p_key) & MSB)
	{
		Sleep(600);
		return true;
	}
	else
	{
		return false;
	}
}

// Hier wird sich am Anfang für den Modus entschieden
void gui::chooseMode()
{
	// Um ein Hub zu erzeugen muss die Myo Connect(Bluetoothverbindung) gestartet sein. Dies führt ansonsten zu einem Error
	// Try and Catch wurde dem Myo-SDK entnommen
	try {
		std::cout << "Attempting to find a Myo..." << std::endl;
		myo = hub.waitForMyo(10000);
		if (!myo) {
			throw std::runtime_error("Unable to find a Myo!");
		}
		std::cout << "Connected to a Myo armband!" << std::endl << std::endl;
		myo->setStreamEmg(myo::Myo::streamEmgEnabled);
		hub.addListener(&myo_data);
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		std::cerr << "Press enter to continue.";
		std::cin.ignore();
	}

	int mode;
	bool stop = false;
	bool wrong = false;

	while(!stop)
	{
		system("CLS");
		std::cout << std::endl
			<< "\t" << "Waehlen sie den Modus aus den sie benutzen moechten." << std::endl
			<< std::endl
			<< "\t\t1. Cursor-Modus" << std::endl
			<< "\t\t2. Lern-Modus" << std::endl;

		if (wrong)
		{
			std::cout << "\tEingabe ist ungültig";
		}
		std::cin >> mode;

		if (mode == 1 || mode == 2)
		{
			wrong = false;
			stop = false;

			// Für den Cursor-Mode werden keine weiteren Daten benötigt da sie direkt vom DataCollector empfangen werden
			if (mode == 1)
			{
				Sleep(600);
				system("CLS");
				cursorMode();
			} 
			// Wird sich für den Lern-Modus entschieden werden die Informationen für Geste und Outputfile
			// gesammelt damit sie im Modus benutzt werden können
			else if (mode == 2)
			{
				std::string input;
				int gesten_id;
				const char* outputname = "default";
				system("CLS");
				std::cout << "\tGeben sie den Namen der Output-Datei ein: \t";
				std::cin.clear();
				std::cin.ignore(INT_MAX, '\n');
				std::cin >> input;
				input += ".csv";
				outputname = input.c_str();

				while(!stop)
				{
					if (wrong)
					{
						system("CLS");
						std::cout << "Geste nicht erkannt." << std::endl;
					}
					std::cout << std::endl << "\tGeben sie die zu lernende Geste ein: \t" << std::endl
						<< "\t\t1. Fist" << std::endl
						<< "\t\t2. WaveIn" << std::endl
						<< "\t\t3. WaveOut" << std::endl << std::endl;
					std::cin.clear();
					std::cin.ignore(INT_MAX, '\n');
					std::cin >> gesten_id;
					// Falls eine falsche Eingabe getätigt wird
					if (!(gesten_id < 4 && gesten_id > 0))
					{
						wrong = true;
					} 
					else
					{
						wrong = false;
						stop = true;
					}
				}
				// Sleep wird gesetzt damit die Enter-Taste den LearnMode nicht sofot beendet
				Sleep(600); 
				learnMode(outputname, gesten_id);
			}
			stop = true;
		} 
		else
		{
			wrong = true;
		}
	}
}
