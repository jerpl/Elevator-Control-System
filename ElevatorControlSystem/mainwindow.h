#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "floor.h"
#include "elevator.h"
#include "qecs.h"

/*
CLASS PURPOSE
    The mainWindow is the communication between the Elevator Control System and the User Interface
*/

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    //ui
    Ui::MainWindow *ui;

    //members
    QECS* qecs;


    //functions




private slots:
    void requestUp(); //Send a request to the control system for an elevator moving up
    void requestDown(); //Send a request to the control system for an elevator moving up
    void confirmStrategy(); //Changes the strategy used by the control system to allocate an elevator
    void updateE1Display(); //These 2 update the direction and location info on the UI
    void updateE2Display();
    void PassengerReq(); //sends a destination request from inside an elevator
    void updateFloorButtonE1(); //These 4 update the button color and set buttons to be active again once a request is complete
    void updateFloorButtonE2();
    void updateDestButtonE1();
    void updateDestButtonE2();

    //void helpPress();

};
#endif // MAINWINDOW_H
