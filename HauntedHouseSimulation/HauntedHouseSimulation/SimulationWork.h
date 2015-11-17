/*
Simulation Header File
Ryan Stevens
*/

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <fstream>
#include <vector>
#include <queue>
#include <iomanip>

#ifndef SimulationWork
#define SimulationWork

// Car Structure For All Automobile Information
class group
{
public:
	int groupNumber;
	// Variable For Entrance Arrival Time (Enter Queue)
	float entranceArrivalTime;
	// Variable For Entrance Depart Time (Exit Queue To Look For Parking)
	float entranceDepartTime;	
	// Variable For time spent in the haunted house
	float M6FUTimeLength;
	// Variable for the time group exits the huanted house
	float M6FUExitTime;
	// Total Time In System
	float totalTimeInSystem;
	// Variable to store what type of ticket thy have
	int ticketType;
	

	// Default Constructor
	group()
	{
		int groupNumber = 0;
		// Variable For Entrance Arrival Time (Enter Queue)
		float entranceArrivalTime = 0.0;
		// Variable For Entrance Depart Time (Exit Queue To Look For Parking)
		float entranceDepartTime = 0.0;
		// Variable For time spent in the haunted house
		float M6FUTimeLength = 0.0;
		// Variable for the time group exits the huanted house
		float M6FUExitTime = 0.0;
		// Total Time In System
		float totalTimeInSystem = 0.0;
		// Variable to store what type of ticket thy have
		int ticketType = 0;
};


// Simulation Info Structure
class Simulation_Information
{
public:
	// Variables For Simulation
	const static int NUMOFEVENTS = 5;
	int numberOfGroups;
	float arrivalRate, M6FUIntervalHigh, M6FUIntervalLow, exitGate, simulationTime, timeOfLastEvent;

	// Statistical Counter Variables
	int nextEventType, numberOfCustomersDelayed, totalNumberOfGroups,
		numberInEntranceQueue, numberInExitQueue, entranceServerStatus, leavingIndex,
		maxEntranceQueueSize, peopleInM6FU, amountOfGroupsLeft;
	float totalEntranceQueueDelayTime, nextLeavingGroup, timeSinceLastEvent,
		areaUnderEntranceQueue, areaEntranceServerStatus, exitTime;

	// Parking Lot Array. Initializing as a vector and will resize dynamically based on user input
	std::vector <float> parkingLotSpots;
	// Entrace And Exit Queue
	std::queue <group> entranceQueue;
	std::queue <group> exitQueue;
	// Car List
	std::vector <group> arrayOfCars;

	// Time of Next Array Event That Will Tell Simulation When The Next Simulated Event Is.
	// Will Be A Constant Size 6 For Size of All Event Handling Functions for Simulation.  
	float timeOfNextEvent[NUMOFEVENTS];

	// Overloaded Constructor. Overloaded Initialization Method
	Simulation_Information(int argc, char * argv[]);
	// Timing Method. Used To Determined Next Event
	void timing(void);
	// Event Function. Chose next event type
	void chooseNextEvent(void);
	// Arrival Function For Entrance Gate
	void entranceArrive(void);
	// Depart Function For Entrance Gate
	void entranceDepart(void);
	// Arrival Function For Exit Gate
	void exit(void);
	// Update Average Time Stats
	void updateAverageTimeStats(void);
	// Report Statisical Data
	void report(void);
};


// Mass Density Function 
float massDensityFunction();
float M6FUTime(float M6FUIntervalLow, float M6FUIntervalHigh);


#endif