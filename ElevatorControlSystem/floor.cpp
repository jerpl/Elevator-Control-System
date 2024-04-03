#include "floor.h"

Floor::Floor(int floorNum): floorNum(floorNum){
    door = new FloorDoor(floorNum);
}

//getter
int Floor::getFloorNum(){
    return this->floorNum;
}

//Main functions
void Floor::openFloorDoors (){
    door->open();
}

void Floor::closeFloorDoors (){
    door->close();
}

