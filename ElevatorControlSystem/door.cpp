#include "door.h"

Door::Door(){}

FloorDoor::FloorDoor(int fNum): floorNum(fNum){}

    void FloorDoor::open(){
        qInfo() << "Floor" << floorNum << "doors opened";
    }
    void FloorDoor::close(){
        qInfo() << "Floor" << floorNum << "doors closed";
    }

ElevatorDoor::ElevatorDoor(int carNum): carNum(carNum){}

    void ElevatorDoor::open(){
        qInfo() << "Elevator" << carNum << "doors opened";
    }
    void ElevatorDoor::close(){
        qInfo() << "Elevator" << carNum << "doors closed";
    }


