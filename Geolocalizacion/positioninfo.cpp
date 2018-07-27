#include "positioninfo.h"
#include "coordinate.h"
#include "address.h"
#include <QDateTime>

PositionInfo::PositionInfo(QObject *parent)
    : QObject(parent), coordinate(new Coordinate(this)), address(new Address(this)), timeStamp(new QDateTime()) { }

PositionInfo::PositionInfo(Coordinate &_coordinate, QObject *parent)
    : QObject(parent), coordinate(new Coordinate(&_coordinate)), address(new Address(this)), timeStamp(new QDateTime()) { }


void PositionInfo::setCoordinate(Coordinate *newCoordinate) {
    coordinate->setCoordinate(newCoordinate);
}

void PositionInfo::setCoordinate(double latitude, double longitude) {
    coordinate->setLatitude(latitude);
    coordinate->setLongitude(longitude);
}

void PositionInfo::setAddress(Address *newAddress) {
    address->setAddress(newAddress);
}

bool PositionInfo::setTimeStamp(QDateTime &_timeStamp) {
    if (_timeStamp.isValid()) {
        timeStamp = &_timeStamp;
        return true;
    }
    return false;
}

bool PositionInfo::setTimeStamp(QString &timeInString) {
    QDateTime _timeStamp(QDateTime::fromString(timeInString));

    if (_timeStamp.isValid()) {
        timeStamp = &_timeStamp;
        return true;
    }
    return false;
}
