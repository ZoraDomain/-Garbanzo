#ifndef BUS_H
#define BUS_H

#include <QPushButton>

class Coordinate;
class Address;
class LogFilePositionSource;
class PositionInfo;
class Bus : public QPushButton {
    Q_OBJECT
public:
    explicit Bus(QWidget *parent = 0);
    Bus(int _index, QWidget *parent = 0);
    Bus(int _index, QString *text, QWidget *parent = 0);
    Bus(int _index, QString *text, Coordinate *_coordinate = Q_NULLPTR,
           Address *_address = Q_NULLPTR, QWidget *parent = 0);
    Address *getAddress() const;
    Coordinate *getCoordinate() const;
    int getIndex() const {return index;}
    void setAddress(Address *newAddress);
    void setCoordinate(Coordinate *newCooridnate);
    void setIndex(int newIndex);
    bool getSet() const {return set;}
    void setSet(bool);
    LogFilePositionSource *getLogFileSource() {return logFile;}

    QString getClassName() const {return className;}

private:
    int index;
    bool set; //this is for active update the center of map with bus coordinate
    Coordinate *coordinate;
    Address *address;
    LogFilePositionSource *logFile;

    QString className; //this is for know that this is a bus
signals:
    void newPosition(int, double, double); //the new position is valid || index, latitude, longitude ||
    void setChanged(int, bool); // this is for update follow me in the root (QML). but first is procced in core
    //void setIsDesactive(int, bool); // this is for disble follow me in the root (QML)

private slots:
    void checkNewUpdate(PositionInfo*);
};

#endif // BUS_H
