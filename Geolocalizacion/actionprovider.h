#ifndef ACTIONPROVIDER_H
#define ACTIONPROVIDER_H

#include <QAction>

class ActionProvider : public QAction
{
    Q_OBJECT
public:
    explicit ActionProvider(QAction *parent = nullptr);
    ActionProvider(QString text, QAction *parent= nullptr);
    void setNameObject(QString const newName);
    QString getNameObject() const {return nameObject;}
private:
        QString nameObject;
signals:
    void triggered_2_0(QString);

private slots:
    void onTriggered();
};

#endif // ACTIONPROVIDER_H
