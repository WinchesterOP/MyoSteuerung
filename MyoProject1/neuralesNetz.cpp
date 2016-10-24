#include "neuralesNetz.h"
#include <array>
#include <MemBrain_inc.h>

// Die EMG-Daten von den acht Sensoren werden in die acht Input-Neuronen gesetzt
int neuralesNetz::setInput(std::array<int8_t, 8> emgSamples)
{
	int inputdata = 0;
	for (int i = 0; i < emgSamples.size(); i++)
	{
		inputdata = emgSamples[i];
		// Nur positiver Input
		if (inputdata < 0)
		{
			inputdata = inputdata * -1;
		}
		if (_MB_ApplyInputAct(i, inputdata) == MEMBRAIN_ERR)
		{
			std::cout << "Fehler beim Input: " << i << std::endl;
		}
	}	
	setOutput();
	return 0;
}

// Die Funktion wird von setInput() aufgerufen und berechnet den Output mit dem gesetztem Input
void neuralesNetz::setOutput()
{
	// MemBrain-Funktion die die Berechnung von Input zum Output ausführt
	_MB_ThinkStep(); 
		//	0 = Fist
		//  1 = WaveIn
		//	2 = WaveOut
		//	3 = Nothing
	resultNeuron = _MB_GetOutputWinnerNeuron();
}

// Die eingegebenen Einstellungen werden auf das neuronale Netz angewendet
void neuralesNetz::setlearnmode(int inputcount, int outputcount, int gesten_id)
{
	setLessonInputCount(inputcount);
	setLessionOutputCount(outputcount);
	std::array<int8_t, 4> pattern_output;
	switch (gesten_id)
	{
		case 1: pattern_output = { 1,0,0,0 }; break; // Fist
		case 2: pattern_output = { 0,1,0,0 }; break; // WaveIn
		case 3: pattern_output = { 0,0,1,0 }; break; // WaveOut
	}
}

// Die gelesenen EMG-Daten werden überprüft ob es sich um Messspitzen handelt
bool neuralesNetz::loadPattern(std::array<int8_t, 8> emg_data)
{
	int fistdouble = 0; 
	bool saveflag = false;
	for (int i = 0; i < 8; i++)
	{
		if (emg_data[i] > SENSORPEAK)
		{
			// Fist-Gesten benutzten mehre Muskelpartien gleichzeitig deswegen sollten mindestens 2 Werte gefunden werden fuer eine genauere Messung
			if (pattern_output[0] != 1 || fistdouble > 1)
			{
				saveflag = true;
			}
			else
			{
				fistdouble++;
			}
		}
	}
	if(saveflag)
	{
		setPattern(emg_data, pattern_output);
		return true;
	}
	return false;
}

// Die berechnetete Geste wird ausgegeben
std::string neuralesNetz::getOutput()
{
	switch (resultNeuron)
	{
	case 0:
		return "fist";
	case 1:
		return "wavein";
	case 2:
		return "waveout";
	case 3:
		return "nothing";
	}
}

// Die Anzahl an Input-Neuronen werden gesetzt für die CSV-Datei
void neuralesNetz::setLessonInputCount(_MB_INT count)
{
	_MB_SetLessonInputCount(count);
	
}

// Die Anzahl an Output-Neuronen werden gesetzt für die CSV-Datei
void neuralesNetz::setLessionOutputCount(_MB_INT count)
{
	_MB_SetLessonOutputCount(count);
}

// Sobald die EMG-Daten von loadPattern() überprüft wurden werden sie der Lesson(CSV-Datei) hinzugefügt
void neuralesNetz::setPattern(std::array<int8_t,8> input, std::array<int8_t, 4> output)
{
	// Falls Fehler auftreten werden sie von den MemBrain-Funktionen als 1 ausgegeben
	int8_t err_flag1 = 0;
	int8_t err_flag2 = 0;
	int8_t err_flag3 = 0;

	_MB_AddPattern(); // MemBrain-Funktion: Fügt eine neue Zeile hinzu bevor sie befüllt werden kann
	err_flag3 = _MB_SelectPattern(patterncount); // die neue Zeile muss ausgewählt werden sonst wird immer die erste Zeile neubefüllt
	patterncount++;

	for (int i = 0 ; i < 8 ; i++)
	{
		err_flag1 = _MB_SetPatternInput(i, input[i]);
	}

	for (int i = 0; i < 4; i++)
	{
		err_flag2 = _MB_SetPatternOutput(i, output[i]);
	}
	
	if (err_flag1 < 0)
	{
		std::cout << "SetPatternInput gibt Fehler aus." << std::endl;
	}
	if (err_flag2 < 0)
	{
		std::cout << "SetPatternOutput gibt Fehler aus." << std::endl;
	}
	if (err_flag3 < 0)
	{
		std::cout << "SelectPattern gibt Fehler aus." << std::endl;
	}
}

// Das neuronale Netz anhand der Lesson-Daten trainieren
void neuralesNetz::learnPattern(int lessons)
{
	// Falls Fehler auftreten werden sie von den MemBrain-Funktionen als 1 ausgegeben
	int8_t err_flag1 = 0;
	int8_t err_flag2 = 0;

	_MB_RandomizeNet(); 
	err_flag1 = _MB_SelectTeacher("Teachers.mbt"); // Standard Lehrer von MemBrain
	for (int i = 0 ; i < lessons ; i++)
	{
		err_flag2 = _MB_TeachStep();
	}
	if (err_flag1 < 0)
	{
		std::cout << "SelectTeacher gibt Fehler aus." << std::endl;
	}
	if (err_flag2 < 0)
	{
		std::cout << "TeachStep gibt Fehler aus." << std::endl;
	}
}

// Importiert eine CSV-Datei mit den Lesson-Daten
void neuralesNetz::loadLesson(const char* file)
{
	if (_MB_ImportLessonRaw(file) < 0)
	{
		std::cout << "loadLesson gibt Fehler aus." << std::endl;
	}
}

// Speichert eine CSV-Datei mit den Lesson-Daten
void neuralesNetz::saveLession(const char* file)
{
	if (_MB_ExportLessonRaw(file,0) < 0)// Variable 0 damit alle Zeilen gespeichert werden
	{
		std::cout << "saveLesson gibt Fehler aus." << std::endl;
	}
	
}