#ifndef STATUSWIDGET_H
#define STATUSWIDGET_H

#include <QWidget>

class QGroupBox;
class QLabel;
class Coordinate;
class Address;
class Marker;
class Bus;
class QLineEdit;
class QHBoxLayout;
class QCheckBox;
class StatusWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StatusWidget(QWidget *parent = 0);
    Coordinate *getCoordinate() const {return coordinate;}
    Address *getAddress() const {return address;}
    void setCoordinate(Coordinate *newCoordinate);
    void setCoordinate(double newAltitude, double newLatitude, double newLongitude);
    void setAddress(Address *newAddress);
    int getIndexOfCurrentObject() const {return index;}
    StatusWidget *instance();
    bool isThereSomeone() const {return thereAreSomeone;}
    QString getNameOfCurrentObject() const {return name;}

private:
    int index; // this is the index of current marker that are in status widget
    bool thereAreSomeone; //this is for know if status widget show information about something
    QString name; // the name of current object that we show information

    QHBoxLayout *idLayout;
    QLabel *idLabel;
    QLineEdit *idLineEdit;
    QCheckBox *setCheckBox;

    QGroupBox *coordinateBox;
    Coordinate *coordinate;
    QLabel *latitudeTitle;
    QLabel *longitudeTitle;
    QLabel *latitude;
    QLabel *longitude;

    QGroupBox *addressBox;
    Address *address;
    QLabel *streetTitle;
    QLabel *districtTitle;
    QLabel *cityTitle;
    QLabel *countryTitle;
    QLabel *stateTitle;
    QLabel *countryCodeTitle;
    QLabel *postalCodeTitle;
    QLabel *street;
    QLabel *district;
    QLabel *city;
    QLabel *country;
    QLabel *state;
    QLabel *countryCode;
    QLabel *postalCode;

    void setupStatusName();
    void setupStatusCoordinateBox();
    void setupStatusAddressBox();
    void updateLabel();
    void setIndex(int newIndex);
    void setThereAreSomeone(const bool newThereAreSomeone);

public slots:
    void update();
    void setName();
    void setName(QString);
    void checkRemoved(int); //this check if the marker that remove is show information here
    /// marker
    void lookForMarker(int);
    void thankForMarker(Marker *);
    void updateInfo(int, Marker *); //this is when the marker changed him information and are in status widget
    /// bus
    void lookForBus(int);
    void thankForBus(Bus *);
    void updateInfo(int, Bus*); //this is when the marker changed him information and are in status widget

signals:
    void requestMarker(int);
    void requestBus(int);
    void thereAreSomeoneChanged(bool);
    void statusNameChanged(int, QString);
    void setFollowMe(int, bool); //this signal is for the core

private slots:
    void onAndOff(bool); // this disable all widget of status
    void setSet(bool);

};

#endif
