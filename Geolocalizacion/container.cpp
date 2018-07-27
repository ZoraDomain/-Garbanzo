#include "container.h"
#include <QtWidgets>

Container::Container(QWidget *parent) : QWidget(parent) {
    vLayout = new QVBoxLayout(this);
}

/*
Container::Container(QWidget *_body, QWidget *parent) :QWidget(parent) {
    body = _body;
    vLayout = new QVBoxLayout(this);
    vLayout->addWidget(body);
}
*/

void Container::setBody(QWidget *_body) {
    if (_body != body) {
        body = _body;
        //vLayout->addWidget(body);
    }
    else
        return;
}

void Container::resizeEvent(QResizeEvent *event) {
    emit resizeMap(event->size().width(),  event->size().height());
    body->resize(event->size().width(),  event->size().height());
    saveGeometry();
    body->saveGeometry();

    //this say the size of the widge3t
    /*
    qDebug("event size: %d, %d", event->size().width(), event->size().height());
    qDebug("widget size: %d, %d", this->width(), this->height());
    qDebug("vLayout size: %d, %d", vLayout->geometry().width(), vLayout->geometry().height());
    qDebug("body size: %d, %d", body->width(), body->height());
    qDebug("parent Widget: %d, %d", this->parentWidget()->width(), this->parentWidget()->height());
    */

}

Container *Container::instance() {
    return this;
}
