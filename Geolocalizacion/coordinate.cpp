#include "coordinate.h"

Coordinate::Coordinate(QObject *parent) : QObject(parent) {
    latitude = 0.0;
    longitude = 0.0;
    altitude = 0.0;
}

Coordinate::Coordinate(const Coordinate *coordinate, QObject *parent) : QObject(parent) {
    latitude = coordinate->getLatitude();
    longitude = coordinate->getLongitude();
    altitude = coordinate->getAltitude();
}
/*
Coordinate::Coordinate(float _latitude, float _longitude, float _altitude, QObject *parent)
    : latitude(_latitude), longitude(_longitude), altitude(_altitude), QObject(parent) { }
*/
Coordinate::Coordinate(double _latitude, double _longitude, QObject *parent)
    : latitude(_latitude), longitude(_longitude), QObject(parent) { }

void Coordinate::setLatitude(double newLatitude) {
    if(latitude != newLatitude)
        latitude = newLatitude;
    else return;
}

void Coordinate::setLongitude(double newLongitude) {
    if(longitude != newLongitude)
        longitude = newLongitude;
    else return;
}

void Coordinate::setAltitude(double newAltitude) {
    if(altitude != newAltitude)
        altitude = newAltitude;
    else return;
}

void Coordinate::setCoordinate(Coordinate *newCoordinate) {
    //setAltitude(newCoordinate->getAltitude());
    //setLongitude(newCoordinate->getLongitude());
    //setLatitude(newCoordinate->getLatitude());

    longitude = newCoordinate->getLongitude();
    latitude = newCoordinate->getLatitude();
}

void Coordinate::setCoordinate(double newLatitude, double newLongitude, double newAltitude) {
    latitude = newLatitude;
    longitude = newLongitude;
    altitude = newAltitude;
}

Coordinate &Coordinate::operator =(const Coordinate &newCoordinate) {
    if (&newCoordinate != this) {
        latitude = newCoordinate.getLatitude();
        longitude = newCoordinate.getLongitude();
        altitude = newCoordinate.getAltitude();
    } else {
        return *this;
    }
}

bool Coordinate::operator !=(const Coordinate &coordinate) {
    if (latitude != coordinate.getLatitude() ||
        longitude != coordinate.getLongitude() ||
        altitude != coordinate.getAltitude()) {
        return true;
    } else {
        return false;
    }
}

bool Coordinate::operator ==(const Coordinate &coordinate) {
    if (latitude == coordinate.getLatitude() ||
        longitude == coordinate.getLongitude() ||
        altitude == coordinate.getAltitude()) {
        return true;
    } else {
        return false;
    }
}
