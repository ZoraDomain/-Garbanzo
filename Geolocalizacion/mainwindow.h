#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MarkerWidget;
class BusWidget;
class StatusWidget;
class InfoWidget;
class QVBoxLayout;
class QQuickView;
class Container;
class QLabel;
class QHBoxLayout;
class Core;
class LogFilePositionSource;
class PositionInfo;
class SourceWidget;
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    static bool parseArgs(QStringList &args, QVariantMap &parameters);
    void initProvider(QVariantMap &parameters);

private:
    QQuickView *view; //this is qml interface

    //widget that contain the root qml object (this widget contain the container for resize with the main window)
    Container *container;

    QObject *root; //this object is for handle the view

    QWidget *_centralWidget;
    QVBoxLayout *vLayout;

    BusWidget *busWidget;    
    MarkerWidget *markerWidget;

    StatusWidget *statusWidget;
    SourceWidget *sourceWidget;

    Core *core; //this is that save all information of map and give out information to all widget

    QDockWidget *markerDock;
    QDockWidget *busDock;
    QDockWidget *markerStatusDock;
    QDockWidget *markerSourceDock;
    QDockWidget *markerAppearanceDock;

    QMenu *fileMenu;
    QAction *newAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *exitAction;
    QMenu *mapMenu;
    QMenu *ProviderMenu;
    QMenu *viewMenu;
    QAction *controlAction;
    QAction *markerAction;
    QMenu *aboutMenu;
    QMenu *developerMenu;
    QAction *testLogPositionSource;
    QAction *testMarkerPositionSignalPlanner; //this is for test the position marker
    QAction *testBusPositionSignalPlanner;
    // for statusBar
    QLabel *coordinateAltitude;
    QLabel *coordinateLongitude;
    QLabel *status;

    void createMenu();
    void setupStatusBar();
    void writeSetting();
    void readSetting();

    //dev mode
    LogFilePositionSource *testSystem;

protected:
    virtual void closeEvent(QCloseEvent *event);

signals:
    void hideDock();
    void showDock();
    void giveFileName(QString); // this give the file name to source widget

public slots:
    void showIndexError(int);
    void addProvider(QString);
    void requestFileName(); //this is for use qFileDialog

private slots:
    void updateMapSize(int, int);
    void updateStatusBar(QString, QString);
    void updateNetworkStatusBar(bool);

    // this give the index of object in the array in qml
    void giveMarkerIndex(int);
    void giveBusIndex(int);

    //dev mode
    void testLogPositionSourceSystem();
    void printWarningMessage(QString, QString);
    void printInformationMessage(QString, QString);
    void printMessage(PositionInfo *);

};

#endif // MAINWINDOW_H
