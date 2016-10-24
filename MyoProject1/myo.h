#define _USE_MATH_DEFINES

#include <array>
#include <math.h>
#include <myo\myo.hpp>

// Die DataCollector-Klasse wurde von dem Myo-SDK übernommen
// Änderungen:
//		- print()-Funktion heißt nun getEMG() und getInertial()
//      - aus zwei ähnlichen DataCollector-Klassen zusammengefügt
//      - OnOrientaion() befasst sich nicht mehr auf einen Bereich von 0 - 18 sondern 0 - 1000 führ genauere Mausführung

// Classes that inherit from myo::DeviceListener can be used to receive events from Myo devices. DeviceListener
// provides several virtual functions for handling different kinds of events. If you do not override an event, the
// default behavior is to do nothing.
class DataCollector : public myo::DeviceListener {
public:
	DataCollector() : emgSamples(), onArm(false), isUnlocked(false), roll_w(0), pitch_w(0), yaw_w(0){}
	std::array<int8_t, 8> emgSamples;
	bool onArm;
	bool isUnlocked;
	int roll_w, pitch_w, yaw_w;

	// onUnpair() is called when Myo been took off
	void onUnpair(myo::Myo* myo, uint64_t timestamp) {
		// Resets variables if unpaired
		roll_w = 0;
		pitch_w = 0;
		yaw_w = 0;
		onArm = false;
		isUnlocked = false;
		emgSamples.fill(0);
	}
	void onEmgData(myo::Myo* myo, uint64_t timestamp, const int8_t* emg) {
		for (int i = 0; i < 8; i++) {
			emgSamples[i] = emg[i];
		}
	}
	// onOrientationData() is called whenever the Myo device provides its current orientation, which is represented
	// as a unit quaternion.
	void onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat) {
		using std::atan2;
		using std::asin;
		using std::sqrt;
		using std::max;
		using std::min;
		// Calculate Euler angles (roll, pitch, and yaw) from the unit quaternion.
		float roll = atan2(2.0f * (quat.w() * quat.x() + quat.y() * quat.z()),
			1.0f - 2.0f * (quat.x() * quat.x() + quat.y() * quat.y()));
		float pitch = asin(max(-1.0f, min(1.0f, 2.0f * (quat.w() * quat.y() - quat.z() * quat.x()))));
		float yaw = atan2(2.0f * (quat.w() * quat.z() + quat.x() * quat.y()),
			1.0f - 2.0f * (quat.y() * quat.y() + quat.z() * quat.z()));
		// Scale from 0 to 1000
		roll_w = static_cast<int>((roll + (float)M_PI) / (M_PI * 2.0f) * 1000);
		pitch_w = static_cast<int>((pitch + (float)M_PI / 2.0f) / M_PI * 1000);
		yaw_w = static_cast<int>((yaw + (float)M_PI) / (M_PI * 2.0f) * 1000);
	}
	// onUnlock() is called whenever Myo has become unlocked, and will start delivering pose events.
	void onUnlock(myo::Myo* myo, uint64_t timestamp) {
		isUnlocked = true;
	}
	// onLock() is called whenever Myo has become locked. No pose events will be sent until the Myo is unlocked again.
	void onLock(myo::Myo* myo, uint64_t timestamp) {
		isUnlocked = false;
	}
	// Gibt einem die EMG-Daten zurück
	std::array<int8_t, 8> getEMG() {
		return emgSamples;
	}
	// Gibt einem die Inertial-Daten zurück
	std::array<int, 3> getInertial() {
		std::array<int, 3> inertial_data = {roll_w,pitch_w,yaw_w};
		return inertial_data;
	}
	
};