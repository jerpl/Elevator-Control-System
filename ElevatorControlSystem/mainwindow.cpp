#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //setup UI elements
    ui->comboBox->addItem("ElevatorCentered");
    ui->comboBox->addItem("AI");
    connect(ui->confirm,SIGNAL(released()),this,SLOT (confirmStrategy()));

    //setup elevator displays
    ui->e1currentPos->setText("Floor 1");
    ui->e1currentDir->setText("Idle");
    ui->EmergMessage->setText("No Emergency");

    ui->e2currentPos->setText("Floor 1");
    ui->e2currentDir->setText("Idle");
    ui->e2EmergMessage->setText("No Emergency");

    //init ECS
    qecs = new QECS();

    //connect foor buttons
    connect(ui->up, SIGNAL(released()), this, SLOT (requestUp()));
    connect(ui->up2, SIGNAL(released()), this, SLOT (requestUp()));
    connect(ui->down2, SIGNAL(released()), this, SLOT (requestDown()));
    connect(ui->up3,SIGNAL(released()),this,SLOT(requestUp()));
    connect(ui->down3, SIGNAL(released()), this, SLOT (requestDown()));
    connect(ui->down4, SIGNAL(released()), this, SLOT (requestDown()));
    //connect display updated from the timers in the qecs class
    connect(qecs,SIGNAL(uiUpdateE1Display()),this,SLOT (updateE1Display())); //gets the signal from the qecs to update the location info for the elevator on the UI
    connect(qecs,SIGNAL(uiUpdateE2Display()),this,SLOT (updateE2Display()));
    //connect button updater
    connect(qecs,SIGNAL(uiUpdateFloorButtonE1()),this,SLOT (updateFloorButtonE1()));
    connect(qecs,SIGNAL(uiUpdateFloorButtonE2()),this,SLOT (updateFloorButtonE2()));
    connect(qecs,SIGNAL(uiUpdateDestButtonE1()),this,SLOT (updateDestButtonE1()));
    connect(qecs,SIGNAL(uiUpdateDestButtonE2()),this,SLOT (updateDestButtonE2()));
    //connect destination buttons inside elevators
    connect(ui->floor1,SIGNAL(released()),this,SLOT (PassengerReq()));
    connect(ui->floor2,SIGNAL(released()),this,SLOT (PassengerReq()));
    connect(ui->floor3,SIGNAL(released()),this,SLOT (PassengerReq()));
    connect(ui->floor4,SIGNAL(released()),this,SLOT (PassengerReq()));
    connect(ui->e2floor1,SIGNAL(released()),this,SLOT (PassengerReq()));
    connect(ui->e2floor2,SIGNAL(released()),this,SLOT (PassengerReq()));
    connect(ui->e2floor3,SIGNAL(released()),this,SLOT (PassengerReq()));
    connect(ui->e2floor4,SIGNAL(released()),this,SLOT (PassengerReq()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete qecs;
}


void MainWindow::requestUp(){
    if (sender()==ui->up){
        ui->up->setEnabled(false);
        ui->up->setStyleSheet("background-color:green"); //this represents what the "switchOn()" function would do if i had to implement the button class
        qInfo ("Requested car going up from floor 1, button will not work until request is serviced");
        qecs->allocateCar(1,1);
    } else if (sender()==ui->up2) {
        ui->up2->setEnabled(false);
        ui->up2->setStyleSheet("background-color:green");
        qInfo ("Requested car going up from floor 2, button will not work until request is serviced");
        qecs->allocateCar(2,1);
    } else if (sender()==ui->up3) {
        ui->up3->setEnabled(false);
        ui->up3->setStyleSheet("background-color:green");
        qInfo ("Requested car going up from floor 3, button will not work until request is serviced");
        qecs->allocateCar(3,1);
       }
    }

void MainWindow::requestDown(){
    if (sender()==ui->down2){
        ui->down2->setEnabled(false);
        ui->down2->setStyleSheet("background-color:green");
        qInfo ("Requested car going down from floor 2, button will not work until request is serviced");
        qecs->allocateCar(2,0);
    }else if (sender()==ui->down3){
        ui->down3->setEnabled(false);
        ui->down3->setStyleSheet("background-color:green");
        qInfo ("Requested car going down from floor 3, button will not work until request is serviced");
        qecs->allocateCar(3,0);
    } else if (sender()==ui->down4){
        ui->down4->setEnabled(false);
        ui->down4->setStyleSheet("background-color:green");
        qInfo ("Requested car going down from floor 4, button will not work until request is serviced");
        qecs->allocateCar(4,0);
    }
}

void MainWindow::PassengerReq(){
    //elevator 1 request
    if (sender()==ui->floor1){
        ui->floor1->setEnabled(false);
        ui->floor1->setStyleSheet("background-color:green");
        qecs->getElevator(0)->addDest(1,3); //direction will have been decided by which floor button was pressed so 3 is to let the elevator know this is a passenger request
    }else if (sender()==ui->floor2){
        ui->floor2->setEnabled(false);
        ui->floor2->setStyleSheet("background-color:green");
        qecs->getElevator(0)->addDest(2,3);
    }else if (sender()==ui->floor3){
        ui->floor3->setEnabled(false);
        ui->floor3->setStyleSheet("background-color:green");
        qecs->getElevator(0)->addDest(3,3);
    }else if (sender()==ui->floor4){
        ui->floor4->setEnabled(false);
        ui->floor4->setStyleSheet("background-color:green");
        qecs->getElevator(0)->addDest(4,3);
    }//elevator 2 request
    else if (sender()==ui->e2floor1){
        ui->e2floor1->setEnabled(false);
        ui->e2floor1->setStyleSheet("background-color:green");
        qecs->getElevator(1)->addDest(1,3);
    }else if (sender()==ui->e2floor2){
        ui->e2floor2->setEnabled(false);
        ui->e2floor2->setStyleSheet("background-color:green");
        qecs->getElevator(1)->addDest(1,3);
    }else if (sender()==ui->e2floor3){
        ui->e2floor3->setEnabled(false);
        ui->e2floor3->setStyleSheet("background-color:green");
        qecs->getElevator(1)->addDest(1,3);
    }else if (sender()==ui->e2floor4){
        ui->e2floor4->setEnabled(false);
        ui->e2floor4->setStyleSheet("background-color:green");
        qecs->getElevator(1)->addDest(1,3);
    }
}

void MainWindow::confirmStrategy(){
    if (ui->comboBox->currentIndex()==0){
        qecs->setStrategy("ElevatorCentered");
    } else if (ui->comboBox->currentIndex()==1){
        qecs->setStrategy("AI");
    }
}

void MainWindow::updateE1Display(){
    int e1Loc = qecs->getElevator(0)->getCurrentFloor();
    if (qecs->getElevator(0)->getDirection()==1){ //elevator is heading upward
        if (e1Loc==2){
            ui->e1currentPos->setText("Floor 2");
            ui->e1currentDir->setText("Going Up");
        } else if (e1Loc==3){
            ui->e1currentPos->setText("Floor 3");
            ui->e1currentDir->setText("Going Up");
        } else if (e1Loc==4){
            ui->e1currentPos->setText("Floor 4");
            ui->e1currentDir->setText("Going Down");
        }
    } else if (qecs->getElevator(0)->getDirection()==0){ //elevator is heading downward
        if (e1Loc==3){
            ui->e1currentPos->setText("Floor 3");
            ui->e1currentDir->setText("Going Down");
        } else if (e1Loc==2){
            ui->e1currentPos->setText("Floor 2");
            ui->e1currentDir->setText("Going Down");
        } else if (e1Loc==1){
            ui->e1currentPos->setText("Floor 1");
            ui->e1currentDir->setText("Going Up");
        }
    }
    if (qecs->getElevator(0)->getDirection()==2){
        ui->e1currentDir->setText("Idle");
    }
}

void MainWindow::updateE2Display(){
    int e2Loc = qecs->getElevator(1)->getCurrentFloor();
    if (qecs->getElevator(1)->getDirection()==1){
        if (e2Loc==2){
            ui->e2currentPos->setText("Floor 2");
            ui->e2currentDir->setText("Going Up");
        } else if (e2Loc==3){
            ui->e2currentPos->setText("Floor 4");
            ui->e2currentDir->setText("Going Up");
        } else if (e2Loc==4){
            ui->e2currentPos->setText("Floor 4");
            ui->e2currentDir->setText("Going Down");
        }
    } else if (qecs->getElevator(1)->getDirection()==0){
        if (e2Loc==3){
            ui->e2currentPos->setText("Floor 3");
            ui->e2currentDir->setText("Going Down");
        } else if (e2Loc==2){
            ui->e2currentPos->setText("Floor 2");
            ui->e2currentDir->setText("Going Down");
        } else if (e2Loc==1){
            ui->e2currentPos->setText("Floor 1");
            ui->e2currentDir->setText("Going Up");
        }
    }
    if (qecs->getElevator(1)->getDirection()==2){
        ui->e2currentDir->setText("Idle");
    }
}

void MainWindow::updateFloorButtonE1(){
    int pos = qecs->getElevator(0)->getCurrentFloor();
    int dir = qecs->getElevator(0)->getNextDestination().second;
    if (dir==1) {
        if (pos==1){
            ui->up->setEnabled(true);
            ui->up->setStyleSheet("background-color: white");
        } else if (pos==2){
            ui->up2->setEnabled(true);
            ui->up2->setStyleSheet("background-color: white");
        } else {
            ui->up3->setEnabled(true);
            ui->up3->setStyleSheet("background-color: white");
        }
    } else if (dir==0){
        if (pos==4){
            ui->down4->setEnabled(true);
            ui->down4->setStyleSheet("background-color: white");
        } else if (pos==3){
            ui->down3->setEnabled(true);
            ui->down3->setStyleSheet("background-color: white");
        } else {
            ui->down2->setEnabled(true);
            ui->down2->setStyleSheet("background-color: white");
        }
    }
}

void MainWindow::updateFloorButtonE2(){
    int pos = qecs->getElevator(1)->getCurrentFloor();
    int dir = qecs->getElevator(1)->getNextDestination().second;
    qInfo()<<"updatating" << dir << "button on floor" << pos;
    if (dir==1) { //up button
        if (pos==1){
            ui->up->setEnabled(true);
            ui->up->setStyleSheet("background-color: white");
        } else if (pos==2){
            ui->up2->setEnabled(true);
            ui->up2->setStyleSheet("background-color: white");
        } else {
            ui->up3->setEnabled(true);
            ui->up3->setStyleSheet("background-color: white");
        }
    } else if (dir==0){
        if (pos==4){
            ui->down4->setEnabled(true);
            ui->down4->setStyleSheet("background-color: white");
        } else if (pos==3){
            ui->down3->setEnabled(true);
            ui->down3->setStyleSheet("background-color: white");
        } else {
            ui->down2->setEnabled(true);
            ui->down2->setStyleSheet("background-color: white");
        }
    }
}

void MainWindow::updateDestButtonE1(){
    int pos = qecs->getElevator(0)->getCurrentFloor();
    if (pos==1){
        ui->floor1->setEnabled(true);
        ui->floor1->setStyleSheet("background-color: white");
    } else if (pos==2){
        ui->floor2->setEnabled(true);
        ui->floor2->setStyleSheet("background-color: white");
    } else if (pos==3){
        ui->floor3->setEnabled(true);
        ui->floor3->setStyleSheet("background-color: white");
    } else if (pos==4){
        ui->floor4->setEnabled(true);
        ui->floor4->setStyleSheet("background-color: white");
    }
}

void MainWindow::updateDestButtonE2(){
    int pos = qecs->getElevator(1)->getCurrentFloor();
    if (pos==1){
        ui->e2floor1->setEnabled(true);
        ui->e2floor1->setStyleSheet("background-color: white");
    } else if (pos==2){
        ui->e2floor2->setEnabled(true);
        ui->e2floor2->setStyleSheet("background-color: white");
    } else if (pos==3){
        ui->e2floor3->setEnabled(true);
        ui->e2floor3->setStyleSheet("background-color: white");
    } else if (pos==4){
        ui->e2floor4->setEnabled(true);
        ui->e2floor4->setStyleSheet("background-color: white");
    }
}
