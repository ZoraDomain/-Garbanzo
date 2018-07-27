#include "mainwindow.h"
#include "markerwidget.h"
#include "buswidget.h"
#include "statuswidget.h"
#include "container.h"
#include "core.h"
#include "logfilepositionsource.h"
#include "positioninfo.h"
#include "coordinate.h"
#include "address.h"
#include "actionprovider.h"
#include "sourcewidget.h"

#include <QtQuick>
#include <QQuickItem>
#include <QQuickView>
#include <QtWidgets>
#include <QtCore/QTextStream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    core = new Core();

    _centralWidget = new QWidget(this);
    vLayout = new QVBoxLayout(_centralWidget);
    vLayout->setSpacing(6);
    vLayout->setContentsMargins(0, 0, 11, 11);

    view = new QQuickView();
    view->setSource(QUrl(QStringLiteral("qrc:/main.qml")));
    container = new Container();
    container->setBody(QWidget::createWindowContainer(view, container->instance()));
    container->setMinimumSize(QSize(500, 500));
    vLayout->addWidget(container);

    markerWidget = new MarkerWidget;
    markerDock = new QDockWidget(tr("Marker"));
    markerDock->setWidget(markerWidget);
    markerDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    markerDock->setFeatures(QDockWidget::DockWidgetClosable & QDockWidget::DockWidgetMovable);
    markerDock->setAllowedAreas(Qt::RightDockWidgetArea);

    busWidget = new BusWidget;
    busDock = new QDockWidget(tr("Bus"));
    busDock->setWidget(busWidget);
    busDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    busDock->setFeatures(QDockWidget::DockWidgetClosable & QDockWidget::DockWidgetMovable);
    busDock->setAllowedAreas(Qt::RightDockWidgetArea);

    statusWidget = new StatusWidget;
    markerStatusDock = new QDockWidget(tr("Status"));
    markerStatusDock->setWidget(statusWidget);
    markerStatusDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    markerStatusDock->setFeatures(QDockWidget::DockWidgetClosable);
    markerStatusDock->setAllowedAreas(Qt::RightDockWidgetArea);

    sourceWidget = new SourceWidget;
    markerSourceDock = new QDockWidget(tr("Source"));
    markerSourceDock->setWidget(sourceWidget);
    markerSourceDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    markerSourceDock->setFeatures(QDockWidget::DockWidgetClosable);
    markerSourceDock->setAllowedAreas(Qt::RightDockWidgetArea);

    //markerStatusWidget = new MarkerStatusWidget;
    markerAppearanceDock = new QDockWidget(tr("Appearance"));
    //markerStatusDock->setWidget(markerStatusWidget);
    markerAppearanceDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    markerAppearanceDock->setFeatures(QDockWidget::DockWidgetClosable);
    markerAppearanceDock->setAllowedAreas(Qt::RightDockWidgetArea);


    this->addDockWidget(Qt::RightDockWidgetArea, markerDock);
    this->addDockWidget(Qt::RightDockWidgetArea, busDock);
    this->addDockWidget(Qt::RightDockWidgetArea, markerStatusDock);
    this->addDockWidget(Qt::RightDockWidgetArea, markerSourceDock);
    this->tabifyDockWidget(busDock, markerDock);
    this->tabifyDockWidget(markerStatusDock, markerSourceDock);
    this->tabifyDockWidget(markerSourceDock, markerAppearanceDock);
    this->setTabPosition(Qt::AllDockWidgetAreas, QTabWidget::North);    
    this->setCentralWidget(_centralWidget);

    // here take the root of qquick view like qobject
    root = view->rootObject();
    Q_ASSERT(root);
    root->setProperty("width", container->width());
    root->setProperty("height", container->height());

    // It is organized by the received signal
    //bus Widget & marker Widget
    QObject::connect(core->instance(), SIGNAL(newAddedMarker(Marker*)), markerWidget, SLOT(addMarker(Marker*)));
    QObject::connect(core->instance(), SIGNAL(newAddedBus(Bus*)), busWidget, SLOT(addBus(Bus*)));
    QObject::connect(root, SIGNAL(currentMarkerChange(int)), markerWidget, SIGNAL(clicked(int)));
    QObject::connect(root, SIGNAL(currentBusChange(int)), busWidget, SIGNAL(clicked(int)));
    // root (qml Document)
    QObject::connect(core->instance(), SIGNAL(setFollowMeInTheRoot(int, bool)), root, SIGNAL(setBusFollowMe(int, bool)));
    QObject::connect(core->instance(), SIGNAL(newMarkerPosition(int,double,double)),
                     root, SIGNAL(newMarkerPosition(int,double,double)));
    QObject::connect(core->instance(), SIGNAL(newBusPosition(int,double,double)),
                     root, SIGNAL(newBusPosition(int,double,double)));
    // source Widget
    QObject::connect(core->instance(), SIGNAL(newPosition(int,double,double)),
                     sourceWidget, SLOT(updateNmeaLogText(int,double,double)));
    QObject::connect(core->instance(), SIGNAL(grandedRequestMarker(Marker*)), sourceWidget, SLOT(setCurrentCorps(Marker*)));
    QObject::connect(core->instance(), SIGNAL(markerRemoved(int)), sourceWidget, SLOT(checkRemoved(int))); ///new
    QObject::connect(statusWidget, SIGNAL(statusNameChanged(int, QString)), sourceWidget, SLOT(updateName(int,QString)));
    QObject::connect(statusWidget, SIGNAL(thereAreSomeoneChanged(bool)), sourceWidget, SLOT(setThereAre(bool)));
    QObject::connect(this, SIGNAL(giveFileName(QString)), sourceWidget, SLOT(addSource(QString)));
    QObject::connect(core->instance(), SIGNAL(grandedRequestBus(Bus*)), sourceWidget, SLOT(setCurrentCorps(Bus*)));
    QObject::connect(core->instance(), SIGNAL(busRemoved(int)), sourceWidget, SLOT(checkRemoved(int))); ///new
    // status Widget
    QObject::connect(core->instance(), SIGNAL(grandedRequestMarker(Marker*)), statusWidget, SLOT(thankForMarker(Marker *)));
    QObject::connect(core->instance(), SIGNAL(grandedRequestBus(Bus*)), statusWidget, SLOT(thankForBus(Bus*)));
    QObject::connect(core->instance(), SIGNAL(markerRemoved(int)), statusWidget, SLOT(checkRemoved(int))); ///new
    QObject::connect(core->instance(), SIGNAL(busRemoved(int)), statusWidget, SLOT(checkRemoved(int)));
    QObject::connect(markerWidget, SIGNAL(clicked(int)), statusWidget, SLOT(lookForMarker(int)));
    QObject::connect(busWidget, SIGNAL(clicked(int)), statusWidget, SLOT(lookForBus(int)));
    QObject::connect(core->instance(), SIGNAL(changedInfoComplete(int, Marker*)),
                     statusWidget, SLOT(updateInfo(int,Marker*)));
    QObject::connect(core->instance(), SIGNAL(changedInfoComplete(int, Bus*)),
                     statusWidget, SLOT(updateInfo(int,Bus*)));
    // mainWindow
    QObject::connect(core->instance(), SIGNAL(indexError(int)), this, SLOT(showIndexError(int)));
    QObject::connect(core->instance(), SIGNAL(receivedMarker(int)), this, SLOT(giveMarkerIndex(int)));
    QObject::connect(core->instance(), SIGNAL(receivedBus(int)), this, SLOT(giveBusIndex(int)));
    QObject::connect(container, SIGNAL(resizeMap(int,int)), this, SLOT(updateMapSize(int, int)));
    QObject::connect(core->instance(), SIGNAL(onlineChanged(bool)), this, SLOT(updateNetworkStatusBar(bool)));
    QObject::connect(core->instance(), SIGNAL(showWarningMessageBox(QString,QString)), this, SLOT(printWarningMessage(QString,QString)));
    QObject::connect(root, SIGNAL(updateInfo(QString, QString)), this, SLOT(updateStatusBar(QString, QString)));
    QObject::connect(root, SIGNAL(addProviderToMenu(QString)), this, SLOT (addProvider(QString)));
    QObject::connect(sourceWidget, SIGNAL(requestFileName()), this, SLOT(requestFileName()));
    QObject::connect(sourceWidget, SIGNAL(showInfomationMessageBox(QString,QString)), this, SLOT(printInformationMessage(QString,QString)));

    // core(can be the qml document to core)
    QObject::connect(statusWidget, SIGNAL(setFollowMe(int, bool)), core->instance(), SLOT(setFollowMe(int, bool)));
    QObject::connect(root, SIGNAL(stopMarkerPositionPlanner()), core->instance(), SLOT(stopMarkerPositionPlanner()));
    QObject::connect(root, SIGNAL(stopBusPositionPlanner()), core->instance(), SLOT(stopBusPositionPlanner()));
    QObject::connect(statusWidget, SIGNAL(requestMarker(int)), core->instance(), SLOT(lookForMarker(int)));
    QObject::connect(statusWidget, SIGNAL(requestBus(int)), core->instance(), SLOT(lookForBus(int)));
    QObject::connect(statusWidget, SIGNAL(statusNameChanged(int, QString)), core->instance(), SLOT(changeName(int,QString))); //new
    QObject::connect(root, SIGNAL(eraseMarker(int)), core->instance(), SLOT(removeMarker(int)));
    QObject::connect(root, SIGNAL(eraseBus(int)), core->instance(), SLOT(removeBus(int)));
    QObject::connect(root, SIGNAL(newMarkerData(QString, double, double,
                                                QString, QString, QString, QString,
                                                QString, QString, QString, QString)),
                     core->instance(), SLOT(addMarker(QString, double, double,
                                                              QString, QString, QString, QString,
                                                              QString, QString, QString, QString)));
    QObject::connect(root, SIGNAL(markerChanged(int, double, double,
                                                QString, QString, QString, QString,
                                                QString, QString, QString, QString)),
                     core->instance(), SLOT(updateMarker(int, double, double,
                                                              QString, QString, QString, QString,
                                                              QString, QString, QString, QString)));
    QObject::connect(root, SIGNAL(newBusData(QString, double, double,
                                                QString, QString, QString, QString,
                                                QString, QString, QString, QString)),
                     core->instance(), SLOT(addBus(QString, double, double,
                                                              QString, QString, QString, QString,
                                                              QString, QString, QString, QString)));
    QObject::connect(root, SIGNAL(busChanged(int, double, double,
                                                QString, QString, QString, QString,
                                                QString, QString, QString, QString)),
                     core->instance(), SLOT(updateBus(int, double, double,
                                                              QString, QString, QString, QString,
                                                              QString, QString, QString, QString)));

    ///bus connection
   // QObject::connect(busWidget, SIGNAL(clicked(int)), core->instance(), SLOT(testCoordinate(int)));

    //QObject::connect(markerWidget, SIGNAL(clicked(int)), markerStatusWidget, SLOT(lookForMarker(int)));
    //QObject::connect(markerStatusWidget, SIGNAL(requestMarker(int)), core->instance(), SLOT(lookForMarker(int)));
    //QObject::connect(core->instance(), SIGNAL(grandedRequestBus(Bus*)), markerStatusWidget, SLOT(thankForMarker(Marker *)));
    //QObject::connect(core->instance(), SIGNAL(indexError(int)), this, SLOT(showIndexError(int)));    

    //QObject::connect(core->instance(), SIGNAL(ChangedInfoComplete(int, Bus*)),
    //                 markerStatusWidget, SLOT(updateInfo(int,Bus*)));


    readSetting();
    createMenu();
    setupStatusBar();
}

void MainWindow::setupStatusBar() {
    coordinateAltitude = new QLabel("0.00");
    coordinateAltitude->setAlignment(Qt::AlignHCenter);

    coordinateLongitude = new QLabel("0.00");
    coordinateLongitude->setIndent(10);

    status = new QLabel("Status: Offline");
    this->statusBar()->addWidget(status, 5);
    this->statusBar()->addWidget(coordinateAltitude);
    this->statusBar()->addWidget(coordinateLongitude);
}

void MainWindow::updateStatusBar(QString altitudeString, QString longitudeString) {
    coordinateAltitude->setText(QString("Lat: " + QString::number(altitudeString.toFloat())));
    coordinateLongitude->setText(QString("Long: " + QString::number(longitudeString.toFloat())));
}

void MainWindow::updateNetworkStatusBar(bool newStatus) {
    if (newStatus) {
        status->setText("Status: Online");
    } else {
        status->setText("Status: Offline");
        printWarningMessage(tr("You are offline"),
                            tr("Without an internet connection the application can not work well"));
    }
}

void MainWindow::initProvider(QVariantMap &parameters) {
    QMetaObject::invokeMethod(root, "initializeProviders",
                              Q_ARG(QVariant, QVariant::fromValue(parameters)));
}

void MainWindow::createMenu() {
    newAction = new QAction(tr("&New"));
    newAction->setShortcut(QKeySequence::New);
    openAction = new QAction(tr("&Open"));
    openAction->setShortcut(QKeySequence::Open);
    saveAction = new QAction(tr("&Save"));
    saveAction->setShortcut(QKeySequence::Save);
    saveAsAction = new QAction(tr("Save as"));
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    exitAction = new QAction(tr("Exit"));
    fileMenu = this->menuBar()->addMenu(tr("File"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);
    ////
    ProviderMenu = new QMenu("Provider");
    mapMenu = this->menuBar()->addMenu("Map");
    mapMenu->addSeparator();
    mapMenu->addMenu(ProviderMenu);
    ////
    testLogPositionSource = new QAction(tr("Test Log Position Source System"));
    testMarkerPositionSignalPlanner = new QAction(tr("Test Marker Position Signal Planner System"));
    testBusPositionSignalPlanner = new QAction(tr("Test Bus Position Signal Planner System"));
    developerMenu = new QMenu(tr("Developer Option"));
    developerMenu->addAction(testLogPositionSource);
    developerMenu->addAction(testMarkerPositionSignalPlanner);
    developerMenu->addAction(testBusPositionSignalPlanner);
    aboutMenu = this->menuBar()->addMenu(tr("About"));
    aboutMenu->addMenu(developerMenu);
    QObject::connect(testLogPositionSource, SIGNAL(triggered()), this, SLOT(testLogPositionSourceSystem()));
    QObject::connect(testMarkerPositionSignalPlanner, SIGNAL(triggered()), core->instance(), SLOT(startMarkerPositionSignalPlanner()));
    QObject::connect(testBusPositionSignalPlanner, SIGNAL(triggered()), core->instance(), SLOT(startBusPositionSignalPlanner()));

}

bool MainWindow::parseArgs(QStringList &args, QVariantMap &parameters) {

    while (!args.isEmpty()) {

        QString param = args.takeFirst();

        if (param.startsWith("--help")) {
            QTextStream out(stdout);
            out << "Usage: " << endl;
            out << "--plugin.<parameter_name> <parameter_value>    -  Sets parameter = value for plugin" << endl;
            out.flush();
            return true;
        }

        if (param.startsWith("--plugin.")) {

            param.remove(0, 9);

            if (args.isEmpty() || args.first().startsWith("--")) {
                parameters[param] = true;
            } else {

                QString value = args.takeFirst();

                if (value == "true" || value == "on" || value == "enabled") {
                    parameters[param] = true;
                } else if (value == "false" || value == "off"
                           || value == "disable") {
                    parameters[param] = false;
                } else {
                    parameters[param] = value;
                }
            }
        }
    }
    return false;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    writeSetting();
    //delete core;
}

void MainWindow::writeSetting() {
    QSettings settings("Marker Fondation", "MarkerApp");
    settings.beginGroup("mainWindow");
    //coming soom
    settings.endGroup();
    settings.beginGroup("dockWidget");
    settings.setValue("markerDock_Geometry", markerDock->geometry());
    settings.setValue("busDock_Geometry", busDock->geometry());
    settings.setValue("markerStatusDock_Geometry", markerStatusDock->geometry());
    settings.endGroup();
}

void MainWindow::readSetting() {
    QSettings settings("Marker Fondation", "MarkerApp");
    settings.beginGroup("mainWindow");
    //coming soom
    settings.endGroup();
    settings.beginGroup("dockWidget");
    QRect MarkerDockRect = settings.value("markerDock_Geometry", QRect(markerDock->geometry())).toRect();
    QRect MarkerStatusDockRect = settings.value("markerStatusDock_Geometry", QRect(markerStatusDock->geometry())).toRect();
    QRect busDockRect = settings.value("busDock_Geometry", QRect(busDock->geometry())).toRect();

    markerDock->resize(MarkerDockRect.size());
    markerStatusDock->resize(MarkerStatusDockRect.size());
    busDock->resize(busDockRect.size());
}

void MainWindow::updateMapSize(int w, int h) {
    root->setProperty("width", w);
    root->setProperty("height", h);
}

void MainWindow::giveMarkerIndex(int index) {
    QMetaObject::invokeMethod(root, "giveIndexMarker",
                              Q_ARG(QVariant, QVariant::fromValue(index)));
}

void MainWindow::giveBusIndex(int index) {
    QMetaObject::invokeMethod(root, "giveIndexBus",
                              Q_ARG(QVariant, QVariant::fromValue(index)));
}

void MainWindow::showIndexError(int index) {
    QMessageBox::warning(this, tr("Marker App"),
                         tr("this index don't have a value /n 0X876TS6%1").arg(index),
                         QMessageBox::Ok);
}

void MainWindow::testLogPositionSourceSystem() {
    testSystem = new LogFilePositionSource(this);
    testSystem->startUpdate();
    QObject::connect(testSystem, SIGNAL(newUpdate(PositionInfo*)), this, SLOT(printMessage(PositionInfo*)));
}

void MainWindow::printMessage(PositionInfo *info) {
    qDebug("TimeStamp: %s, Latitude: %f, Longitude: %f",
           info->getTimeStamp()->time().toString(), info->getCoordinate()->getLatitude(), info->getCoordinate()->getLongitude());
}

void MainWindow::printInformationMessage(QString title, QString text) {
    QMessageBox::information(this, title, text,
                             QMessageBox::Close);
}

void MainWindow::printWarningMessage(QString title, QString text) {
    QMessageBox::warning(this, title, text,
                         QMessageBox::Ok);
}

void MainWindow::addProvider(QString name) {
    ActionProvider *newProvider = new ActionProvider(name);
    QObject::connect(newProvider, SIGNAL(triggered_2_0(QString)), root, SIGNAL(changeMapProvider(QString)));
    ProviderMenu->addAction(newProvider);

}

void MainWindow::requestFileName() {
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
        emit giveFileName(fileName);
}
