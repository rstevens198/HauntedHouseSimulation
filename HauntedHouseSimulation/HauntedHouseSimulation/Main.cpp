/*
Ryan Stevens
810318529
Class: CS 558
Project: Haunted House Simulation for extra credit
Description: This program is to run simulations and determine the best way to make money. should it be bigger groups? 
	should it be pushing people through quicker? or should be be not using sites like groupon to sell tickets? 
Ryan Stevens
*/

#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <Windows.h>
#include "SimulationWork.h"

// Main Function
int main(int argc, char * argv[])
{
	// Random See With Time. Need For Random Variates
	srand(static_cast<unsigned> (time(NULL)));

	// Create A Simulation Instance
	Simulation_Information firstSimulation = Simulation_Information(argc, argv);

	// Simulation Loop. Run Simulation Until Exit Parameter is met.
	while (firstSimulation.numberOfGroupsExit <= firstSimulation.numberOfGroups)
	{
		// Timing Function To Determine The Next Event
		firstSimulation.timing();

		// Update time-average statistical accumulators. 
		firstSimulation.updateAverageTimeStats();


		// Switch To The Next Event Type Based on Event Type
		firstSimulation.chooseNextEvent();
	}

	// Call Report Function
	firstSimulation.report();

	// Pause And Wait For Keyboard At The End Of The Program
	system("pause");

	return 0;
}