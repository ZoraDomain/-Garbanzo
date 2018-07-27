#include "markerpositionsignalplanner.h"
#include "marker.h"
#include "logfilepositionsource.h"

#include <QHash>
#include <QDebug>


MarkerPositionSignalPlanner::MarkerPositionSignalPlanner() {
    stopped =  true;
    out = false;
    //QObject::connect(this, SIGNAL(start_()), this, SLOT(reanudar()));
    QObject::connect(this, SIGNAL(countChanged()), this, SLOT(checkNumOfObjectToPlanner()));

    for (int j = 0; j <= 100; j++)
        taken[j].isThereSomeone = false;

}

MarkerPositionSignalPlanner::~MarkerPositionSignalPlanner() {
    stop();
    getOut();
    this->wait();
    qDebug() << " Thread marker position signal planner has stopped";
}

void MarkerPositionSignalPlanner::stop() {
    if (stopped == false)
        stopped = true;
    else
        return;
}

void MarkerPositionSignalPlanner::setObjectToPlanner(QHash<int, Marker *> &newHash, int *newindexArray ) {
    markerHash = &newHash;
    indexArray = newindexArray;
}

void MarkerPositionSignalPlanner::resumen() {
    if (stopped = true)
        stopped = false;
    else
        return;
}

void MarkerPositionSignalPlanner::run() {
    while (out == false) {
        while (stopped == false) {
            if (!(markerHash->isEmpty())) {
                for (int i = 0; i <= getCount(); i++) {
                    if (isChecked(indexArray[i]) == false && stopped == false) {
                        //qDebug() << "se ha leido el" << indexArray[i];
                        setChecked(indexArray[i], 1);
                        if (markerHash->value(indexArray[i])->getLogFileSource()->getHaveNewData() && stopped == false) {
                            emit markerHash->value(indexArray[i])->getLogFileSource()->newUpdate(
                                        markerHash->value(indexArray[i])->getLogFileSource()->getPositionInfo());

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

bool MarkerPositionSignalPlanner::allhaveBeenRead() {
    for (int i = 0; i <= 100; i++) {
        if (taken[i].isThereSomeone == true &&
                taken[i].isReadable == true && taken[i].isWasRead == false)
                return false;
    }
    return true;
}

void MarkerPositionSignalPlanner::setChecked(int index, int status) {
    for (int i = 0; i <= getCount(); i++) {
        for (int j = 0; j <= 100; j++) {
            if (taken[j].index == index && taken[i].isWasRead != status) {
                taken[j].isWasRead = status;
                 return;
            }
        }

    }
}

bool MarkerPositionSignalPlanner::isChecked(int index) {
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

void MarkerPositionSignalPlanner::restartTaken() {
    updateTakenListIndex();
    for (int i = 0; i <= getCount(); i++) {
        for (int j = 0; j <= 100; j++) {
            if (taken[j].index == indexArray[i]) {
                taken[j].isWasRead = false;
                taken[j].isReadable = markerHash->value(indexArray[i])->getLogFileSource()->isRunning();
                break;
            }
        }
    }
}

void MarkerPositionSignalPlanner::updateTakenListIndex() {
    for (int i = 0; i <= getCount(); i++) {
        taken[i].index = indexArray[i];
        taken[i].isThereSomeone = true;
    }
}

void MarkerPositionSignalPlanner::setCount(const int newCount) {
    if (count != newCount) {
        count = newCount;
        emit countChanged();
    }
    else
        return;
}

void MarkerPositionSignalPlanner::checkNumOfObjectToPlanner() {
    if (count > 0) {
        stopped = false;
    } else
        return;
}

void MarkerPositionSignalPlanner::getOut() {
    out = true;
}
