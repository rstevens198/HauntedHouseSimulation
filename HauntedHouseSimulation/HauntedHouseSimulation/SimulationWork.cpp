//Ryan stevens
// This is where all the work for the simulation is done. 

#include "SimulationWork.h"

// Mnemonic For Server Busy
#define BUSY 1  
// Mnemonic For Service IDLE
#define IDLE 0  
// Empty Spot
#define EMPTY 1.0e+30

// Global group counter to see the next group arriving
int arrivingGroupCounter = 0;
bool fastpassLastTicketType = false;

// Constructor that Initializes all Values
Simulation_Information::Simulation_Information(int argc, char * argv[])
{
	// Check Argument Count 
	if (argc > 1)
	{
		// Create A File Instance
		std::fstream simulationInput;

		// Open File
		simulationInput.open(argv[1]);

		// Store Variables From Text File
		while (simulationInput.is_open() && simulationInput.good())
		{
			// Number of cars
			simulationInput >> numberOfGroups;
			// rate at which people are let in the HH
			simulationInput >> M6FUEnterRate;
			// Interarrival High Rate
			simulationInput >> M6FUIntervalHigh;
			// Interarrival Low Rate
			simulationInput >> M6FUIntervalLow;
			// percentage for groupon tickets
			simulationInput >> grouponTicketsPercentage;
			// Percentage of door ticket sales
			simulationInput >> doorTicketPercentage;
			// Percentage of fastpass ticket sales
			simulationInput >> fastpassTcketPercentage;
			// Percentage of other ticket sales
			simulationInput >> otherTicketPercentage;
			// ticket value for groupon
			simulationInput >> groupOnTicketAmount;
			// ticket value for walk up
			simulationInput >> doorTicketAmount;
			// Amount the fastpass sells for
			simulationInput >> fastpassTicketAmount;
			// Amount of tickets sold from othger websites
			simulationInput >> otherTicketPercentage;
			// Amount of people allowed in the building, This is for fire code
			simulationInput >> amountInsideBuilding;
		}
		// Close File 
		simulationInput.close();
	}
	else
	{
		// Default Initialization Variables.
		numberOfGroups = 5; // 377
		M6FUEnterRate = 420;
		M6FUIntervalHigh = 900;
		M6FUIntervalLow = 600;
		grouponTicketsPercentage = 55;
		doorTicketPercentage = 33;
		fastpassTcketPercentage = 4;
		otherTicketPercentage = 8;
		groupOnTicketAmount = 2.00;
		doorTicketAmount = 5.00;
		fastpassTicketAmount = 13.50;
		amountInsideBuilding = 4;


	}

	// Initialize the variables. These follow the same order that is in the .h file
	outsideServerStatus = IDLE;
	fastpassServerStatus = IDLE;
	insideServerStatus = IDLE;
	simulationTime = 0.0;
	timeOfLastEvent = 0.0;
	timeOfNextEvent = 0.0;
	nextEventType = 0;
	numberOfGroupsExit = 0;
	peopleInM6FU = 0;
	totalOutsideQueueDelayTime = 0.0;
	totalFastpassQueueDelayTime = 0.0;
	totalinsideQueueDelayTime = 0.0;
	timeSinceLastEvent = 0.0;
	areaUnderOutsideQueue = 0.0;
	areaUnderFastpassQueue = 0.0;
	areaUnderInsideQueue = 0.0;


	arrayOfGroups.resize(numberOfGroups);

	// Initialization the groups
	for (int i = 0; i < numberOfGroups; i++)
	{
		// Create The Amount Of Car Objects Requested By The User
		arrayOfGroups[i] = Group();

		// Assign A Car Number To Each Car
		arrayOfGroups[i].groupNumber = i;

		// determine ticket type;
		arrayOfGroups[i].ticketType = TicketTypeSet(grouponTicketsPercentage, doorTicketPercentage, fastpassTcketPercentage, otherTicketPercentage);

		// set the number of people in each group
		arrayOfGroups[i].groupPeopleNumber = PeopleInGroup();

		//set the time they spend in the HH
		arrayOfGroups[i].M6FUTimeLength = M6FUTime(M6FUIntervalLow, M6FUIntervalHigh);
	}
	
	// Initialize event list. Since no customers are present, the departure (service completion) event is eliminated from consideration
	// First arrival
	nextEventTypeArray[1] = simulationTime + massDensityFunction(simulationTime);


	arrayOfGroups[0].entranceArrivalTime = nextEventTypeArray[1];
	arrayOfGroups[0].ticketType = 1;
	arrivingGroupCounter++;
	
	// set emtpys
	nextEventTypeArray[2] = EMPTY;
	nextEventTypeArray[3] = EMPTY;
	nextEventTypeArray[4] = EMPTY;
	nextEventTypeArray[5] = EMPTY;
	nextEventTypeArray[6] = EMPTY;

}

void Simulation_Information::timing(void)
{
	// MinTimeUntilNextEvent
	float minTimeNextEvent = 1.0e+29;

	// Set to 0 until an event is chosen
	nextEventType = 0;

	// Determine the event type of the next event to occur.
	// Iterate through all event types
	for (int i = 1; i < NUMOFEVENTS; ++i)
	{
		// Find smallest time
		if (nextEventTypeArray[i] < minTimeNextEvent)
		{
			// Set Min Time To Next Event
			minTimeNextEvent = nextEventTypeArray[i];
			// Set Next Event Type
			nextEventType = i;
			// Set Time Of Last Event To Simulation 
			timeOfLastEvent = simulationTime;

		}
	}

	// Check to see whether the event list is empty.
	// If all times of next events are EMPTY no event is scheduled
	if (nextEventType == 0)
	{
		// The event list is empty, so stop the simulation.
		std::cout << "The event list is empty at time: " << simulationTime << std::endl;
		exit(1);
	}

	// The event list is not empty, so advance the simulation clock.
	simulationTime = minTimeNextEvent;
}

void Simulation_Information::chooseNextEvent(void)
{
	switch (nextEventType)
	{
	case 1:
		// Arrival at outside line
		outsideLineArrive();
		break;
	case 2:
		// Exit at outside line
		outsideLineExit();
		break;
	case 3:
		// Arrival at fastpass line
		fastPassLineArrive();
		break;
	case 4:
		// Exit at fastpass queue
		fastPassLineExit();
		break;
	case 5:
		// Put people in the HH
		M6FU();
		break;
	case 6:
		// Departure from HH
		exitFunction();
		break;
	}
}

void Simulation_Information::outsideLineArrive(void)
{
	if (arrivingGroupCounter < numberOfGroups)
	{
		// get the next event and determine if it is a fastpass
		arrayOfGroups[arrivingGroupCounter].entranceArrivalTime = massDensityFunction(simulationTime) + simulationTime;

		if (arrayOfGroups[arrivingGroupCounter].ticketType < 3)
		{
			nextEventTypeArray[1] = arrayOfGroups[arrivingGroupCounter].entranceArrivalTime;
		}
		else
		{
			nextEventTypeArray[3] = arrayOfGroups[arrivingGroupCounter].entranceArrivalTime;
		}
	}

	//see if server is busy
	if (outsideServerStatus == BUSY)
	{
		outsideQueue.push(arrayOfGroups[arrivingGroupCounter - 1]);
		if (arrivingGroupCounter < numberOfGroups)
			arrivingGroupCounter++;
	}
	else
	{
		// set the server to busy
		outsideServerStatus = BUSY;

		//schedule departure
		nextEventTypeArray[2] = simulationTime + outsideLineTicketTime;
		outsideQueue.push(arrayOfGroups[arrivingGroupCounter - 1]);
		if ( arrivingGroupCounter < numberOfGroups)
			arrivingGroupCounter++;
	}
}

void Simulation_Information::outsideLineExit(void)
{
	//for error checking purposes
	//see if the queue is empty if it is reset it
	if (outsideQueue.size() == 0)
	{
		outsideServerStatus = IDLE;
		nextEventTypeArray[2] = EMPTY;
	}
	// if the amount inside building is full so we will increment the time
	else if (insideQueue.size() >= amountInsideBuilding)
	{
		nextEventTypeArray[2] = simulationTime + outsideLineTicketTime;
	}
	// if the fastpass queue has no one in it we will push someone on it,
	// or if the last person to bu pushed was a fastpass then we push.
	else if ((fastpassQueue.size() == 0) || (fastpassLastTicketType == true))
	{
	
 		Group temp = outsideQueue.front();

		insideLine(temp);
		outsideQueue.pop();
		fastpassLastTicketType = false;

			// if our queue is now empty change the parameters
			if (outsideQueue.size() == 0)
			{
				outsideServerStatus = IDLE;
				nextEventTypeArray[2] = EMPTY;				
			}
			// otherwise we will set the sim time for the next event for this
			else
			{
				outsideServerStatus = BUSY;
				nextEventTypeArray[2] = simulationTime + outsideLineTicketTime;
			}
	}
}

void Simulation_Information::fastPassLineArrive(void)
{
	if (arrivingGroupCounter < numberOfGroups)
	{
		// get the next event and determine if it is a fastpass
		arrayOfGroups[arrivingGroupCounter].entranceArrivalTime = massDensityFunction(simulationTime);

		if (arrayOfGroups[arrivingGroupCounter].ticketType < 3)
		{
			nextEventTypeArray[1] = arrayOfGroups[arrivingGroupCounter].entranceArrivalTime;
		}
		else
		{
			nextEventTypeArray[3] = arrayOfGroups[arrivingGroupCounter].entranceArrivalTime;
		}
	}

	//see if server is busy
	if (fastpassServerStatus == BUSY)
	{
		fastpassQueue.push(arrayOfGroups[arrivingGroupCounter - 1]);
		arrivingGroupCounter++;
	}
	else
	{
		// set the server to busy
		fastpassServerStatus = BUSY;

		//schedule departure
		nextEventTypeArray[4] = simulationTime + outsideLineTicketTime;

		fastpassQueue.push(arrayOfGroups[arrivingGroupCounter - 1]);

		arrivingGroupCounter++;
	}
}

void Simulation_Information::fastPassLineExit(void)
{
	//for error checking purposes
	//see if the queue is empty if it is reset it
	if (fastpassQueue.size() == 0)
	{
		fastpassServerStatus = IDLE;
		nextEventTypeArray[4] = EMPTY;
	}

	// if the amount inside building is full so we will increment the time
	else if (insideQueue.size() >= amountInsideBuilding)
	{
		nextEventTypeArray[4] = simulationTime + fastpassLineTicketTime;
	}
	else if ((outsideQueue.size() > 0) || (fastpassLastTicketType == true))
	{
		nextEventTypeArray[4] = simulationTime + fastpassLineTicketTime;
	}
	else
	{
		
		Group temp = fastpassQueue.front();
		
		insideLine(temp);
		fastpassQueue.pop();
		fastpassLastTicketType = true;

		// if our queue is now empty change the parameters
		if (fastpassQueue.size() == 0)
		{
			fastpassServerStatus = IDLE;
			nextEventTypeArray[4] = EMPTY;
		}
		// otherwise we will set the sim time for the next event for this
		else
		{
			fastpassServerStatus = BUSY;
			nextEventTypeArray[4] = simulationTime + fastpassLineTicketTime;
		}
	}

}

// this is will push people into the inside line. Originally this was set up for the nextEventType array
// but this was changed since we do not need to set a time for this.
void Simulation_Information::insideLine(Group temp)
{
	const int twoMinutes = 120; 

	int tempNUmber = temp.groupNumber;

	arrayOfGroups[temp.groupNumber].insideLineTime = simulationTime;

	// if no one is in the queue and no ones in the huanted house wait 1 minute 
	// we push and set the next event.
	if (insideQueue.size() == 0 && M6FUQueue.size() == 0)
	{
		insideQueue.push(arrayOfGroups[tempNUmber]);
		insideServerStatus = BUSY;
		nextEventTypeArray[5] = simulationTime + twoMinutes;
	}
	else if (insideQueue.size() == 0 && M6FUQueue.size() > 0)
	{
		insideQueue.push(arrayOfGroups[tempNUmber]);
		insideServerStatus = BUSY;
		nextEventTypeArray[5] = simulationTime + M6FUEnterRate;
	}
	// just push.
	else
	{
		insideQueue.push(arrayOfGroups[tempNUmber]);
	}
}



void Simulation_Information::M6FU(void)
{
	Group temp = insideQueue.front();
	
	M6FUQueue.push(arrayOfGroups[temp.groupNumber]);
	arrayOfGroups[temp.groupNumber].M6FUEnterTime = simulationTime;
	
	if (M6FUQueue.size() <= 1)
	{
		nextEventTypeArray[6] = simulationTime + temp.M6FUTimeLength;
	}

	insideQueue.pop();
	if (insideQueue.size() == 0) 
	{
		insideServerStatus = IDLE;
		nextEventTypeArray[5] = EMPTY;

	}
	else 
	{
		insideServerStatus = BUSY;
		nextEventTypeArray[5] = simulationTime + M6FUEnterRate;
	}
		
	

}

void Simulation_Information::exitFunction(void)
{
	Group temp = M6FUQueue.front();
	
	// stat adjustments
	arrayOfGroups[temp.groupNumber].M6FUExitTime = simulationTime;
	arrayOfGroups[temp.groupNumber].totalTimeInSystem = arrayOfGroups[temp.groupNumber].M6FUExitTime - arrayOfGroups[temp.groupNumber].entranceArrivalTime;
	// end stats
	arrayOfGroups[temp.groupNumber].lineWaitTime = arrayOfGroups[temp.groupNumber].insideLineTime - arrayOfGroups[temp.groupNumber].entranceArrivalTime;
	arrayOfGroups[temp.groupNumber].insideLineTime = arrayOfGroups[temp.groupNumber].M6FUEnterTime - arrayOfGroups[temp.groupNumber].insideLineTime;
	arrayOfGroups[temp.groupNumber].totalLineTime = arrayOfGroups[temp.groupNumber].lineWaitTime + arrayOfGroups[temp.groupNumber].insideLineTime;


	M6FUQueue.pop();
	numberOfGroupsExit++;

	if(M6FUQueue.size() == 0)
	{
		nextEventTypeArray[6] = EMPTY;
	}
	else
	{
		temp = M6FUQueue.front();
		nextEventTypeArray[6] = temp.M6FUTimeLength + temp.M6FUEnterTime;
	}

	std::cout << " this is the number that exited: " << numberOfGroupsExit;
}

void Simulation_Information::updateAverageTimeStats(void)
{
	areaUnderOutsideQueue = outsideQueue.size() * (simulationTime - timeOfLastEvent);
	areaUnderFastpassQueue = fastpassQueue.size() * (simulationTime - timeOfLastEvent);
	areaUnderInsideQueue = insideQueue.size() * (simulationTime - timeOfLastEvent);

	std::ofstream statsData;

	// Open A Stat's Data File
	statsData.open("SimulationStats.csv", std::ofstream::out | std::ofstream::app);

	// Get All Data
	statsData << simulationTime << ", " << outsideQueue.size() << ", " << fastpassQueue.size() << ", " << insideQueue.size() << ", "
		<< M6FUQueue.size() << ", " << areaUnderOutsideQueue << ", " << areaUnderFastpassQueue << ", " << areaUnderInsideQueue << std::endl;

	if (arrivingGroupCounter % 10 == 0)
	{
		// Show Simulation Time, Last Event Time, Update Queue Sizes. Get Some Area Under Curve Statistics 
		std::cout << "Current Simulation Time: " << simulationTime << std::endl;
		std::cout << "Current Outside Queue Size: " << outsideQueue.size() << std::endl;
		std::cout << "Current Fastpass Queue Size: " << fastpassQueue.size() << std::endl;
		std::cout << "Current Inside Queue Size: " << insideQueue.size() << std::endl;
		std::cout << "Current M6FU Queue Size: " << M6FUQueue.size() << std::endl;
		std::cout << "Current Customers In The System: " << (arrivingGroupCounter - numberOfGroupsExit) << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
	}
}

void Simulation_Information::report(void)
{
	int totalGroupOnTickets = 0;
	int totalDoorTickets = 0;
	int totalOtherTickets = 0;
	int totalFastpassTickets = 0;
	int totalMoneyMade = 0;
	int totalFastpassMoney = 0;
	int totalGroupOnMoney = 0;
	int totalDoorMoney = 0;
	int totalOtherMoney = 0;
	
	int otherTicketAmount = 5; // this is a placeholder. Needs to be changed after I find out the actual ticket price.

	int avgGroupSize = 0;


	for (int i = 0; i < arrayOfGroups.size(); i++)
	{
		avgGroupSize += arrayOfGroups[i].groupPeopleNumber;

		if (arrayOfGroups[i].ticketType == 0)
			totalGroupOnTickets++;
		else if (arrayOfGroups[i].ticketType == 1)
			totalDoorTickets++;
		else if (arrayOfGroups[i].ticketType == 2)
			totalOtherTickets++;
		else
			totalFastpassTickets++;	
	}
	avgGroupSize = avgGroupSize / numberOfGroups;
	
	totalDoorMoney = doorTicketAmount * totalDoorTickets;
	totalGroupOnMoney = groupOnTicketAmount * totalGroupOnTickets;
	totalOtherMoney = otherTicketAmount * totalOtherTickets;
	totalFastpassMoney = fastpassTicketAmount * totalFastpassTickets;
	totalMoneyMade = totalDoorMoney + totalGroupOnMoney + totalOtherMoney + totalFastpassMoney;

	std::cout << "Number of groups exit: " << numberOfGroupsExit << std::endl;;
	std::cout << "Total money made: " << totalMoneyMade << std::endl;
}

float massDensityFunction(float simTime)
{
	// F(X) = 24 * 3(1 - X)
	// X is a Random Value Between 0-1 Not Including 1
	float randomNumber = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	// If Random Number is 1 Call Method Again
	// This is because we will return 0 which could give us bugs
	// From two events having the same sim time. 
	if (randomNumber == 1.0)
		return massDensityFunction(simTime);
	
	// this is to make the make it so at the begining and end we
	// get longer times of people showing up.
	if ((simTime < 3600 || simTime > 14400) && randomNumber > .5)
		return massDensityFunction(simTime);

	// this is opposite so that if we are during our peak times we will get smaller arrival times
	if ((simTime > 3600 && simTime < 14400) && randomNumber < .5)
		return massDensityFunction(simTime);

	// Return F(X) = 72(1-X). We should be getting from 0-72
	return 960 * (1 - randomNumber);
}


float M6FUTime(float IntervalLow, float IntervalHigh)
{
	float hauntedHouseTime = 0;
	hauntedHouseTime = (std::rand() % (static_cast<int>(IntervalHigh) - static_cast<int>(IntervalLow))) + IntervalLow;
	return hauntedHouseTime;
}

int PeopleInGroup()
{
	int people = -1;
	people = std::rand() % 7 + 1;
	return people;
}

int TicketTypeSet(int group, int door, int fast, int other)
{
	int ticket = -1;
	ticket = std::rand() % 100;
	if (ticket < group)
		return 0;
	else if (ticket < (group + door))
		return 1;
	else if (ticket < (group + door + other))
		return 2;
	else
		return 3; 
}

