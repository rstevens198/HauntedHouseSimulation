#include "SimulationWork.h"

// Mnemonic For Server Busy
#define BUSY 1  
// Mnemonic For Service IDLE
#define IDLE 0  
// Empty Spot
#define EMPTY 1.0e+30

// Global Car Counter
int GourpCounter = 0;

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
			// Number of Parking Spots
			simulationInput >> parkingSpots;
			// Number of cars
			simulationInput >> numberOfCars;
			// Arrival Rate
			simulationInput >> arrivalRate;
			// Interarrival High Rate
			simulationInput >> parkIntervalHigh;
			// Interarrival Low Rate
			simulationInput >> parkIntervalLow;
			// Exit Gate
			simulationInput >> exitGate;
		}
		// Close File 
		simulationInput.close();
	}
	else
	{
		// Default Initialization Variables.
		parkingSpots = 50;
		numberOfCars = 100;
		arrivalRate = 60;
		parkIntervalHigh = 300;
		parkIntervalLow = 120;
		exitGate = 60;
	}

	// Statistical Counters.
	simulationTime = 0.0;
	nextEventType = 0;
	numberOfCustomersDelayed = 0;
	totalNumberOfCustomers = numberOfCars;
	numberInEntranceQueue = 0;
	numberInExitQueue = 0;
	entranceServerStatus = 0;
	exitServerStatus = 0;
	exitServerStatus = IDLE;
	entranceServerStatus = IDLE;
	maxEntranceQueueSize = 0;
	maxExitQueueSize = 0;
	totalEntranceQueueDelayTime = 0.0;
	totalExitQueueDelayTime = 0.0;
	totalSearchTime = 0.0;
	parked = 0;
	nextLeavingCar = 1.0e+29;
	leavingIndex = 0;
	timeSinceLastEvent = 0.0;
	areaUnderEntranceQueue = 0.0;
	areaUnderExitQueue = 0.0;
	areaEntranceServerStatus = 0.0;
	areaExitServerStatus = 0.0;
	exitTime = 0.0;
	amountOfCustomersLeft = 0;

	// Make Parking Spot Array The Correct Size
	parkingLotSpots.resize(parkingSpots);

	// Make Each Parking Spot Null or 0. We will add or replace the spot with zero after the spot is taken. 
	for (int i = 0; i < parkingSpots; i++)
		parkingLotSpots[i] = EMPTY;

	// Resize The Vector To The Number Of Cars in the simulation. 
	// We expand past the number of cars, to make sure we don't get an indexing error.  
	arrayOfCars.resize(numberOfCars * 5);

	// Initialization the Cars into the car arrays
	for (int i = 0; i < numberOfCars * 5; i++)
	{
		// Create The Amount Of Car Objects Requested By The User
		arrayOfCars[i] = Car();

		// Assign A Car Number To Each Car
		arrayOfCars[i].carNumber = i;

		// Set All Value To Zero
		arrayOfCars[i].numberOfTimesLooked = 0;
	}

	// Initialize event list. Since no customers are present, the departure (service completion) event is eliminated from consideration
	// First arrival
	timeOfNextEvent[1] = simulationTime + massDensityFunction();

	// Schedule First Event
	arrayOfCars[0].entranceArrivalTime = timeOfNextEvent[1];

	// Depart entrance queue
	timeOfNextEvent[2] = EMPTY;

	// Leave parking space
	timeOfNextEvent[3] = EMPTY;

	// Arrive at exit queue
	timeOfNextEvent[4] = EMPTY;

	// Leave Exit Queue
	timeOfNextEvent[5] = EMPTY;
}