#include <string>

std::string getEMG()
{
}


std::string getInertial() 
{
	// Clear the current line
	std::cout << '\r';
	// Bars for the gyrosensor
	std::cout << '[' << std::string(roll_w, '*') << std::string(18 - roll_w, ' ') << ']'
		<< '[' << std::string(pitch_w, '*') << std::string(18 - pitch_w, ' ') << ']'
		<< '[' << std::string(yaw_w, '*') << std::string(18 - yaw_w, ' ') << ']' << std::endl;

	// Tells us which arm is used
	std::string arm_position = "left";
	if (arm == myo::armLeft) {
		arm_position = "right";
	}
	std::cout << "[ The position is on the " << arm_position << "arm]"
		<< '[' << posisi << std::string(14 - posisi.size(), ' ') << ']' << std::endl;
	// Test
	std::cout << "\n\n hier steht der eigentliche Text" << std::endl;
}