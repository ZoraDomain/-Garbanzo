#ifndef MARKERWIDGET_H
#define MARKERWIDGET_H

#include <QWidget>

class QGridLayout;
class Marker;
class QSpacerItem;
class Coordinate;
class Address;
class SignalMapper;
class QScrollArea;
class MarkerWidget : public QWidget {
    Q_OBJECT

public:
    MarkerWidget(QWidget *parent = 0);
    MarkerWidget *instance();

private:
    QScrollArea *markerArea;

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
    void addMarker(Marker *newMarker);

};

#endif // MARKERWIDGET_H
