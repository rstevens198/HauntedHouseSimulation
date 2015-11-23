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
class Group
{
public:
	int groupNumber;
	// Variable for the number on people in each group
	int groupPeopleNumber;
	// Variable For arrival to line. either fastpass or regular
	float entranceArrivalTime;
	// Variable For time they got to the inside line
	float insideLineTime;
	// variable used to store time entering the HH
	float M6FUEnterTime;
	// Variable For time spent in the haunted house
	float M6FUTimeLength;
	// Variable for the time group exits the huanted house
	float M6FUExitTime;
	// Total Time In System
	float totalTimeInSystem;
	// Variable to store what type of ticket thy have
	int ticketType;
	// variable to store how long it takes to get inside. 
	float lineWaitTime;
	// variable to store how long it takes to go from inside line to HH
	float insideHHTime;
	// variable to store total time spent in lines
	float totalLineTime;
	

	// Default Constructor
	Group()
	{
		int groupNumber = 0;
		// Variable for the number on people in each group
		int groupPeopleNumber = 0;
		// Variable For Entrance Arrival Time 
		float entranceArrivalTime = 0.0;
		// Variable For when they get to the inside line
		float insideLineTime = 0.0;
		// variable used to store time entering the HH
		float M6FUEnterTime = 0.0;
		// Variable For time spent in the haunted house
		float M6FUTimeLength = 0.0;
		// Variable for the time group exits the huanted house
		float M6FUExitTime = 0.0;
		// Total Time In System
		float totalTimeInSystem = 0.0;
		// Variable to store what type of ticket thy have
		int ticketType = 0;
		// variable to store how long it takes to get inside. 
		float lineWaitTime = 0.0;
		// variable to store how long it takes to go from inside line to HH
		float insideHHTime = 0.0;
		// variable to store total time spent in lines
		float totalLineTime = 0.0;
	}
};


// Simulation Info Structure
class Simulation_Information
{
public:
	// Variables For Simulation
	const static int NUMOFEVENTS = 7;
	const static int outsideLineTicketTime = 120;
	const static int fastpassLineTicketTime = 119;
	int numberOfGroups, amountInsideBuilding, 
		grouponTicketsPercentage, doorTicketPercentage, fastpassTcketPercentage, otherTicketPercentage,
		outsideServerStatus, fastpassServerStatus, insideServerStatus;
	
	float insideLineRate, M6FUEnterRate, M6FUIntervalHigh, M6FUIntervalLow,
		simulationTime, timeOfLastEvent, timeOfNextEvent;

	// Statistical Counter Variables
	int nextEventType, numberOfGroupsExit, peopleInM6FU,
		totalGroupOnTickets, totalDoorTickets, totalOtherTickets;
	
	float totalOutsideQueueDelayTime, totalFastpassQueueDelayTime, totalinsideQueueDelayTime,
		  timeSinceLastEvent,
		  areaUnderOutsideQueue, areaUnderFastpassQueue, areaUnderInsideQueue,
		  groupOnTicketAmount, doorTicketAmount, fastpassTicketAmount;
	
		 


	// Entrace And Exit Queue and M6FUqueue
	std::queue <Group> outsideQueue;
	std::queue <Group> fastpassQueue;
	std::queue <Group> insideQueue;
	std::queue <Group> M6FUQueue;
	// group List
	std::vector <Group> arrayOfGroups;
	

	// Time of Next Array Event That Will Tell Simulation When The Next Simulated Event Is.
	// Will Be A Constant Size 6 For Size of All Event Handling Functions for Simulation.  
	float nextEventTypeArray[NUMOFEVENTS];

	// Overloaded Constructor. Overloaded Initialization Method
	Simulation_Information(int argc, char * argv[]);
	// Timing Method. Used To Determined Next Event
	void timing(void);
	// Event Function. Chose next event type
	void chooseNextEvent(void);
	// Arrival Function For outsideLine
	void outsideLineArrive(void);
	// Exit Function For outsideLine
	void outsideLineExit(void);
	// Arrival Function for FastPassLine
	void fastPassLineArrive(void);
	// Exit Function for FastPassLine
	void fastPassLineExit(void);
	// sets up the inside line queue
	void insideLine(Group);
	// put people in the haunted house
	void M6FU(void);
	// Arrival Function For Exit Gate
	void exitFunction(void);
	// Update Average Time Stats
	void updateAverageTimeStats(void);
	// Report Statisical Data
	void report(void);
};


// Functions dealing with Random numbers 
float massDensityFunction();
float M6FUTime(float M6FUIntervalLow, float M6FUIntervalHigh);
int PeopleInGroup();
int TicketTypeSet(int group, int door, int fastpass, int other);

#endif