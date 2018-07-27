#ifndef CHECKNETWORK_H
#define CHECKNETWORK_H

#include <QThread>

class QTimer;
class CheckNetwork : public QThread //// this is for verified if you are online
{
    Q_OBJECT
public:
    explicit CheckNetwork(QObject *parent = nullptr);
    ~CheckNetwork();
    void stop();
    bool isOnline() {return online;}

private:
    bool online;
    void setOnline(bool);

    volatile bool stopped;

protected:
    void run();

signals:
    void onlineChanged(bool);

};

#endif // CHECKNETWORK_H
