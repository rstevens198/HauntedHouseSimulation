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
			// rate at which people get to inside line
			simulationInput >> insideLineRate;
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
			//simulationInput >> amountInsideBuilding;
		}
		// Close File 
		simulationInput.close();
	}
	else
	{
		// Default Initialization Variables.
		numberOfGroups = 100;
		insideLineRate = 480;
		M6FUEnterRate = 465;
		M6FUIntervalHigh = 900;
		M6FUIntervalLow = 600;
		grouponTicketsPercentage = 50;
		doorTicketPercentage = 40;
		fastpassTcketPercentage = 5;
		otherTicketPercentage = 5;
		groupOnTicketAmount = 4.00;
		doorTicketAmount = 10.00;
		fastpassTicketAmount = 25.00;
		//amountInsideBuilding = 39;


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
	totalGroupOnTickets = 0;
	totalDoorTickets = 0;
	totalOtherTickets = 0;
	peopleInM6FU = 0;
	totalOutsideQueueDelayTime = 0.0;
	totalFastpassQueueDelayTime = 0.0;
	totalinsideQueueDelayTime = 0.0;
	timeSinceLastEvent = 0.0;
	areaUnderOutsideQueue = 0.0;
	areaUnderFastpassQueue = 0.0;
	areaUnderInsideQueue = 0.0;


	arrayOfGroups.resize(numberOfGroups);

	// Initialization the Cars into the car arrays
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
	}
	
	// Initialize event list. Since no customers are present, the departure (service completion) event is eliminated from consideration
	// First arrival
	nextEventTypeArray[1] = simulationTime + massDensityFunction();


	arrayOfGroups[0].entranceArrivalTime = nextEventTypeArray[1];
	
	// Depart entrance queue
	nextEventTypeArray[2] = EMPTY;
	

	// Leave parking space
	nextEventTypeArray[3] = EMPTY;

	// Arrive at exit queue
	nextEventTypeArray[4] = EMPTY;

	// Leave Exit Queue
	nextEventTypeArray[5] = EMPTY;

	// increment the number of groups arrived
	arrivingGroupCounter++;
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
		// Arrival at inside line
		insideLineArrive();
		break;
	case 6:
		// Exit at the inside line
		insideLineExit();
		break;
	case 7:
		// Put people in the HH
		M6FU();
		break;
	case 8:
		// Departure from HH
		exitFunction();
		break;
	}
}

void Simulation_Information::outsideLineArrive(void)
{
	// get the next event and determine if it is a fastpass
	arrayOfGroups[arrivingGroupCounter].entranceArrivalTime = massDensityFunction();

	
		
}

void Simulation_Information::outsideLineExit(void)
{

}

void Simulation_Information::fastPassLineArrive(void)
{
	// get the next event and determine if it is a fastpass
	arrayOfGroups[arrivingGroupCounter].entranceArrivalTime = massDensityFunction();

	if (arrayOfGroups[arrivingGroupCounter].ticketType < 3)
	{
		nextEventTypeArray[1] = arrayOfGroups[arrivingGroupCounter].entranceArrivalTime;
	}
	else
	{
		nextEventTypeArray[2] = arrayOfGroups[arrivingGroupCounter].entranceArrivalTime;
	}


}

void Simulation_Information::insideLine(void)
{

}

void Simulation_Information::M6FU(void)
{

}

void Simulation_Information::exitFunction(void)
{

}

void Simulation_Information::updateAverageTimeStats(void)
{

}

void Simulation_Information::report(void)
{

}

float massDensityFunction()
{
	// F(X) = 24 * 3(1 - X)
	// X is a Random Value Between 0-1 Not Including 1
	float randomNumber = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	// If Random Number is 1 Call Method Again
	if (randomNumber == 1.0)
		return massDensityFunction();

	// Return F(X) = 72(1-X). We should be getting from 0-72
	return 72 * (1 - randomNumber);
}


float M6FUTime(float IntervalLow, float IntervalHigh)
{
	float hauntedHouseTime = 0;
	hauntedHouseTime = IntervalLow - (std::rand() % (static_cast<int>(IntervalHigh) - static_cast<int>(IntervalLow) + 1));
	return hauntedHouseTime;
}

int PeopleInGroup()
{
	int people = -1;
	people = std::rand() % 8 + 1;
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

