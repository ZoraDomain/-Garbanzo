#include <QApplication>
#include <QtQuick/QQuickView>
#include "mainwindow.h"
#include "interface.h"
#include <QtCore/QTextStream>
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQuick/QQuickItem>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    qmlRegisterType<Interface>("InterfaceIO", 1, 0, "Interface");

    MainWindow w;

    QVariantMap parameters;
    QStringList args(QCoreApplication::arguments());

    //tokens from the enviroment
    const QByteArray mapBoxMapId = qgetenv("MAPBOX_MAP_ID");
    const QByteArray mapBoxAccessToken = qgetenv("MAPBOX_ACCESS_TOKEN");
    const QByteArray hereAppId = qgetenv("HERE_APP_ID");
    const QByteArray hereToken = qgetenv("HERE_TOKEN");
    const QByteArray esriToken = qgetenv("ESRI_TOKEN");

    /////////////
    if (!mapBoxMapId.isEmpty())
        parameters["mapbox.map_id"] = QString::fromLocal8Bit(mapBoxMapId);
    if (!mapBoxAccessToken.isEmpty()) {
        parameters["mapbox.access_token"] = QString::fromLocal8Bit(mapBoxAccessToken);
        parameters["mapboxgl.access_token"] = QString::fromLocal8Bit(mapBoxAccessToken);
    }
    if (!hereAppId.isEmpty())
        parameters["here.app_id"] = QString::fromLocal8Bit(hereAppId);
    if (!hereToken.isEmpty())
        parameters["here.token"] = QString::fromLocal8Bit(hereToken);
    if (!esriToken.isEmpty())
        parameters["esri.token"] = QString::fromLocal8Bit(esriToken);

    if(MainWindow::parseArgs(args, parameters))
        return 0;
    if (!args.contains(QStringLiteral("osm.useragent")))
        parameters[QStringLiteral("osm.useragent")] = QStringLiteral("QtLocation Map");

    w.initProvider(parameters);
    //////////////////////

    w.showMaximized();
    w.show();

    return app.exec();
}
