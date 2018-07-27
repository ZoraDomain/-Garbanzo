#ifndef LOGFILEPOSITIONSOURCE_H
#define LOGFILEPOSITIONSOURCE_H

#include <QObject>

class QFile;
class QTimer;
class PositionInfo;
class QDir;
class LogFilePositionSource : public QObject {
    Q_OBJECT
public:
    explicit LogFilePositionSource(QObject *parent = nullptr);
    void startUpdate();
    void stopUpdate();
    void setinterval(int);

    PositionInfo *getPositionInfo() {return info;}  
    QString getPath() const {return path;}
    void setPath(const QString newPath);
    void setHaveNewData(const bool newHaveData);
    bool getHaveNewData() const {return haveNewData;}
    bool isRunning() const {return running;}
    void finish();

private:
    QFile *logFile;
    QTimer *timer;
    PositionInfo *info;

    QString path;

    bool haveNewData;
    bool running;

signals:
    void newUpdate(PositionInfo *);
    void pathChanged(QString);
    void alreadyRead();

private slots:
    void readNextLine();
    void updateLogFileName(QString);
};

#endif // LOGFILEPOSITIONSOURCE_H
