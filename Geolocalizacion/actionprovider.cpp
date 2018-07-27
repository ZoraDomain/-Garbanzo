#include "actionprovider.h"

ActionProvider::ActionProvider(QAction *parent) : QAction(parent) {
    QObject::connect(this, SIGNAL(triggered()), this, SLOT(onTriggered()));
}

ActionProvider::ActionProvider(QString text, QAction *parent) : nameObject(text), QAction(parent) {
    setText(text);
    QObject::connect(this, SIGNAL(triggered()), this, SLOT(onTriggered()));
}

void ActionProvider::setNameObject(const QString newName) {
    if (nameObject != newName)
        nameObject = newName;
    else return;
}

void ActionProvider::onTriggered() {
    emit triggered_2_0(getNameObject());
}
