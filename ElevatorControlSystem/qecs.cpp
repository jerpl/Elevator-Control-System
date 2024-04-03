#include "qecs.h"

QECS::QECS(QObject *parent) : QObject(parent), strategy("ElevatorCentered")
{
    //Initialisation: creates elevators, floors, and timers
    Elevator* e1 = new Elevator(1);
    Elevator* e2 = new Elevator(2);
    elevators[0]= e1;
    elevators[1]= e2;
    Floor* f1 = new Floor(1);
    Floor* f2 = new Floor(2);
    Floor* f3 = new Floor(3);
    Floor* f4 = new Floor(4);
    floors[0] = f1;
    floors[1] = f2;
    floors[2] = f3;
    floors[3] = f4;
    QTimer* fsens1 = new QTimer();
    QTimer* fsens2 = new QTimer();
    QTimer* e1IdleSensor = new QTimer();
    QTimer* e2IdleSensor = new QTimer();
    QTimer* e1Loadtimer = new QTimer();
    QTimer* e2Loadtimer = new QTimer();
    fsensors[0] = fsens1;
    fsensors[1] = fsens2;
    fsensors[2] = e1IdleSensor;
    fsensors[3] = e2IdleSensor;
    fsensors[4] = e1Loadtimer;
    fsensors[5] = e2Loadtimer;

    //sensor connections
    connect(fsens1,SIGNAL(timeout()),this,SLOT (e1FloorAndDisplayUpdate()));
    connect(fsens2,SIGNAL(timeout()),this,SLOT (e2FloorAndDisplayUpdate()));
    connect(e1IdleSensor,SIGNAL(timeout()),this,SLOT (e1IdleTimer()));
    connect(e2IdleSensor,SIGNAL(timeout()),this,SLOT (e2IdleTimer()));
    connect(e1Loadtimer,SIGNAL(timeout()),this,SLOT (e1LoadingTimer()));
    connect(e2Loadtimer,SIGNAL(timeout()),this,SLOT (e2LoadingTimer()));

}

//getter
string QECS::getStrategy(){
    return strategy;
}

Elevator* QECS::getElevator(int index){
    return elevators[index];
}

//setter
void QECS::setStrategy(string strat){
    strategy=strat;
    qInfo("strategy changed");
}

//Main Functions
bool QECS::areAllIdle(){
    for (int i = 0; i<MAX_ELEVATORS;i++){
        if (elevators[i]->isIdle()==false){
            return false;
        }
    }
    return true;
}

void QECS::moveIdle(){
    for (int i = 0; i<MAX_ELEVATORS;i++){
        if (elevators[i]->isIdle()){
            moveFromFloor(i,1); //tell whichever elevator is idle to start moving upward
            break;
        }
    }
}

void QECS::moveFromFloor(int index, int dir){
    elevators[index]->start(dir);
    fsensors[index]->start(5000);
}

void QECS::e1FloorAndDisplayUpdate(){
    elevators[0]->updateLocation();  //first, update the location member and display
    emit uiUpdateE1Display();

    if (elevators[0]->getCurrentFloor()==elevators[0]->getNextDestination().first){ //then, check if the elevator has arrived at its destination yet
        //perform expected behaviour when arriving at a floor, bell, elevatorDoor, floorDoor, etc.
        fsensors[0]->stop(); //stop simulating movement
        elevators[0]->setDirection(elevators[0]->getNextDestination().second); //update the direction to match what was requested by the floorButton press
        if (elevators[0]->isIdle()==false){
            elevators[0]->ringBell();
            elevators[0]->openDoor();
            floors[(elevators[0]->getCurrentFloor())-1]->openFloorDoors();
            fsensors[4]->start(5000); //start the loading timer
            emit uiUpdateE1Display();
            qInfo()<<"Loading time commenced, please get into the elevator, when timer ends, you may select a destination inside the elevator";
        }
    }
}

void QECS::e2FloorAndDisplayUpdate(){
    elevators[1]->updateLocation();  //first, update the location member and display
    emit uiUpdateE2Display();

    if (elevators[1]->getCurrentFloor()==elevators[1]->getNextDestination().first){ //check if the elevator has arrived at its destination yet
        fsensors[1]->stop(); //stop simulating movement
        elevators[1]->setDirection(elevators[1]->getNextDestination().second);
        if (elevators[1]->isIdle()==false){
            elevators[1]->ringBell();
            elevators[1]->openDoor();
            floors[(elevators[1]->getCurrentFloor())-1]->openFloorDoors();
            fsensors[5]->start(5000); //start the loading timer
            emit uiUpdateE2Display();
            qInfo()<<"Loading time commenced, please get into the elevator, when timer ends, you may select a destination inside the elevator";
        }
    }
}

void QECS::allocateCar (int floorNum, int direction){
    if (areAllIdle()){      //first checks if there are no active elevators and if so, moves the first one and immidiately tells it where to stop as it's the only request in the system
        elevators[0]->addDest(floorNum,direction);
        moveIdle();
//----------------------------------------------------------------------------------
    } else if (strategy=="ElevatorCentered"){
        qInfo("Using ElevatorCentered strategy to decide which elevator to send..."); //get absolute val of the difference between each elevator's location and the requested floor
        if (abs(elevators[0]->getCurrentFloor()-floorNum)<abs(elevators[1]->getCurrentFloor()-floorNum)){
            elevators[0]->addDest(floorNum,direction);
            if (elevators[0]->isIdle()){moveIdle();}
        }else {
            elevators[1]->addDest(floorNum,direction);
            if (elevators[1]->isIdle()){moveIdle();}
        }
//----------------------------------------------------------------------------------
    } else if (strategy=="AI"){
        qInfo("Using AI strategy to decide which elevator to send...");
        if (elevators[1]->isIdle()){    //if true, E1 is NOT idle but E2 is idle
            if (elevators[0]->getDirection()==direction){
                if (elevators[0]->getCurrentFloor()<=floorNum){ //E1 is on heading toward this floor already so it makes sense to add
                    elevators[0]->addDest(floorNum,direction);
                } else if (elevators[0]->getCurrentFloor()>=floorNum){ //E1 is heading the right direction but is passed the requested floor already
                    elevators[1]->addDest(floorNum,direction);
                    moveIdle();
                }
            } else { //E1 is not heading toward this request
                elevators[1]->addDest(floorNum,direction);
                moveIdle();
            }
        } else {
            if (elevators[0]->getDirection()==direction){
                if (elevators[1]->getDirection()==direction){
                    if (abs(elevators[0]->getCurrentFloor()-floorNum)<abs(elevators[1]->getCurrentFloor()-floorNum)){
                        elevators[0]->addDest(floorNum,direction);
                    } else {
                        elevators[1]->addDest(floorNum,direction);
                    }
                } else {
                    elevators[0]->addDest(floorNum,direction);
                }
            } else {
                elevators[1]->addDest(floorNum,direction);
            }
        }
//----------------------------------------------------------------------------------
    } else {
        qInfo("no strategy spesifeid, unable to complete the request. Please select an allocation strategy in the UI");
    }
}

void QECS::e1IdleTimer(){
    fsensors[2]->stop();
    if (elevators[0]->getNumDests()>0){ // a request came in from another floor while waiting
        //check where the request is relative to current position, need to fix the situation where the elevator heads down because it's nextDestination.second is down
        if (elevators[0]->getNextDestination().first>elevators[0]->getCurrentFloor()){
            moveFromFloor(0,1);
        } else {
            moveFromFloor(0,0);
        }
    } else { // no requests, therefore return to floor 1 to idle
        elevators[0]->addDest(1,2);
        elevators[0]->setDirection(0);
        emit uiUpdateE1Display();
        moveFromFloor(0,0); //tell E1 to move downward
    }
}

void QECS::e2IdleTimer(){
    fsensors[3]->stop();
    if (elevators[1]->getNumDests()>0){
        if (elevators[1]->getNextDestination().first>elevators[1]->getCurrentFloor()){
            moveFromFloor(1,1);
        } else {
            moveFromFloor(1,0);
        }
    } else {
    elevators[1]->addDest(1,2);
    elevators[1]->setDirection(0);
    emit uiUpdateE2Display();
    moveFromFloor(1,0); //tell E2 to move downward
    }
}

void QECS::e1LoadingTimer(){
    fsensors[4]->stop();
    emit uiUpdateFloorButtonE1(); //enable the button assocaited with the request that has been completed
    emit uiUpdateDestButtonE1();
    int reqRemaining = elevators[0]->serviced(); //complete the request
    elevators[0]->ringBell();
    elevators[0]->closeDoor();
    floors[(elevators[0]->getCurrentFloor())-1]->closeFloorDoors();

    if (reqRemaining==0){ //if no requests remain and Elevator isnt already idle on F1, then make it return to F1 and idle there.
        if (elevators[0]->isIdle()==false){
            qInfo("Please select a destination, if none is chosen for 10 seconds, E1 will return to ground floor and idle");
            fsensors[2]->start(10000);
        } else {
            emit uiUpdateE1Display();
            qInfo("E1 is idling on floor 1");
        }
    } else { //if the elevator has more requests to service, begin servicing the next request
        qInfo("E1 is servicing its next request");
        if (elevators[0]->getNextDestination().first>elevators[0]->getCurrentFloor()){
            moveFromFloor(0,1);
        }else{
            moveFromFloor(0,0);
        }
    }
}

void QECS::e2LoadingTimer(){
    fsensors[5]->stop();
    qInfo()<<"Loading complete";
    emit uiUpdateFloorButtonE2();
    emit uiUpdateDestButtonE2();
    int reqRemaining = elevators[1]->serviced(); //complete the request
    elevators[1]->ringBell();
    elevators[1]->closeDoor();
    floors[(elevators[1]->getCurrentFloor())-1]->closeFloorDoors();
    if (reqRemaining==0){   //If no requests remain, activate the IdleTimer
        if (elevators[1]->isIdle()==false){
            qInfo("Please select a destination, if none is chosen for 10 seconds, E2 will return to ground floor and idle");
            fsensors[3]->start(10000);
        } else {
            emit uiUpdateE2Display();
            qInfo("E2 is idling on floor 1");
        }
    } else { //if the elevator has more requests to service, begin servicing the next request
        qInfo("E2 is servicing its next request");
        if (elevators[1]->getNextDestination().first>elevators[1]->getCurrentFloor()){
            moveFromFloor(1,1);
        }else{
            moveFromFloor(1,0);
        }
    }
}

