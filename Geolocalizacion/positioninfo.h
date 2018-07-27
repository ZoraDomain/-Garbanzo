#ifndef POSITIONINFO_H
#define POSITIONINFO_H

#include <QObject>
#include <coordinate.h>

class Coordinate;
class Address;
class PositionInfo : public QObject {
    Q_OBJECT

public:
    explicit PositionInfo(QObject *parent = nullptr);
    PositionInfo(Coordinate &, QObject *parent = nullptr);
    Coordinate *getCoordinate() const {return coordinate;}
    void setCoordinate(Coordinate *);
    void setCoordinate(double latitude = 0.0, double longitude = 0.0);
    Address *getAddress() const {return address;}
    void setAddress(Address *);
    QDateTime *getTimeStamp() {return timeStamp;}
    bool setTimeStamp(QDateTime &);
    bool setTimeStamp(QString &);

private:
    Coordinate *coordinate;
    Address *address;
    QDateTime *timeStamp;
};

#endif // POSITIONINFO_H
