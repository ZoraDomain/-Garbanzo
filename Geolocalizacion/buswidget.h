#ifndef BUSWIDGET_H
#define BUSWIDGET_H

#include <QWidget>

class QGridLayout;
class Bus;
class QSpacerItem;
class Coordinate;
class Address;
class SignalMapper;
class QScrollArea;
class BusWidget : public QWidget {
    Q_OBJECT

public:
    BusWidget(QWidget *parent = 0);
    BusWidget *instance();

private:
    QScrollArea *busArea;

    QGridLayout *listLayout;
    QWidget *scrollAreaContens;

    static int row;
    QSpacerItem *verticalSpacer;

    SignalMapper *signalMapper;

protected:
    virtual void resizeEvent(QResizeEvent *event);

signals:
    void clicked(int);

public slots:
    void addBus(Bus *newBus);

};

#endif // BUSWIDGET_H
