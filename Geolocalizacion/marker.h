#ifndef MARKER_H
#define MARKER_H

#include <QPushButton>

class Coordinate;
class Address;
class LogFilePositionSource;
class PositionInfo;
class Marker : public QPushButton {
    Q_OBJECT
public:
    explicit Marker(QWidget *parent = 0);
    Marker(int _index, QWidget *parent = 0);
    Marker(int _index, QString *text, QWidget *parent = 0);
    Marker(int _index, QString *text, Coordinate *_coordinate = Q_NULLPTR,
           Address *_address = Q_NULLPTR, QWidget *parent = 0);
    Address *getAddress() const;
    Coordinate *getCoordinate() const;
    int getIndex() const {return index;}
    void setAddress(Address *newAddress);
    void setCoordinate(Coordinate *newCooridnate);
    void setIndex(int newIndex);
    bool getSet() const {return set;}
    LogFilePositionSource *getLogFileSource() {return logFile;}

    QString getClassName() const {return className;}

private:
    int index;
    bool set; //this is if you want to move the marker with a nmea log
    Coordinate *coordinate;
    Address *address;
    LogFilePositionSource *logFile;

    QString className; //this is for know that this is a marker

signals:
    void newPosition(int, double, double); //the new position is valid || index, latitude, longitude ||

private slots:
    void checkNewUpdate(PositionInfo*);
};

#endif // MARKER_H
