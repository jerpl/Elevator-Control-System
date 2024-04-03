#include "elevator.h"

//constructor
Elevator::Elevator(int carNum): carNum(carNum), currentFloor(1), direction(2), numDests(0){//direction 0 == down, 1 == up, 2 == idle
    bell = new Bell();
    door = new ElevatorDoor(carNum);
}

//Setter
void Elevator::setDirection(int dir){direction=dir;}

//Getters
int Elevator::getCarNum(){return carNum;}
int Elevator::getDirection(){return direction;}
int Elevator::getCurrentFloor(){return currentFloor;}
int Elevator::getNumDests(){return numDests;}

std::pair<int,int> Elevator::getNextDestination(){
    if (numDests>0){
        return nextDest;
    } else
        qInfo("this elevator has no destinations");
}

//main functions
bool Elevator::isIdle(){ //direction == 2 when an elevator is not moving
    if (direction==2){
        return true;
    }else{
        return false;
    }
}

void Elevator::start(int dir){
   direction=dir;
}

void Elevator::addDest(int floorNum, int direction){
    if (direction==3){  //first check if this request came from inside the elevator
        if (numDests==0){
            if (isIdle()){
                qInfo("cannot add request from inside an idle elevator");
            }else{
                //Check if request is valid: if the passenger request an elevator going up, they must select a floor abole the current floor. Same logic for going down.
                if ( (getDirection()==1 && getCurrentFloor()<floorNum) || (getDirection()==0 && getCurrentFloor()>floorNum) ){
                    nextDest=std::pair<int,int>(floorNum,getDirection());
                    destinations[0]=floorNum;
                    destinations[1]=getDirection();
                    ++numDests;
                    qInfo()<< "Passenger in Elevator" << getCarNum() << "has selected floor" << floorNum;
                } else {
                    qInfo("You cannot select a floor in the opposide direction that you called the elevator");
                }
            }
        }
    //If we get here, it means the request is not from within the elevator, thus we dont need to check if the request is valid, as the QECS allocateCar() function will have dealt with that
    }else if (numDests==0){
        nextDest=std::pair<int,int>(floorNum,direction);
        destinations[0]=floorNum;
        destinations[1]=direction;
        ++numDests;
        if (getNextDestination().second==2){
            qInfo() << "Elevator" <<getCarNum() << "has not recieved any requests, it willl now return to floor 1 and idle";
        }else{
            qInfo() << "Elevator" <<getCarNum() << "has accepted request to service floor"<<floorNum;
        }
    } else if (numDests<=MAX_DEST){
        destinations[numDests*2]=floorNum;
        destinations[(numDests*2)+1]=direction;
        ++numDests;
        qInfo() << "Elevator" <<getCarNum() << "has accepted request to service floor"<<floorNum;
    } else {
        qInfo("elevator has reached max destinations at one time");
    }
}

int Elevator::serviced(){
    if (nextDest.second==2){ //determine if this was a request to go idle, if so, set the elevator to idle
        direction=2;
    }
    if (numDests>0){//manage the destinations list by shifting contents and then decreating numDests by 1
        for (int i=0;i<numDests*2;++i){
            destinations[i]=destinations[i+2];
        }--numDests;
    }
    qInfo()<<"Elevator" << getCarNum() << "has serviced its request at floor" << currentFloor <<": It has" << numDests << "requests left to service";
    nextDest=std::pair<int,int>(destinations[0],destinations[1]);
    return numDests;

}

void Elevator::updateLocation(){
    if (direction==1 && currentFloor==4){ //reached top floor (this is beacuse this function updates the floor to 4 once its there so when called, it will still be 3)
            direction=0;
    }else if (direction==1 && currentFloor<4){ //moving up
        ++currentFloor;
        qInfo() << "Elevator" << getCarNum() << "has moved to floor" << currentFloor << "heading to" << getNextDestination().first;
    }else if (direction==0 && currentFloor==2 && getNextDestination().second==2){ //reached base floor to idle
        --currentFloor;
        qInfo() << "Elevator" << getCarNum() <<  "has returned to floor 1 to idle until further requests are sent";
        setDirection(2);
    }else if (direction==0 && currentFloor>1){ //moving down
        --currentFloor;
        qInfo() << "Elevator" << getCarNum() << "has moved to floor" << currentFloor << "heading to" << getNextDestination().first;
    }else if (direction==0 && currentFloor==1){ //just services a request on floor 1 and now wishes to idle
        setDirection(2);
    }else {
        qInfo() << "catch all, this should never happen";
    }
}

void Elevator::ringBell(){
    bell->ring();
}

void Elevator::openDoor(){
    door->open();
}

void Elevator::closeDoor(){
    door->close();
}
