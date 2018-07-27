#ifndef BUSPOSITIONSIGNALPLANNER_H
#define BUSPOSITIONSIGNALPLANNER_H

#include <QObject>
#include <QThread>
#include <taken.h>

class Bus;
class QTimer;
class BusPositionSignalPlanner : public QThread
{
    Q_OBJECT
public:
    BusPositionSignalPlanner();
    ~BusPositionSignalPlanner();
    void stop();
    bool getStopped() const {return stopped;}
    void setObjectToPlanner(QHash<int, Bus*> &, int *);
    void setCount(const int newCount);
    int getCount() const {return (count) - 1;} // this return the number in format  where it start to count from zero
                                                   // only for handle the index
    void getOut(); // this is for terminate thread

protected:
    void run();

signals:
    void start_();
    void newPosition(int, double, double);
    void countChanged();

private:
    QTimer *timer;
    QHash<int, Bus*> *busHash;
    int count; // WARNING this is the number of bus that we need to planner signal,
               // if this number is incorrent can happpen bad thing
    int *indexArray;
    struct Taken taken[100]; // that save the index of object,
                             // if he want that we read the log file,
                             // and if we already read you data in the circuit
    bool volatile stopped;
    bool out;
    void restartTaken();
    void updateTakenListIndex();
    bool isChecked(int);
    void setChecked(int, int);
    bool allhaveBeenRead();

public slots:
    void resumen();

private slots:
    void checkNumOfObjectToPlanner(); // this check if there are object to planner
};


#endif // BUSPOSITIONSIGNALPLANNER_H
