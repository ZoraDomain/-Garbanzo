#include "logfilepositionsource.h"
#include "positioninfo.h"

#include <QDebug>
#include <QFile>
#include <QTimer>
#include <QMessageBox>
#include <QDateTime>
#include <QDir>

LogFilePositionSource::LogFilePositionSource(QObject *parent) : QObject(parent), info(new PositionInfo),
    haveNewData(0), running(0) {
    logFile = new QFile(this);
    logFile->setFileName(path);

    timer = new QTimer(this);
    timer->setInterval(1000);

    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(readNextLine()));
    QObject::connect(this, SIGNAL(pathChanged(QString)), this, SLOT(updateLogFileName(QString)));
}

void LogFilePositionSource::startUpdate() {
    if (!logFile->open(QIODevice::ReadOnly))
        qDebug() << "Cannot open the file" + logFile->fileName(), + ": " + logFile->errorString();
    timer->start();
    running = true;
}

void LogFilePositionSource::stopUpdate() {
    timer->stop();
    running = false;
    haveNewData = false;
}

void LogFilePositionSource::finish() {
    stopUpdate();
    logFile->close();
    path = "";
}

void LogFilePositionSource::setinterval(int msec) {
    timer->setInterval(msec);
}

void LogFilePositionSource::setHaveNewData(const bool newHaveData) {
    if (haveNewData != newHaveData) {
        haveNewData = newHaveData;
        if (haveNewData == false)
            emit alreadyRead();
    }
    return;
}

void LogFilePositionSource::readNextLine() {
    QByteArray line = logFile->readLine().trimmed();
    if (!line.isEmpty()) {
        QList<QByteArray> data = line.split(' ');
        QDateTime timeStamp(QDateTime::fromString(data.value(0)));
        bool hasLatitude = false;
        bool hasLongitude = false;
        double latitude = data.value(1).toDouble(&hasLatitude);
        double longitude = data.value(2).toDouble(&hasLongitude);

        if(hasLatitude == true && hasLongitude == true) {
            info->setCoordinate(latitude, longitude);
            info->setTimeStamp(timeStamp);
            //emit newUpdate(info);
            setHaveNewData(true);
        }

    }
}

void LogFilePositionSource::setPath(const QString newPath) {
    if (path != newPath) {
        path = newPath;
        emit pathChanged(path);
    }
    else return;;
}

void LogFilePositionSource::updateLogFileName(QString newPath) {
    stopUpdate();
    logFile->close();
    logFile->setFileName(newPath);
    startUpdate();
}
