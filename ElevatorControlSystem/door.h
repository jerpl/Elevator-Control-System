#ifndef DOOR_H
#define DOOR_H

#include "qglobal.h"
#include "qdebug.h"

using namespace std;

class Door {
public:
    Door();

    void open();
    void close();

};

class ElevatorDoor : public Door {
public:
    ElevatorDoor(int carNum);
    void open();
    void close();
private:
    int carNum;
};

class FloorDoor : public Door {
public:
    FloorDoor(int floorNum);
    void open();
    void close();
private:
    int floorNum;
};

#endif // DOOR_H
