#include "buspositionsignalplanner.h"
#include "bus.h"
#include "logfilepositionsource.h"

#include <QHash>
#include <QDebug>


BusPositionSignalPlanner::BusPositionSignalPlanner() {
    stopped =  true;
    out = false;
    //QObject::connect(this, SIGNAL(start_()), this, SLOT(reanudar()));
    QObject::connect(this, SIGNAL(countChanged()), this, SLOT(checkNumOfObjectToPlanner()));

    for (int j = 0; j <= 100; j++)
        taken[j].isThereSomeone = false;

}

BusPositionSignalPlanner::~BusPositionSignalPlanner() {
    stop();
    getOut();
    this->wait();
    qDebug() << " Thread bus position signal planner have been deleted";
}

void BusPositionSignalPlanner::stop() {
    if (stopped == false)
        stopped = true;
    else
        return;
}

void BusPositionSignalPlanner::setObjectToPlanner(QHash<int, Bus*> &newHash, int *newindexArray ) {
    busHash = &newHash;
    indexArray = newindexArray;
}

void BusPositionSignalPlanner::resumen() {
    if (stopped = true)
        stopped = false;
    else
        return;
}

void BusPositionSignalPlanner::run() {
    while (out == false) {
        while (stopped == false) {
            if (!(busHash->isEmpty())) {
                for (int i = 0; i <= getCount(); i++) {
                    if (isChecked(indexArray[i]) == false && stopped == false) {
                        //qDebug() << "se ha leido el" << indexArray[i];
                        setChecked(indexArray[i], 1);
                        if (busHash->value(indexArray[i])->getLogFileSource()->getHaveNewData() && stopped == false) {
                            emit busHash->value(indexArray[i])->getLogFileSource()->newUpdate(
                                        busHash->value(indexArray[i])->getLogFileSource()->getPositionInfo());

                            stopped = true; //qDebug() << "se detuvo";
                            msleep(1000);
                        }
                    }
                }
                if (allhaveBeenRead() == true)
                     restartTaken();
            }
        }
    }
}

bool BusPositionSignalPlanner::allhaveBeenRead() {
    for (int i = 0; i <= 100; i++) {
        if (taken[i].isThereSomeone == true &&
                taken[i].isReadable == true && taken[i].isWasRead == false)
                return false;
    }
    return true;
}

void BusPositionSignalPlanner::setChecked(int index, int status) {
    for (int i = 0; i <= getCount(); i++) {
        for (int j = 0; j <= 100; j++) {
            if (taken[j].index == index && taken[i].isWasRead != status) {
                taken[j].isWasRead = status;
                 return;
            }
        }

    }
}

bool BusPositionSignalPlanner::isChecked(int index) {
    for (int i = 0; i <= getCount(); i++) {
        for (int j = 0; j <= 100; j++) {
            if (taken[j].index == index && taken[j].isWasRead == 0) {
                return false;
            }

            if (taken[j].index == index && taken[j].isWasRead == 1) {
                return true;
            }
        }
    }
}

void BusPositionSignalPlanner::restartTaken() {
    updateTakenListIndex();
    for (int i = 0; i <= getCount(); i++) {
        for (int j = 0; j <= 100; j++) {
            if (taken[j].index == indexArray[i]) {
                taken[j].isWasRead = false;
                taken[j].isReadable = busHash->value(indexArray[i])->getLogFileSource()->isRunning();
                break;
            }
        }
    }
}

void BusPositionSignalPlanner::updateTakenListIndex() {
    for (int i = 0; i <= getCount(); i++) {
        taken[i].index = indexArray[i];
        taken[i].isThereSomeone = true;
    }
}

void BusPositionSignalPlanner::setCount(const int newCount) {
    if (count != newCount) {
        count = newCount;
        emit countChanged();
    }
    else
        return;
}

void BusPositionSignalPlanner::checkNumOfObjectToPlanner() {
    if (count > 0) {
        stopped = false;
    } else
        return;
}

void BusPositionSignalPlanner::getOut() {
    out = true;
}
