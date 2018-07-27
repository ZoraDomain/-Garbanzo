#ifndef INTERFACE_H
#define INTERFACE_H

#include <QQuickItem>

class Interface : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString basicText READ basicText WRITE setBasicText)

public:
    Interface(QQuickItem *parent = 0);

    QString basicText() const {return m_basicText;}
    void setBasicText(const QString &newBasicText);

    Q_INVOKABLE void showInConsole(const QString &text) const;
private:
    QString m_basicText;

signals:

public slots:
};

#endif // INTERFACE_H
