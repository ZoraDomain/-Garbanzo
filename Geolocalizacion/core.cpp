#include "core.h"
#include "marker.h"
#include "bus.h"
#include "coordinate.h"
#include "address.h"
#include "markerpositionsignalplanner.h"
#include "buspositionsignalplanner.h"
#include "logfilepositionsource.h"
#include "checknetwork.h"

#include <stdlib.h>
#include <QMessageBox>
#include <QDebug>
#include <QTimer>

Core::Core(QObject *parent) : QObject(parent) {
    markerPositionSignalPlanner = new MarkerPositionSignalPlanner();
    markerPositionSignalPlanner->setObjectToPlanner(markerHash, IndexOfMarker);

    busPositionSignalPlanner = new BusPositionSignalPlanner();
    busPositionSignalPlanner->setObjectToPlanner(busHash, IndexOfBus);

    ///start thread
    checkNetwork = new CheckNetwork(this);
    QObject::connect(checkNetwork, SIGNAL(onlineChanged(bool)), this, SLOT(setOnline(bool)));
    setOnline(false);
    checkNetwork->start(QThread::NormalPriority);
    ///

    ///this is signal is used to source and status widget
    QObject::connect(this, SIGNAL(newMarkerPosition(int,double,double)), this, SIGNAL(newPosition(int,double,double)));
    QObject::connect(this, SIGNAL(newBusPosition(int,double,double)),    this, SIGNAL(newPosition(int,double,double)));

    numOfMarker = 0;
    numOfBus = 0;
    //QObject::connect(markerPositionSignalPlanner, SIGNAL(newPosition(int,double,double)),
    //                 this, SIGNAL(newMarkerPosition(int,double,double)));
    //markerPositionSignalPlanner->start();
}

Core::~Core() {
    markerPositionSignalPlanner->getOut();
    busPositionSignalPlanner->getOut();
    checkNetwork->stop();
}

void Core::addMarker(QString text, double latitude, double longitude,
                     QString street, QString district, QString city, QString county,
                     QString state, QString countryCode, QString country, QString postalCode) {
    if (isOnline()) {
        int index = 'M' - (int)numOfMarker;
        IndexOfMarker[numOfMarker] = index;
        numOfMarker++;

        Coordinate *tempCoordinate = new Coordinate(latitude, longitude);
        Address *tempAddress = new Address(street, district, city, country, state, countryCode, postalCode);

        Marker *newMarker = new Marker(index ,&text, tempCoordinate, tempAddress);

        markerHash.insert(index, newMarker);
        emit newAddedMarker(newMarker); ///this add the bus in the widget
        emit receivedMarker(index);     ///this is for give the index to qml object

        markButton(index); ///

        QObject::connect(newMarker, SIGNAL(newPosition(int,double,double)), this,
                         SLOT(stopMarkerPositionPlanner(int,double,double)));
        markerPositionSignalPlanner->setCount(numOfMarker);

    } else
        emit showWarningMessageBox(tr("You are offline"),
                                   tr("For add marker you must have a internet connection"));
}

void Core::addBus(QString text, double latitude, double longitude,
                     QString street, QString district, QString city, QString county,
                     QString state, QString countryCode, QString country, QString postalCode) {
    if(isOnline()) {
        const int index = ('B' - (int)numOfBus ) + availableBus;
        IndexOfBus[numOfBus] = index;
        numOfBus++;

        Coordinate *tempCoordinate = new Coordinate(latitude, longitude);
        Address *tempAddress = new Address(street, district, city, country, state, countryCode, postalCode);

        Bus *newBus = new Bus(index ,&text, tempCoordinate, tempAddress);

        busHash.insert(index, newBus);
        emit newAddedBus(newBus); ///this add the bus in the bus widget
        emit receivedBus(index);  ///this is for give the index to qml object

        markButton(index);

        QObject::connect(newBus, SIGNAL(newPosition(int,double,double)), this,
                         SLOT(stopBusPositionPlanner(int,double,double)));
        QObject::connect(newBus, SIGNAL(setChanged(int, bool)), this, SLOT(updateFollowMeForBus(int,bool)));

        busPositionSignalPlanner->setCount(numOfBus);


    } else
        emit showWarningMessageBox(tr("You are offline"),
                                   tr("For add bus you must have a internet connection"));
}

void Core::updateMarker(int index, double latitude, double longitude,
                        QString street, QString district, QString city, QString county,
                        QString state, QString countryCode, QString country, QString postalCode) {
    Coordinate tempCoordinate(latitude, longitude);
    Address tempAddress(street, district, city, country, state, countryCode, postalCode);

    Marker *tempMarker = markerHash.value(index);
    tempMarker->setCoordinate(&tempCoordinate);
    tempMarker->setAddress(&tempAddress);
    checkForMarkerHaveNewdata(index);
    emit changedInfoComplete(tempMarker->getIndex(), tempMarker);

    if (markerPositionSignalPlanner->getStopped() == true)
        markerPositionSignalPlanner->resumen();

}

void Core::updateBus(int index, double latitude, double longitude,
                        QString street, QString district, QString city, QString county,
                        QString state, QString countryCode, QString country, QString postalCode) {
    Coordinate tempCoordinate(latitude, longitude);
    Address tempAddress(street, district, city, country, state, countryCode, postalCode);

    Bus *tempBus = busHash.value(index);
    tempBus->setCoordinate(&tempCoordinate);
    tempBus->setAddress(&tempAddress);
    checkForBusHaveNewdata(index);
    emit changedInfoComplete(tempBus->getIndex(), tempBus);

    if (busPositionSignalPlanner->getStopped() == true)
        busPositionSignalPlanner->resumen();
}

void Core::checkForMarkerHaveNewdata(int index) {
    if (markerHash.value(index)->getLogFileSource()->isRunning() &&
            markerHash.value(index)->getLogFileSource()->getHaveNewData()) {
        markerHash.value(index)->getLogFileSource()->setHaveNewData(false);
    }
}

void Core::checkForBusHaveNewdata(int index) {
    if (busHash.value(index)->getLogFileSource()->isRunning() &&
            busHash.value(index)->getLogFileSource()->getHaveNewData()) {
        busHash.value(index)->getLogFileSource()->setHaveNewData(false);
    }
}

QString Core::ofWhoIsIndex(int index) {
    if (numOfMarker > 0) {
        for (int i = 0; i <= numOfMarker - 1; i++) {
            if (index == IndexOfMarker[i]) {
                return "Marker";
            }
        }
    }

    if (numOfBus > 0) {
        for (int i = 0; i <= numOfBus- 1; i++) {
            if (index == IndexOfBus[i]) {
                return "Bus";
            }
        }
    }

    return "";
}

Core *Core::instance() {
    return this;
}

bool Core::indexisValidForMarker(int index) {
    if (markerHash.contains(index)) {
        return true;
    } else {
        return false;
    }
    return false;
}

bool Core::indexisValidForBus(int index) {
    if (busHash.contains(index)) {
        return true;
    } else {
        return false;
    }
    return false;
}

void Core::removeMarker(int index) {
    delete markerHash.value(index);
    markerHash.remove(index);
    emit markerRemoved(index);
    markerPositionSignalPlanner->setCount(numOfMarker);
}

void Core::removeBus(int index) {
    delete busHash.value(index);
    busHash.remove(index);
    emit busRemoved(index);
    busPositionSignalPlanner->setCount(numOfBus);
}

void Core::markButton(int index) {
    removeButtonMark();
    if (ofWhoIsIndex(index) == "Marker") {
            if(indexisValidForMarker(index))
                getMarker(index)->setFlat(false);
        return;
    }
    if (ofWhoIsIndex(index) == "Bus") {
            if(indexisValidForBus(index))
                getBus(index)->setFlat(false);
        return;
    }
}

void Core::removeButtonMark() {
    if (numOfMarker > 0) {
        for (int i = 0; i < numOfMarker; i++) {
            if (markerHash.value(IndexOfMarker[i])->isFlat() == false)
                markerHash.value(IndexOfMarker[i])->setFlat(true);
        }
    }
    if (numOfBus > 0) {
        for (int j = 0; j < numOfBus; j++) {
            if (busHash.value(IndexOfBus[j])->isFlat() == false)
                busHash.value(IndexOfBus[j])->setFlat(true);
        }
    }
    return;
}

void Core::lookForMarker(int index) {
    if (indexisValidForMarker(index)) {
        markButton(index);
        emit grandedRequestMarker(getMarker(index));
    } else {
        emit indexError(index);
        return;
    }
    return;
}

void Core::lookForBus(int index) {
    if (indexisValidForBus(index)) {
        markButton(index);
        emit grandedRequestBus(getBus(index));
    } else {
        emit indexError(index);
        return;
    }
    return;
}

void Core::changedMarkerName(int index, QString text) {
    getMarker(index)->setText(text);
}

void Core::changeName(int index, QString text) {
    if (ofWhoIsIndex(index) == "Marker")
        changedMarkerName(index, text);
    else
        changedBusName(index, text);
}

void Core::changedBusName(int index, QString text) {
    getBus(index)->setText(text);
}

void Core::updateFollowMeForBus(int index, bool set) {
    if (set  == true) {
        for (int i = 0; i < numOfBus; i++) {
            if (IndexOfBus[i] != index)
                busHash.value(IndexOfBus[i])->setSet(false);
        }
        emit setFollowMeInTheRoot(index, set);
    }
    emit setFollowMeInTheRoot(index, set);
}

void Core::stopMarkerPositionPlanner(int index, double latitude, double longitude) {
    markerPositionSignalPlanner->stop();
    emit newMarkerPosition(index, latitude, longitude);
}

void Core::stopBusPositionPlanner(int index, double latitude, double longitude) {
    busPositionSignalPlanner->stop();
    emit newBusPosition(index, latitude, longitude);
}

void Core::stopMarkerPositionPlanner() {
    markerPositionSignalPlanner->stop();
}

void Core::stopBusPositionPlanner() {
    busPositionSignalPlanner->stop();
}

void Core::testCoordinate(int index) {
    Bus *test = getBus(index);
    qDebug() << test->getCoordinate()->getLatitude();
    qDebug() << test->getCoordinate()->getLongitude();

    qDebug() << test->getAddress()->getStreet();
    qDebug() << test->getAddress()->getCity();
    qDebug() << test->getAddress()->getCountry();
}

void Core::startMarkerPositionSignalPlanner() {
    markerPositionSignalPlanner->start(QThread::NormalPriority);
    qDebug() << "have been called start thread for marker posistion signal planner";
}

void Core::startBusPositionSignalPlanner() {
    busPositionSignalPlanner->start(QThread::NormalPriority);
    qDebug() << "have been called start thread for bus posistion signal planner";
}

void Core::setFollowMe(int index, bool newSet) {
    if (ofWhoIsIndex(index) == "Bus") {
        busHash.value(index)->setSet(newSet);
    }
}

void Core::setOnline(bool newOnline) {
    if (online != newOnline) {
        online = newOnline;
        emit onlineChanged(online);
    }   else
        return;
}

//int Core::numOfMarker = 0;
//int Core::IndexOfMarker[] = {};

//int Core::numOfBus = 0;
//int Core::IndexOfBus[] = {};
