#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <iostream>
#include <string>
#include "qglobal.h"
#include "QtDebug"
#include "bell.h"
#include "door.h"

using namespace std;

const int MAX_DEST = 20; // this number is double the maximum number of destination requests a single elevator can store to service, this is becasue i will store 2 integers for each request, floor and direction

class Elevator {

  public:
    //constructor
    Elevator(int carNum); //the destination list does not need to be populated on creation, it will be populated when the elevator recieves a request

    //getters
    int getCarNum();
    int getDirection();
    int getCurrentFloor();
    int getNumDests();
    std::pair<int,int> getNextDestination();

    //setter
    void setDirection(int dir);

    //main functions
    bool isIdle ();
    void start(int direction);
    void addDest(int floorNum, int direction); //Handels requests from floor buttons and from passengers in the elevator. Also manages the elevators list of destinations to reflect the new request
    int serviced(); //an elevator has serviced the first request in its list, removes that request from the list and prepares for its next request
    void updateLocation(); //used to update it's location every time it passes a floor (simulated by Qtimer object representing a sensor)
    void ringBell(); //rings a bell to inform passengers that the doors are going to close or open
    void openDoor(); //opens elevator doors
    void closeDoor(); //closes elevator doors


  private:
    //member variables
    int carNum;
    int currentFloor;
    int direction; //i will use ints for the direction so they can be stored in the same list as the destinations, 0 is down, 1 is up, 2 is idle
    int destinations[MAX_DEST]; //the elevator will store its own destination list consisting of an int representingthe destination floor, then an int for direction , then the next destination repeats the pattern
                                //EXAMPLE: destinations[] = [3,0,2,0,1,1] means there are three destinations, the frist is floor 3 going down, then floor 2 going down, then floor 1 going up
    int numDests;
    std::pair<int,int> nextDest;
    Bell* bell;
    ElevatorDoor* door;
};
#endif // ELEVATOR_H
