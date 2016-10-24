#define NEURONALNET "myo.mbn"

#include "myo.h"
#include "algorithmus.h"
#include "neuralesNetz.h"

class gui
{
private:
	void cursorMode();
	void learnMode(const char* outputname, int gesten_id);

	DataCollector myo_data;
	algorithmus algo;
	neuralesNetz n_net;
	myo::Myo* myo;
	myo::Hub hub; // benötigt Myo Connect Software
	

public:
	// Konstruktor
	gui::gui() : n_net(neuralesNetz(NEURONALNET)) 
	{
	}

	// Destruktor
	gui::~gui()
	{
	}

	bool listenKeyPress(short p_key);
	void chooseMode();	
};