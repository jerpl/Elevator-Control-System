#ifndef QECS_H
#define QECS_H

#include <QObject>
#include <iostream>
#include <string>
#include "qglobal.h"
#include "QtDebug"
#include "elevator.h"
#include "floor.h"
#include "QTimer"
#include "cstdlib"

using namespace std;

//the following define number of elevators and floors for this given test of the system
const int MAX_ELEVATORS = 2;
const int MAX_FLOORS = 4;
const int NUM_SENSORS = (MAX_ELEVATORS*3); //each elevator has a timer to simulate a sensor controlling floor movement, + timers to keep doors open + timers to return idle elevators to floor 1
class QECS : public QObject
{
    Q_OBJECT

  public:
    //constructor
    explicit QECS(QObject *parent = nullptr);

    //Getters
    string getStrategy();
    Elevator* getElevator(int index);

    //Setters
    void setStrategy(string strat);


    //main use case functions
    bool areAllIdle(); // none of the elevators are in operation
    void moveIdle(); //moves the first elevator in the list that is currently idle
    void moveFromFloor(int index, int dir); //basic move function that will tell elevators to leave the floor they're on
    void allocateCar (int floorNum, int direction); //commands an elevator to service a request, can be adjusted by chosing a strategy on the UI
    void openDoors(int floorNum, int carNum); //opens the elevator and floor doors
    void coseDoors(int floorNum, int carNum); //closes the elevator and floor doors
    void serviced (int floorNum, int direction); //called when an elevator services a request, communicates this with the main window to turn the button lights off and

    //safety functions

    void forceIdle(int carNum); //prevents movement until moveIdle is called, allows the ECS to VITO elevator movement in the case of emergency
    bool callSafetyService (int carNum); //will return true if they respond, false if they don't
    void connectToCar (int carNum); //connects building safety service to the car if "callSafetyService" returns true
    void callPolice ();
    void sendEmergMessage (string EmergType);
    void powerOut();

  signals:
    //signals are not implimented the the .cpp file
    //update direction and location of an elevator on the UI
    void uiUpdateE1Display();
    void uiUpdateE2Display();
    //reflect completed requests on the UI by enabling and turning the "lights" on a button off
    void uiUpdateFloorButtonE1();
    void uiUpdateFloorButtonE2();
    void uiUpdateDestButtonE1();
    void uiUpdateDestButtonE2();

  public slots:
    //The following functions update the currentFloor member of elevator class and change the information accordingly in the UI, they also stop the elevator from moving past its destination
    void e1FloorAndDisplayUpdate();
    void e2FloorAndDisplayUpdate();
    //Below function handels the behaviour of an elevator while it is waiting at a floor for passngers to make request, if new requests come in it decides where to go, if not, it goes to floor 1 to idle
    void e1IdleTimer();
    void e2IdleTimer();
    //Below handles loading behaviour, keeps the doors open and informs the passenger (TA) that they can select a floor once this timer is done and the idle timer begins
    void e1LoadingTimer();
    void e2LoadingTimer();


  private:
    //member variables
    Elevator* elevators[MAX_ELEVATORS];
    Floor* floors[MAX_FLOORS];
    QTimer* fsensors[NUM_SENSORS]; //Qtimers represent snesors, one is assiciated with each elevator and when activated, they will simulate the moment of elevators between
                                   //  floors over time intervals
    string strategy;


};

#endif // QECS_H
