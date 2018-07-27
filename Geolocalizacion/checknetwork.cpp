#include "checknetwork.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>

CheckNetwork::CheckNetwork(QObject *parent) : QThread(parent) {
    stopped = false;
    online = false;
}

CheckNetwork::~CheckNetwork() {
    stop();
}

void CheckNetwork::setOnline(bool newOnline) {
    if (online != newOnline) {
        online = newOnline;
        emit onlineChanged(online);
    } else
        return;
}

void CheckNetwork::run() {
    while (stopped == false) {
        msleep(1000);
        QNetworkAccessManager nam;
        QNetworkRequest req(QUrl("http://google.com"));
        QNetworkReply *reply = nam.get(req);
        QEventLoop loop;
        QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();
        if(reply->bytesAvailable()) {
            setOnline(true);
        } else {
            setOnline(false);
        }
    }
}

void CheckNetwork::stop() {
    stopped = true;
    this->wait();
}
