#include "interface.h"

Interface::Interface(QQuickItem *parent) : QQuickItem(parent) {

}

void Interface::setBasicText(const QString &newBasicText) {
    if (m_basicText != newBasicText)
        m_basicText = newBasicText;
    else
        return;
}

void Interface::showInConsole(const QString &text) const {
    qDebug() << "QML:" << text;
}
