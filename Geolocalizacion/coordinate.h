#ifndef COORDINATE_H
#define COORDINATE_H

#include <QObject>

class Coordinate : public QObject {
    Q_OBJECT
public:
    explicit Coordinate(QObject *parent = 0);
    explicit Coordinate(const Coordinate *coordinate, QObject *parent = Q_NULLPTR);
    //explicit Coordinate(float _latitude = 0.0, float _longitude = 0.0, float _altitude = 0.0, QObject *parent = 0);
    Coordinate(double _latitude = 0.0, double _longitude = 0.0, QObject *parent = 0);
    double getLatitude()  const {return latitude;}
    double getLongitude() const {return longitude;}
    double getAltitude()  const {return altitude;}
    void setLatitude(double newLatitude = 0.0);
    void setLongitude(double newLongitude = 0.0);
    void setAltitude(double newAltitude = 0.0);
    void setCoordinate(Coordinate *newCoordinate);
    void setCoordinate(double newLatitude = 0.0, double newLongitude = 0.0, double newAltitude = 0.0);

    Coordinate &operator=(const Coordinate &);
    bool operator !=(const Coordinate &);
    bool operator ==(const Coordinate &);

private:
    double latitude;
    double longitude;
    double altitude;
};

#endif // COORDINATE_H
