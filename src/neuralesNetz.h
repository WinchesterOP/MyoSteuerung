#define INPUTSENSOREN 8 // Anzahl der Sensoren die benutzt werden
#define SENSORPEAK 90 // Messhöhe ab welcher die Werte gespeichert werden

#include <MemBrain_inc.h>
#include <iostream>
#include <array>

class neuralesNetz
{
public:
	std::array<int8_t, 4> gesture_flag_counter;
	int resultNeuron;
	_MB_INT patterncount = 0;

	// Konstruktor
	neuralesNetz::neuralesNetz(const char* dateiname)
	{
		// Initialisert ein Netz da ansonsten ein Default-Netz genutzt wird
		_MB_AddNet();
		// Selektiert das neue Netz
		_MB_SelectNet(1);
		// "myo_reduce.mbn"

		if (_MB_LoadNet(dateiname) == MEMBRAIN_ERR)
		{
			std::cout << "Fehler beim laden der mdb-Datei" << std::endl;
		}
	}

	// Destruktor
	neuralesNetz::~neuralesNetz()
	{
	}

	std::string getOutput();

	int setInput(std::array<int8_t, 8> emgSamples);
	void setPattern(std::array<int8_t, 8> input, std::array<int8_t, 4> output);
	void setlearnmode(int inputcount, int outputcount, int gesten_id);
	void learnPattern(int lessons);
	void saveLession(const char* file);
	bool loadPattern(std::array<int8_t, 8> emg_data);

private:
	std::array<int8_t, 4> pattern_output;

	void setOutput();
	void setLessonInputCount(_MB_INT count);
	void setLessionOutputCount(_MB_INT count);
	void loadLesson(const char* file);
	
};