#include "algorithmus.h"
#include <windows.h>
#include <myo/cxx/Myo.hpp>
#include "myo.h"

// 
std::string algorithmus::check_gesture(std::string gesture)
{
	int8_t gesture_id = 0;
	int8_t result_gesture_id = 3;
	std::string result_string = "nothing";
	
	// Wenn keine der drei Gesten erkannt wurde wird immer "nothing" ausgegeben
	if (gesture == "fist") 
	{
		gesture_id = 0;
	}
	else if (gesture == "wavein") 
	{
		gesture_id = 1;
	}
	else if (gesture == "waveout")
	{
		gesture_id = 2;
	}
	else
	{
		gesture_id = 3;
	}

	// Erst mehrere aufeinanderfolgende Signale führen zu einer Geste
	// Dies funktionert wie ein Entpreller da davon ausgegangen wird das
	// niemals bloß ein einziges Signal durchkommt beim vollführen
	// einer Geste
	if (last_gesture_id == gesture_id) 
	{
		flag_counter[gesture_id]++;
		for (int i = 0 ; i < 4 ; i++)
		{
			if (i != gesture_id && flag_counter[i] > 0)
			{
					flag_counter[i]--;
			}
		}
	}
	else 
	{
		if (flag_counter[gesture_id] > 0)
		{
			flag_counter[gesture_id]--;
		}
	}
	// für 50 Messwerte pro Sekunde hat sich ein Entpreller-Wert von 3 am besten erwiesen
	if (flag_counter[gesture_id] > ENTPRELLER)
	{
		result_gesture_id = gesture_id;
		flag_counter[gesture_id]--;
	} 
	else
	{
		result_gesture_id = 3; // Nothing

	}
	last_gesture_id = gesture_id;
	// Ausgabe
	switch (result_gesture_id)
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

//Setzt die Y-Achse fest für das Feld in dem man die Hand bewegen muss
void algorithmus::set_start_inertial1(int inertial1)
{
	start_inertial_1 = inertial1;
	start_inertial_1_flag = false; // wird gesetzt wenn der Inertial-Bereich über 1000 oder unter 0 geht

	inertial_1_down = start_inertial_1 - MOUSEFIELDAREA;
	if (inertial_1_down < 0)
	{
		inertial_1_down = MAXINERTIAL + inertial_1_down;
		inertial_1_border_flag = true;
	}
	inertial_1_top = start_inertial_1 + MOUSEFIELDAREA;
	if (inertial_1_top > MAXINERTIAL)
	{
		inertial_1_top -= MAXINERTIAL;
		inertial_1_border_flag = true;
	}
}

//Setzt die X-Achse fest für das Feld in dem man die Hand bewegen muss
void algorithmus::set_start_inertial2(int inertial2)
{
	start_inertial_2 = inertial2;
	start_inertial_2_flag = false; // wird gesetzt wenn der Inertial-Bereich über 1000 oder unter 0 geht

	inertial_2_down = start_inertial_2 - MOUSEFIELDAREA;
	if (inertial_2_down < 0)
	{
		inertial_2_down = MAXINERTIAL + inertial_2_down;
		inertial_2_border_flag = true;
	}
	inertial_2_top = start_inertial_2 + MOUSEFIELDAREA;
	if (inertial_2_top > MAXINERTIAL)
	{
		inertial_2_top -= MAXINERTIAL;
		inertial_2_border_flag = true;
	}
}

// Anhand der Inertial-Daten wird die Y-Position des Mauszeigers bestimmt
int algorithmus::get_ykoord(int inertial1) // Pitch-Koordinate
{
	if (inertial1 < 0)
	{
		// negative Daten führen zum absturz und werden nicht angenommen
		// negative Werte kommen im Normalfall bloß während der Initialiserung des Myos durch
	}
	else
	{
		if (start_inertial_1_flag)
		{
			set_start_inertial1(inertial1);
		}

		int y_koord = inertial1; 
		double cursor_y = 0;

		// Wenn sich der Mauszeiger über den Bildschirmrand bewegen sollte verhart er am Rand
		if (y_koord > inertial_1_top && !inertial_1_border_flag)
		{
			y_koord = inertial_1_top;
		}
		else if (y_koord < inertial_1_down && !inertial_1_border_flag)
		{
			y_koord = inertial_1_down;
		}
		else if (y_koord > inertial_1_top && inertial_1_border_flag && y_koord < inertial_1_down)
		{
			y_koord = inertial_1_top;
		}
		else if (y_koord < inertial_1_down && inertial_1_border_flag && y_koord > inertial_1_top)
		{
			y_koord = inertial_1_down;
		}

		double y_percentage;
		int y_perventage_temp = (y_koord - inertial_1_down);

		// Wenn der Inertial-Bereich über 1000 oder unter 0 geht wird die Grenze hinzuaddiert da sonst mit negativen Werten gearbeitet wird
		if (y_perventage_temp < 0 && inertial_1_border_flag)
		{
			y_percentage = (MAXINERTIAL + (double)y_perventage_temp) / ((MOUSEFIELDAREA * 2) / 100);
		}
		else
		{
			y_percentage = (double)y_perventage_temp / ((MOUSEFIELDAREA * 2) / 100);
		}

		y_percentage = 100 - y_percentage;
		cursor_y = ((double)window_vertical / 100) * y_percentage;

		return cursor_y;
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------
// Hauptbestandteil der Software
// Hier werden Änderungen vorgenommen die zu Steuerung beötigt werden
// Momentan steuert die Funktion den Mauscursor
void algorithmus::cursor(std::array<int, 3> inertial_data, std::string gesture, myo::Myo* myo)
{
	bool same_gesture;
	if (gesture == last_gesture)
	{
		same_gesture = true;
	}
	else
	{
		same_gesture = false;
	}
	last_gesture = gesture;
	// Handfläche zu Decke verursacht dass keine Aktionen mehr ausgeführt werden
	// Das Programm läuft trotzdem weiter
	if (inertial_data[0] > 320) 
	{
		if (gesture != "nothing" && !same_gesture)
		{
			myo->vibrate(myo::Myo::vibrationShort);
		}
		int x_koord = get_xkoord(inertial_data[2]);
		int y_koord = get_ykoord(inertial_data[1]);
		SetCursorPos(x_koord, y_koord);
		if (gesture == "fist" && !same_gesture)
		{
			mouse_event(MOUSEEVENTF_LEFTDOWN, x_koord, y_koord, 0, 0);
			mouse_event(MOUSEEVENTF_LEFTUP, x_koord, y_koord, 0, 0);
			Sleep(300);
		}
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------

// Es werden mit positiven EMG-Daten gearbeitet daher werden alle negativen Werte negiert
std::array<int8_t, 8> algorithmus::getpositiv(std::array<int8_t, 8> emg_data)
{
	for (int i = 0; i < 8; i++)
	{
		if (emg_data[i] < 0)
		{
			emg_data[i] = emg_data[i] * -1;
		}

		if (emg_data[i] < 0)
		{
			emg_data[i] = 127;
		}
	}
	return emg_data;
}

// Anhand der Inertial-Daten wird die Y-Position des Mauszeigers bestimmt
int algorithmus::get_xkoord(int inertial2)
{
	if (inertial2 < 0)
	{
		// negative Daten führen zum absturz und werden nicht angenommen
		// negative Werte kommen im Normalfall bloß während der Initialiserung des Myos durch
	}
	else
	{
		if (start_inertial_2_flag)
		{
			set_start_inertial2(inertial2);
		}

		int x_koord = inertial2;
		double cursor_x = 0;

		// Wenn sich der Mauszeiger über den Bildschirmrand bewegen sollte verhart er am Rand
		if (x_koord > inertial_2_top && !inertial_2_border_flag)
		{
			x_koord = inertial_2_top;
		}
		else if (x_koord < inertial_2_down && !inertial_2_border_flag)
		{
			x_koord = inertial_2_down;
		}
		else if (x_koord > inertial_2_top && inertial_2_border_flag && x_koord < inertial_2_down)
		{
			x_koord = inertial_2_top;
		}
		else if (x_koord < inertial_2_down && inertial_2_border_flag && x_koord > inertial_2_top)
		{
			x_koord = inertial_2_down;
		}

		double x_percentage;
		int x_perventage_temp = (x_koord - inertial_2_down);

		// Wenn der Inertial-Bereich über 1000 oder unter 0 geht wird die Grenze hinzuaddiert da sonst mit negativen Werten gearbeitet wird
		if (x_perventage_temp < 0 && inertial_2_border_flag)
		{
			x_percentage = (MAXINERTIAL + (double)x_perventage_temp) / ((MOUSEFIELDAREA * 2) / 100);
		}
		else
		{
			x_percentage = (double)x_perventage_temp / ((MOUSEFIELDAREA * 2) / 100);
		}

		x_percentage = 100 - x_percentage;
		cursor_x = ((double)window_horizontal / 100) * x_percentage;

		return cursor_x;
	}
}

// Gibt die Desktop Ausflösung an
void algorithmus::GetDesktopResolution(int& horizontal, int& vertical)
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	horizontal = desktop.right;
	vertical = desktop.bottom;
}
