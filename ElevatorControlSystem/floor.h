#ifndef FLOOR_H
#define FLOOR_H

#include <iostream>
#include <string>
#include "qglobal.h"
#include "QtDebug"
#include "door.h"

using namespace std;

class Floor {

  public:
    //constructor
    Floor(int floorNum);

    //getter
    int getFloorNum();

    //Main functions
    void openFloorDoors ();
    void closeFloorDoors ();

  private:
    //member variables
    int floorNum;
    FloorDoor* door;

};

#endif // FLOOR_H


