# HauntedHouseSimulation
simulation for a haunted house to determine how to make the most money
README

Program description: This program is used as a haunted house simulator for Motel 6 Feet Under. It is set up so that the program runs the same is the world haunted house for the year 2015. The way the this haunted house was set up is that there is a outside queue and a fastpass fueue which both merge into the inside queue. from their the people go into the haunted house, spend 8-15 minutes in it and then leave.  a visual diagram is below. the i stands for inside line, the f is fastpass, and the 0 is outside line. The purpose of this program is to determine type of ticket is best to sell to make the most profit. The user can change the probability of each ticket type, the amount of each ticket type, the amount of groups who go through, the amount of groups allowed inside (this is due to fire codes), the rates that people will be let in the haunted house and inside the queues, and the high and low value for the amount of time spent in the haunted house. 

      i
      i
ffff
      000000000000

File Manifest:

.gitignore README.md simulationValues.txt HauntedHouseSimulation HauntedHouseSimulation.sln HauntedHouseSimulation.vcxproj HauntedHouseSimulation.vcxproj.filters Main.cpp SimulationStats.csv SimulationWork.cpp SimulationWork.h

The source code can be found under SimulationWork.h, SimulationWork.cpp, and main.cpp. Simulation.cppWork is used to do all the work and the Main is just used to call it. Main has a while loop which calls timing() the updateAverageTimeStats() and the chooseNextEvent() functions. The timing determines what will happen next and the chooseNextEvent() goes to where it needs to. 

For the mass density function what I did was I took the average time that groups would show up the the haunted house ( which is based off of the ticket sales) and I set it as the scalar. I also made it so at first hour the haunted house is opened groups come in at a longer rate and during peak hourse groups come in at smaller rates. It is important to note that all time is done in seconds. This can be changed to minutes but I figured it would be nicer to be more accurate. 

Compile Instructions: The user needs to run this program on visual studios 2015 which is free to download. the user will unzup the file and double click the HauntedHouseSimulation.sln. this will open up visual studios 2015. The user will then have build the program in visual studios before they can run it.

If the user wishes to compile it using visual studios 2013 they will nees to right click on the prject, go to Properties. Under Configuration Properties, go to general. Change your platform toolset to one of the availiabe ones.

Operating Instructions: Once the program is built then the user runs it in visual studios. If the user wants the program to read from the simulationValues.txt then they need to set it up so that this is a command line argument. this can be done by running the program through the command line or by setting this up in visual studios by:
1.clicking debug
2. properties.
3. configuration properties.
4. debugging.
5. and setting the command line arguments to the location of the simulationValues.txt location. 

Design Choices: As mentioned above the time is based in seconds. This is due to have a greater accuracy.The stoping condition is also when an amount of groups exit the haunted House. I debated on having this the stopping condition or having a time be a stopping condition. I ultimately decided on having this as the stopping condition since their were times when we were open an extra hour due to more customers. Because of this if we were posted our times for 5 hours we would sometimes be open for 6 hours or 6 and a half hours since we opened early at times. 

Extra Features: As mentioned before the user can either set the input variables in the program itself or they can use the simulationValues.txt file. the simulationValues.txt is as follows:
1.numberOfGroups = 75; 
2.  M6FUEnterRate = 240;
3.  M6FUIntervalHigh = 600;
4.  M6FUIntervalLow = 480;
5.  grouponTicketsPercentage = 55;
6.  doorTicketPercentage = 33;
7.  fastpassTcketPercentage = 4;
8.  otherTicketPercentage = 8;
9.  groupOnTicketAmount = 2.00;
10. doorTicketAmount = 5.00;
11. fastpassTicketAmount = 13.50;
12. amountInsideBuilding = 4;

Also I output the areaunder the queues in a .clr file. The reason why I didnt put the end report in this is because the end report are one time stats and if the user wanted to compare the stats over several runs then though would have to put them in a different file since the .clr file is overwritten everytime the program is called. 

Since we have the fastpass and the outside line merge into one line (inside queue) this program is set up so that if bothe the lines have people in it then it will pull from the groups every other. This is to simulate the real world haunted house. The user can also change how many groups are allowed inside at once (due to fire code and the haunted house floor plan will change next year but not the building). 


Known bugs: The only known bug at this point is when the simulation gets dows to the last group it will push a copy of him back onto the beggining of the ouatside queue. This does not mess with the other groups but it makes the last group kind of useless. That being said if we do not look at the last group then this prolem is not an issue. sometimes this does hav advert effects on the area under the queue especially at the end but since the client was not interested in that this is not a big deal. 

Lessons Learned: When I started to write this I thought it would be much easier and take much less time then it did. The timing was the same as any discrete event simulation but that was it. The rest of the program was very different than the parking lot simulation.
It goes to show that the core concepts once learned are able to carry over. 
