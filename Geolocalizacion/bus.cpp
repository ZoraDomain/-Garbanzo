#include "bus.h"
#include "coordinate.h"
#include "address.h"
#include "logfilepositionsource.h"
#include "positioninfo.h"

#include <QPushButton>
#include <QDebug>

Bus::Bus(QWidget *parent) : QPushButton(parent) {
    className = "Bus";
    setFlat(true);
    set = false;
    setStyleSheet(QString("QPushButton { background-color: #fffcbd; Text-align:left } " ));

    ///active the logfile
    /// //this active the log file for catch coordinate from file //index, latitude, longitude
    logFile = new LogFilePositionSource(this);
    QObject::connect(logFile, SIGNAL(newUpdate(PositionInfo*)), this, SLOT(checkNewUpdate(PositionInfo*)));
}

Bus::Bus(int _index, QWidget *parent) : index(_index), QPushButton(parent) {
    className = "Bus";
    setFlat(true);
    setStyleSheet(QString("QPushButton { background-color: #fffcbd; Text-align:left } " ));
    set = false;

    ///active the logfile
    /// //this active the log file for catch coordinate from file //index, latitude, longitude
    logFile = new LogFilePositionSource(this);
    QObject::connect(logFile, SIGNAL(newUpdate(PositionInfo*)), this, SLOT(checkNewUpdate(PositionInfo*)));
}

Bus::Bus(int _index, QString *text, QWidget *parent) : index(_index), QPushButton(parent) {
    className = "Bus";
    setText(*text);
    setStyleSheet(QString("QPushButton { background-color: #fffcbd; Text-align:left } " ));
    setFlat(true);
    set = false;

    ///active the logfile
    /// //this active the log file for catch coordinate from file //index, latitude, longitude
    logFile = new LogFilePositionSource(this);
    QObject::connect(logFile, SIGNAL(newUpdate(PositionInfo*)), this, SLOT(checkNewUpdate(PositionInfo*)));
}

Bus::Bus(int _index, QString *text, Coordinate *_coordinate, Address *_address, QWidget *parent) :
    index(_index), coordinate(_coordinate), address(_address), QPushButton(parent) {
    className = "Bus";
    setFlat(true);
    setStyleSheet(QString("QPushButton { background-color: #fffcbd; Text-align:left } " ));
    setText(*text);
    set = false;

    ///active the logfile
    /// //this active the log file for catch coordinate from file //index, latitude, longitude
    logFile = new LogFilePositionSource(this);
    QObject::connect(logFile, SIGNAL(newUpdate(PositionInfo*)), this, SLOT(checkNewUpdate(PositionInfo*)));
}

void Bus::setAddress(Address *newAddress) {
    address->setStreet(newAddress->getStreet());
    address->setDistrict(newAddress->getDistrict());
    address->setCity(newAddress->getCity());
    address->setCountry(newAddress->getCountry());
    address->setState(newAddress->getState());
    address->setCountryCode(newAddress->getCountryCode());
    address->setPostalCode(newAddress->getPostalCode());
}

void Bus::setCoordinate(Coordinate *newCoordinate) {
    coordinate->setLatitude(newCoordinate->getLatitude());
    coordinate->setLongitude(newCoordinate->getLongitude());
}

Address *Bus::getAddress() const {
    return this->address;
}

Coordinate *Bus::getCoordinate() const {
    return this->coordinate;
}

void Bus::setIndex(int newIndex) {
    if (index != newIndex)
        index = newIndex;
    else return;
}

void Bus::setSet(bool newSet) {
    bool tempSet = getSet();
    if (set != newSet) {
        set = newSet;
        if (set == true) {
            emit setChanged(this->index, set);
            return;
        } else if (set == false && tempSet == true) {
            emit setChanged(this->index, set);
            return;
        }
    }
}

void Bus::checkNewUpdate(PositionInfo *newPositionInfo) {
    if (newPositionInfo->getCoordinate()->getLatitude() != this->getCoordinate()->getLatitude() ||
        newPositionInfo->getCoordinate()->getLongitude() != this->getCoordinate()->getLongitude()) {
        this->setCoordinate(newPositionInfo->getCoordinate());
        //emit newPosition(index, QString::number(getCoordinate()->getLatitude()), QString::number(getCoordinate()->getLongitude()));
        emit newPosition(index, getCoordinate()->getLatitude(), getCoordinate()->getLongitude());
    }
    else
        return;
}
