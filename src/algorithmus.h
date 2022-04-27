// Gestenerkennungs Einstellungen
#define ENTPRELLER 3

// Mouseclicker Einstellungen
#define MOUSEFIELDAREA 200 // Bereich in dem man den Arm bewegen muss fuer den Mauszeiger
#define MAXINERTIAL 1000 // höchster Wert den die Inertial-Sensoren messen

#include <array>
#include <myo/myo.hpp>

class algorithmus
{
private:
	std::array<int8_t, 4> flag_counter;
	std::string last_gesture;
	int8_t last_gesture_id;

	bool start_inertial_1_flag = true;
	bool start_inertial_2_flag = true;

	int start_inertial_1 = 0;
	int inertial_1_top = 0;
	int inertial_1_down = 0;
	bool inertial_1_border_flag = false;
	
	int start_inertial_2 = 0;
	int inertial_2_top = 0;
	int inertial_2_down = 0;
	bool inertial_2_border_flag = false;

	int window_horizontal;
	int window_vertical;

	void set_start_inertial1(int inertial1);
	void set_start_inertial2(int inertial2);
	int get_xkoord(int inertial2);
	int get_ykoord(int inertial1);
	void GetDesktopResolution(int& horizontal, int& vertical);
	
public:
	// Konstruktor
	algorithmus::algorithmus()
	{
		flag_counter = {0,0,0,0};
		last_gesture_id = 3;
		last_gesture = "nothing";
		GetDesktopResolution(window_horizontal, window_vertical);
	}

	// Destruktor
	algorithmus::~algorithmus()
	{
	}

	void cursor(std::array<int,3> inertial_data, std::string gesture, myo::Myo* myo);
	std::string check_gesture(std::string gesture);
	std::array <int8_t, 8> getpositiv(std::array <int8_t,8> emg_data);
};
