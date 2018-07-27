#ifndef SOURCEWIDGET_H
#define SOURCEWIDGET_H

#include <QWidget>
#include "corps.h"

class QGroupBox;
class QPushButton;
class QPlainTextEdit;
class Marker;
class Bus;
class SourceWidget : public QWidget {
    Q_OBJECT
public:
    explicit SourceWidget(QWidget *parent = nullptr);
    struct Corps getCurrentCorps() const {return corps;}

    bool getThereAre() const {return thereAre;}

private:
    bool thereAre; // this say  if there are a object
    struct Corps corps; // this save the object to show information
    QGroupBox *nmeaLog;
    QPushButton *addSourceButton;
    QPushButton *removeSourceButton;
    QPlainTextEdit *nmeaLogText;

    void setupNmeaLog();
    QString whatWeHave() const; //this return the class name of the object tha we keep

signals:
    void currentObjectChanged();
    void showInfomationMessageBox(QString title, QString Text);
    void requestFileName(); //this is for resquest filename in mainwindow

public slots:
    void setCurrentCorps(Marker *);
    void setCurrentCorps(Bus *);
    void updateNmeaLogText(int, double, double);
    void checkRemoved(int); // this check if the oject exist for clear the text edit
    void setThereAre(bool);
    void addSource(QString);
    void removeSource();
    void updateName(int, QString);

private slots:
    void addSourceButtonSlot();
    void removeSourceButtonSlot();
    //void lookForAddSource(); //this check what the object



    void clearAll();
};

#endif // SOURCEWIDGET_H
