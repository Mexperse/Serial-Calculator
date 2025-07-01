// SerialCalculator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream> // Include the iostream header for input/output operations
#include <string> // Include the string header for string manipulation
#include "SerialPort.hpp" // Ensure you have the SerialPort.hpp file in project directory
#include <fstream> // Include the fstream header for file operations
#include <ctime> // Include the ctime header for time-related functions

const char* port = "\\\\.\\COM3"; // Change this to your serial port

// Generate a timestamp for logging
std::string getTimestamp() {
	time_t now = time(nullptr); // Get the current time
	struct tm timeinfo;  // Create a tm structure to hold the local time
	char buf[20]; // Buffer to hold the formatted string

	// Fill timeinfo with local time information
	if (localtime_s(&timeinfo, &now) != 0) {
		return "[Error getting time]"; // Return an error message if localtime_s fails
	}

	strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeinfo); // Time format
	return std::string(buf); // Returns the formatted time as a string
}

int main()
{
	SerialPort arduino(port); // Create a SerialPort object with the specified port
	if (!arduino.isConnected()) { // Check if the connection to the serial port was successful
		std::cout << "Failed to connect to Arduino on" << port << std::endl; // Output an error message if the connection failed
		return 1; // Exit the program with an error code	
	}
	std::cout << "Connected to Arduino on " << port << std::endl; // Output a success message if the connection was successful

	std::ofstream resultfile("results.txt", std::ios::app); // Opens result file in append mode
	if (!resultfile.is_open()) { // Check if the file is open sucessfully 
		std::cerr << "Could not open results file.\n"; // Output an error message if the file could not be opened
		return 1; // Exits the program with error code
	}

	while (true) { // Start an infinite loop to continuously read from the serial port
		std::string expression; // Declare a string to hold user input
		std::cout << "Enter a calculation (e.g., 5 * 5) or type 'exit' to exit and save the result: "; // Prompt the user for input
		std::getline(std::cin, expression); // Read a line of input from the user
		if (expression == "exit") { // Check if the user wants to exit the program
			break; // Exit the loop if the user types "exit"
		}

		std::string fullMessage = expression + "\n"; // Adds a newline character to the input expression
		arduino.writeSerialPort(fullMessage.c_str(), fullMessage.length()); // Send the input to the Arduino

		Sleep(100); // Wait and Sleeps for 100 milliseconds to allow the Arduino to process the input

		char buffer[256]; // Declare a buffer to hold the response from the Arduino
		arduino.readSerialPort(buffer, sizeof(buffer) - 1); // Read the response from the Arduino

		std::string result(buffer); // Convert the buffer to a string
		std::cout << "Result: " << result << std::endl; // Output the result to the console
		
		//std::cout << "Timestamp:" << getTimestamp() << std::endl; // Output the current timestamp to the console
		resultfile << "[" << getTimestamp() << "] " << result; // Write the expression and result to the results file
	}
	resultfile.close(); // Closes the result file
	std::cout << "Exiting program. Results saved to results.txt.\n"; // Outputs a message signaling the end of program execution and that results are saved
	return 0; // Return 0 to indicate successful completion of the program
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
