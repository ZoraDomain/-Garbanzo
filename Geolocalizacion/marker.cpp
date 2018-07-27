#include "marker.h"
#include "coordinate.h"
#include "address.h"
#include "logfilepositionsource.h"
#include "positioninfo.h"

#include <QPushButton>

Marker::Marker(QWidget *parent) : QPushButton(parent) {
    className = "Marker";
    setFlat(true);
    setStyleSheet(QString("QPushButton { background-color: #fffcbd; Text-align:left } " ));

    ///active the logfile
    /// //this active the log file for catch coordinate from file //index, latitude, longitude
    logFile = new LogFilePositionSource(this);
    QObject::connect(logFile, SIGNAL(newUpdate(PositionInfo*)), this, SLOT(checkNewUpdate(PositionInfo*)));
}

Marker::Marker(int _index, QWidget *parent) : index(_index), QPushButton(parent) {
    className = "Marker";
    setFlat(true);
    setStyleSheet(QString("QPushButton { background-color: #fffcbd; Text-align:left } " ));

    ///active the logfile
    /// //this active the log file for catch coordinate from file //index, latitude, longitude
    logFile = new LogFilePositionSource(this);
    QObject::connect(logFile, SIGNAL(newUpdate(PositionInfo*)), this, SLOT(checkNewUpdate(PositionInfo*)));
}

Marker::Marker(int _index, QString *text, QWidget *parent) : index(_index), QPushButton(parent) {
    className = "Marker";
    setText(*text);
    setStyleSheet(QString("QPushButton { background-color: #fffcbd; Text-align:left } " ));
    setFlat(true);

    ///active the logfile
    /// //this active the log file for catch coordinate from file //index, latitude, longitude
    logFile = new LogFilePositionSource(this);
    QObject::connect(logFile, SIGNAL(newUpdate(PositionInfo*)), this, SLOT(checkNewUpdate(PositionInfo*)));
}

Marker::Marker(int _index, QString *text, Coordinate *_coordinate, Address *_address, QWidget *parent) :
    index(_index), coordinate(_coordinate), address(_address), QPushButton(parent) {
    className = "Marker";
    setFlat(true);
    setStyleSheet(QString("QPushButton { background-color: #fffcbd; Text-align:left } " ));
    setText(*text);

    ///active the logfile
    /// //this active the log file for catch coordinate from file //index, latitude, longitude
    logFile = new LogFilePositionSource(this);
    QObject::connect(logFile, SIGNAL(newUpdate(PositionInfo*)), this, SLOT(checkNewUpdate(PositionInfo*)));
}

void Marker::setAddress(Address *newAddress) {
    address->setStreet(newAddress->getStreet());
    address->setDistrict(newAddress->getDistrict());
    address->setCity(newAddress->getCity());
    address->setCountry(newAddress->getCountry());
    address->setState(newAddress->getState());
    address->setCountryCode(newAddress->getCountryCode());
    address->setPostalCode(newAddress->getPostalCode());
}

void Marker::setCoordinate(Coordinate *newCoordinate) {
    coordinate->setLatitude(newCoordinate->getLatitude());
    coordinate->setLongitude(newCoordinate->getLongitude());
}

Address *Marker::getAddress() const {
    return this->address;
}

Coordinate *Marker::getCoordinate() const {
    return this->coordinate;
}

void Marker::setIndex(int newIndex) {
    if (index != newIndex)
        index = newIndex;
    else return;
}

void Marker::checkNewUpdate(PositionInfo *newPositionInfo) {
    if (newPositionInfo->getCoordinate()->getLatitude() != this->getCoordinate()->getLatitude() ||
        newPositionInfo->getCoordinate()->getLongitude() != this->getCoordinate()->getLongitude()) {
        this->setCoordinate(newPositionInfo->getCoordinate());
        //emit newPosition(index, QString::number(getCoordinate()->getLatitude()), QString::number(getCoordinate()->getLongitude()));
        emit newPosition(index, getCoordinate()->getLatitude(), getCoordinate()->getLongitude());
    }
    else
        return;
}
