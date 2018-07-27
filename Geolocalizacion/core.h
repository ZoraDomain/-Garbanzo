#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QHash>

class Marker;
class Bus;
class Address;
class QTimer;
class MarkerPositionSignalPlanner;
class BusPositionSignalPlanner;
class CheckNetwork;
class Core : public QObject {
    Q_OBJECT
public:
    explicit Core(QObject *parent = nullptr);
    ~Core();
    Core *instance();

    bool isOnline() const {return online;}
private:
    QString ofWhoIsIndex(int); // this look for the index in the marker and bus array and return the  class name (Bus|Marker)
    ///// marker
    QHash<int, Marker*> markerHash;
    const int availableMarker = 25;
    int numOfMarker;
    int IndexOfMarker[25];

    Marker *getMarker(int index) const {return markerHash.value(index); }
    bool indexisValidForMarker(int); ///ever call this function after call get Marker

    MarkerPositionSignalPlanner *markerPositionSignalPlanner;
    void checkForMarkerHaveNewdata(int index); ///this is for off the have new data in logFile

    void changedMarkerName(int, QString); ///changed the name

    ///// bus
    QHash<int, Bus*> busHash;
    const int availableBus = 60;
    int numOfBus;
    int IndexOfBus[60];

    Bus *getBus(int index) const {return busHash.value(index); }
    bool indexisValidForBus(int); ///ever call this function after call get bus

    BusPositionSignalPlanner *busPositionSignalPlanner;
    void checkForBusHaveNewdata(int index); ///this is for off the have new data in logFile

    void changedBusName(int, QString); ///changed the name

    void markButton(int); /// this fill the button when object  are sent; is to know that are select
    void removeButtonMark(); /// this check all object to remove mark

    CheckNetwork *checkNetwork; ///this a thread that is used to know if you are online
    bool online;

signals:
    ////// marker
    void newAddedMarker(Marker *); //this emit when a new marker is added
    void grandedRequestMarker(Marker *); //this use for return the object to all widget
    void markerRemoved(int);
    void indexError(int);
    void receivedMarker(int); //this give the index number for edit the marker in qml
    void changedInfoComplete(int, Marker *);
    void newMarkerPosition(int, double, double); //this connect send the new position to qml  

    ////// bus
    void newAddedBus(Bus *); //this emit when a new bus is added
    void grandedRequestBus(Bus *); //this use for return the object to all widget
    //void indexError(int);
    void busRemoved(int);
    void receivedBus(int); //this give the index number for edit the bus in qml
    void changedInfoComplete(int, Bus *); //is called for update bus and emit when all changed have been success
    void newBusPosition(int, double, double); //this connect send the new position to qml

    void newPosition(int, double, double); // this signal is emit when marker emit a new position and bus emit newPosition

    //////this is for check network
    void onlineChanged(bool);

    //////this is for show a message box
    void showWarningMessageBox(QString title, QString Text);

    void setFollowMeInTheRoot(int, bool); //this send the signal to root(QML document)


public slots:
    void testCoordinate(int index);
    void changeName(int, QString); // this look for the object that want changed name

    ////// marker
    void lookForMarker(int);
    void removeMarker(int);
    void addMarker(QString text, double latitude, double longitude,
                   QString street, QString district, QString city, QString county,
                   QString state, QString countryCode, QString country, QString postalCode);
    void updateMarker(int index, double latitude, double longitude,
                   QString street, QString district, QString city, QString county,
                   QString state, QString countryCode, QString country, QString postalCode);    
    void startMarkerPositionSignalPlanner();
    void stopMarkerPositionPlanner(); // this only stop the signal posiiton planner

    ////// bus
    void lookForBus(int);
    void removeBus(int);
    void addBus(QString text, double latitude, double longitude,
                   QString street, QString district, QString city, QString county,
                   QString state, QString countryCode, QString country, QString postalCode);
    void updateBus(int index, double latitude, double longitude,
                   QString street, QString district, QString city, QString county,
                   QString state, QString countryCode, QString country, QString postalCode);
    void startBusPositionSignalPlanner();
    void stopBusPositionPlanner(); // this only stop the signal posiiton planner

    void setFollowMe(int, bool); // this is for set in the object

private slots:
    void setOnline(bool);
    void updateFollowMeForBus(int, bool); // this reset all follow me and send the signal for root(QML)

    //this stop the signal position planner and send the signal for make reverse geocoding
    void stopMarkerPositionPlanner(int, double, double);
    void stopBusPositionPlanner(int, double, double);

};

#endif // CORE_H
