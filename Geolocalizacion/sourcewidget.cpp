#include "sourcewidget.h"
#include "marker.h"
#include "logfilepositionsource.h"
#include "bus.h"

#include <QPushButton>
#include <QPlainTextEdit>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QDebug>

SourceWidget::SourceWidget(QWidget *parent) : QWidget(parent) {
    setupNmeaLog();

    ///this set up the corps object
    corps.bus = 0;
    corps.marker = 0;

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(nmeaLog);

    thereAre = false;
    QObject::connect(this, SIGNAL(currentObjectChanged()), this, SLOT(clearAll()));
    QObject::connect(addSourceButton, SIGNAL(clicked()), this, SLOT(addSourceButtonSlot()));
    QObject::connect(removeSourceButton, SIGNAL(clicked()), this, SLOT(removeSourceButtonSlot()));
    clearAll();

}

void SourceWidget::setupNmeaLog() {
    nmeaLog = new QGroupBox("Nmea Log", this);
    addSourceButton = new QPushButton("Add Source", this);
    removeSourceButton = new QPushButton("Remove Source", this);;
    nmeaLogText = new QPlainTextEdit(); nmeaLogText->setReadOnly(true);
    nmeaLogText->setFont(QFont("Courier New", 5, 1));

    QHBoxLayout *nmeaLayout = new QHBoxLayout(this);
    QVBoxLayout *buttonLayout = new QVBoxLayout(this);

    buttonLayout->addWidget(addSourceButton);
    buttonLayout->addWidget(removeSourceButton);

    nmeaLayout->addLayout(buttonLayout);
    nmeaLayout->addWidget(nmeaLogText);

    nmeaLog->setLayout(nmeaLayout);
    nmeaLog->setFont(QFont("Courier New", 12, 1));

}

void SourceWidget::setCurrentCorps(Marker *newMarker) {
    if (newMarker->getClassName() == "Marker") {
        if (corps.marker != newMarker) {
            corps.marker = newMarker;
            corps.bus = 0;
            nmeaLog->setTitle("Nmea Log " + getCurrentCorps().marker->text());
            clearAll();
        }
        else return;
    } else
        return;
}

void SourceWidget::setCurrentCorps(Bus *newBus) {
    if (newBus->getClassName() == "Bus") {
        if (corps.bus != newBus) {
            corps.bus = newBus;
            corps.marker = 0;
            nmeaLog->setTitle("Nmea Log " + getCurrentCorps().bus->text());
            clearAll();
        }
        else return;
    } else
        return;
}

void SourceWidget::updateName(int index, QString name) {
    if (whatWeHave() == "Marker") {
        if (getCurrentCorps().marker->getIndex() == index)
            nmeaLog->setTitle("Nmea Log " + name);
        else
            return;
    } else if (whatWeHave() == "Bus") {
        if (getCurrentCorps().bus->getIndex() == index)
            nmeaLog->setTitle("Nmea Log " + name);
        else
            return;
    }
}

void SourceWidget::checkRemoved(int index) {
    emit currentObjectChanged();
}

void SourceWidget::addSource(QString fileName) {
    if(!fileName.isEmpty()) {
        if (whatWeHave() == "Marker")
            getCurrentCorps().marker->getLogFileSource()->setPath(fileName);
        else if (whatWeHave() == "Bus")
            getCurrentCorps().bus->getLogFileSource()->setPath(fileName);
    }
}

QString SourceWidget::whatWeHave() const {
    if (getCurrentCorps().bus == 0)
       return getCurrentCorps().marker->getClassName();
    else if (getCurrentCorps().marker == 0)
        return getCurrentCorps().bus->getClassName();
}

void SourceWidget::addSourceButtonSlot() {
    if (whatWeHave() == "Marker") {
        if (getCurrentCorps().marker->getSet() == 1) {
            emit showInfomationMessageBox(tr("Marker"),
                                          tr("Remeber that the marker is a fixed object /n "
                                             "if you want to make it movable unselect set option in status widget"));
            emit requestFileName();

        } else
            emit requestFileName();
    } else if (whatWeHave() == "Bus") {
        emit requestFileName();
    }
}

void SourceWidget::removeSourceButtonSlot() {
    if (whatWeHave() == "Marker") {
        if (getCurrentCorps().marker->getLogFileSource()->isRunning()) {
            const QString message("Has been remove the log file of the marker: " + getCurrentCorps().marker->text() + "\n" +
                                                     "and path" + getCurrentCorps().marker->getLogFileSource()->getPath());
            emit showInfomationMessageBox(tr("Marker"),
                                          message);
            removeSource();
        }
        return;
    } else if (whatWeHave() == "Bus") {
        if (getCurrentCorps().bus->getLogFileSource()->isRunning()) {
            const QString message("Has been remove the log file of the bus: " + getCurrentCorps().bus->text() + "\n" +
                                                     "and path" + getCurrentCorps().bus->getLogFileSource()->getPath());
            emit showInfomationMessageBox(tr("Bus"),
                                          message);
            removeSource();
        }
    }
}

void SourceWidget::updateNmeaLogText(int index, double latitude, double longitude) {
    //qDebug("in the source widget the marker has the index: %d and the index recive is: %d", getCurrentMarker()->getIndex(),index );

    if (whatWeHave() == "Marker") {
        if (getCurrentCorps().marker->getIndex() == index)
            nmeaLogText->insertPlainText(QString::number(latitude) + " " + QString::number(longitude));
        else return;
    } else if (whatWeHave() == "Bus") {
        if (getCurrentCorps().bus->getIndex() == index)
            nmeaLogText->insertPlainText(QString::number(latitude) + " " + QString::number(longitude));
        else return;
    }
}

void SourceWidget::removeSource() {
    if (whatWeHave() == "Marker") {
        getCurrentCorps().marker->getLogFileSource()->finish();
        return;
    } else if (whatWeHave() == "Bus") {
        getCurrentCorps().bus->getLogFileSource()->finish();
        return;
    }
}
void SourceWidget::setThereAre(bool newThereAre) {
    if (thereAre != newThereAre) {
        thereAre = newThereAre;
        clearAll();
    } else return;
}

void SourceWidget::clearAll() {
    if (thereAre == false) {
        nmeaLog->setTitle("Nmea Log");
        addSourceButton->setDisabled(true);
        removeSourceButton->setDisabled(true);
        nmeaLogText->setDisabled(true);
        nmeaLogText->clear();
    } else {
        addSourceButton->setDisabled(false);
        removeSourceButton->setDisabled(false);
        nmeaLogText->setDisabled(false);
        nmeaLogText->clear();
        nmeaLogText->insertPlainText(tr("<LATITUDE>, <LONGITUDE>"));
    }
}
