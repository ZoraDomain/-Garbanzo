import QtQuick 2.5
import QtQuick.Controls 2.0
import QtQuick.Controls 1.4
import QtPositioning 5.6
import QtLocation 5.5
import QtQuick.Layouts 1.0
import InterfaceIO 1.0

Item {
    id: app
    property variant map
    property variant parameters

    ////////////////// marker signal
    signal newMarkerData(string text, double latitude, double longitude,
                         string street, string district, string city, string county, string state,
                         string countryCode, string country, string postalCode)
    signal markerChanged(int index, double latitude, double longitude,
                         string street, string district, string city, string county, string state,
                         string countryCode, string country, string postalCode)
    signal newMarkerPosition(int index, double latitude, double longitude)
    signal eraseMarker(int index)
    signal currentMarkerChange(int index) // this is when you click in map marker
    signal stopMarkerPositionPlanner() // this stop position planner
    ////////////////////

    ////////////////// bus signal
    signal newBusData(string text, double latitude, double longitude,
                         string street, string district, string city, string county, string state,
                         string countryCode, string country, string postalCode)
    signal busChanged(int index, double latitude, double longitude,
                         string street, string district, string city, string county, string state,
                         string countryCode, string country, string postalCode)
    signal newBusPosition(int index, double latitude, double longitude)
    signal eraseBus(int index)
    signal currentBusChange(int index) // this is when you click in map
    signal stopBusPositionPlanner() // this stop position planner
    signal setBusFollowMe(int index, bool set)
    ////////////////////

    ////
    signal changeMapProvider(string provider)
    signal addProviderToMenu(string provider);
    ////
    signal updateInfo(string latitude, string longitude);
    signal showLoadingView()

    visible: true
    width: 1000
    height:  650

    function createMap(provider) {
        var plugin
        if (parameters && parameters > 0)
            plugin = Qt.createQmlObject ('import QtLocation 5.6; Plugin{ name:"' + provider + '"; parameters: app.parameters}', app)
        else
            plugin = Qt.createQmlObject('import QtLocation 5.6; Plugin{ name:"' + provider + '"}', app)

        if (map) {
            map.destroy()
        }
        map = mapComponent.createObject(page);
        map.plugin = plugin

        map.forceActiveFocus()
    }

    function getPlugin() {
        var plugin = Qt.createQmlObject('import QtLocation 5.6; Plugin{}', app)
        var myArray = new Array
        for (var i = 0; i < plugin.availableServiceProviders.length; i++) {
            var tempPlugin = Qt.createQmlObject('import QtLocation 5.6; Plugin {name: "'+ plugin.availableServiceProviders[i] + '"}', app)
            if (tempPlugin.supportsMapping())
                myArray.push(tempPlugin.name)
        }
        myArray.sort()
        return myArray
    }

    function initializeProviders(pluginParameters) {
        var parameters = new Array()
        for (var prop in pluginParameters) {
            var parameter = Qt.createQmlObject('import QtLocation 5.6; PluginParameter{ name: "'+ prop + '"; value: "' + pluginParameters[prop]+'"}',app)
            parameters.push(parameter)
        }
        app.parameters = parameters
        var plugins = getPlugin()
        for (var i = 0; i < plugins.length; i++) {
            addProviderToMenu(plugins[i])
        }

        createMap("osm");
    }

    ////////////////////////////////// this function give the index that will be use in the backend
    function giveIndexMarker(index) {
        map.setMarkerIndex(parseInt(index), parseInt(map.currentMarker))
    }

    function giveIndexBus(index) {
        map.setBusIndex(parseInt(index), parseInt(map.currentBus))
    }
    ////////////////////////////////////////

    MainMenu {
        id: mapMainMenu

        onFileClicked: {
            switch(item) {
            case "exit":
                app.close()
            }
        }

        onViewClicked: {
            switch(item) {
            case "showControl":
                map.controlShow()
                break;
            case "hideControl":
                map.controlHide()
                break;
            }

        }
    }

    ContextMenu {
        id: mapContextMenu

        function show(coordinate) {
            stackView.pop(page)
            mapContextMenu.coordinate = coordinate
            mapContextMenu.update()
            mapContextMenu.popup()
        }

        onItemClicked:
            switch(item) {
            case "addMaker":
                map.addMarker();
                break;
            case "addBus":
                map.addBus();
                break;
            case "getCoordinate":
                 map.resquestCoordinateAndAddress()
                break;
            }
    }

    BusContextMenu {
        id: busContextMenu

        function show() {
            stackView.pop(page)
            busContextMenu.busCount = map.buses.length
            busContextMenu.update()
            busContextMenu.popup()
        }

        onItemClicked: {
            switch(item) {
            case "getCoordinate":

                break;
            case "drawCircle":
                //map.showMarkerCircle(map.currentMarker)
                break;
            case "hideCircle":
                //map.hideMarkerCircle(map.currentMarker)
                break;
            case "deleteBus":
                map.deleteBus(map.currentBus)
                break;
             case "deleteAllBuses":
                 map.deleteAllBuses()
                 break;
            }
        }
    }

    MarkerContextMenu {
        id: markerContextMenu

        function show() {
            stackView.pop(page)
            markerContextMenu.markerCount = map.markers.length
            markerContextMenu.update()
            markerContextMenu.popup()
        }

        onItemClicked: {
            switch(item) {
            case "getCoordinate":

                break;
            case "drawCircle":
                map.showMarkerCircle(map.currentMarker)
                break;
            case "hideCircle":
                map.hideMarkerCircle(map.currentMarker)
                break;
            case "deleteMarker":
                map.deleteMarker(map.currentMarker)
                break;
             case "deleteAllMarker":
                 map.deleteAllMarker()
                 break;
            }

        }
    }

    Drawer {
        id: notifyDrawer
        width: 300
        height: 70
        modal: false
        background: Rectangle {
            id: notifyBackground
            color: "#d3c5c5"
            border.color: Qt.lighter(color)
            border.width: 2
            opacity: 0.8
            smooth: true
        }
        margins: 250

        function show(title, text) {
            notifyDrawer.open()
            notifyTitleLabel.text = title
            notifyLabel.text = text

        }

        Timer {
            interval: 3000; running: true; repeat: false;
            onTriggered: notifyDrawer.close()
        }

        ItemDelegate {
            Item {
                ColumnLayout {
                    x: 10
                    y: 5

                    Label {
                        id: notifyTitleLabel
                        font.pixelSize: 16
                    }

                    Label {
                        id: notifyLabel
                        font.pixelSize: 10
                    }

                }
            }
        }
    }


    StackView {
        id: stackView
        anchors.fill: parent
        focus: true
        initialItem: Item {
            id: page
        }

        function showCoordinateAndAddress(coordinate, address) {
            stackView.push({item: Qt.resolvedUrl("/Coordinate&Address.qml"), properties: {
                                                                                "coordinate" : coordinate,
                                                                                "address" : address
                               }})
            stackView.currentItem.closeForm.connect(stackView.closeForm)
        }

        function showLoadingView() {
            stackView.push({item: Qt.resolvedUrl("/loadingView.qml")})
            stackView.currentItem.begin()
        }

        function closeForm() {
            pop(page)
        }
    }

    Component {
            id: mapComponent

            MainMap {
                width: page.width
                height: page.height

                //marker signal
                onMarkerReady: io.addMarkerData(coordinate, address)
                onMarkerHasChanged: io.requestMarkerChanged(index, coordinate, address)
                onMarkerGeocodeLoading: stopMarkerPositionPlanner()
                onChangedMapMarkerFocus: currentMarkerChange(index)
                onRequestEraseMarker: eraseMarker(index)
                onShowMarkerContextMenu: markerContextMenu.show(coordinate)

                //bus signal
                onBusReady: io.addBusData(coordinate, address)
                onBusGeocodeLoading: stopBusPositionPlanner()
                onChangedMapBusFocus: currentBusChange(index)
                onBusHasChanged: io.requestBusChanged(index, coordinate, address)
                onShowBusContextMenu: busContextMenu.show(coordinate)
                onRequestEraseBus: eraseBus(index)

                //other signal
                onShowContextMenu: mapContextMenu.show(coordinate)
                onMouseClicked: io.updateCoordinate(infoCoordinate)

                //menu
                onCoordinateAndAddressReady: stackView.showCoordinateAndAddress(coordinate, address)

                Component.onCompleted: {
                    if (mapComponent.status == Component.Ready)
                        notifyDrawer.show("Welcome To Marker", "if you want to see the documentation click to me")

                }
            }
        }

    Interface {
        id: io

        function addMarkerData(coordinate, address) {
            newMarkerData("Marker_" + map.markersCount,
                          coordinate.latitude,  coordinate.longitude,
                          address.street,  address.district,  address.city,  address.county,
                          address.state, address.countryCode,  address.country,  address.postalCode);
        }

        function addBusData(coordinate, address) {
            newBusData("Bus_" + map.busCount,
                       coordinate.latitude,  coordinate.longitude,
                       address.street,  address.district,  address.city,  address.county,
                       address.state, address.countryCode,  address.country,  address.postalCode)
        }

        function updateCoordinate(coordinate) {
            updateInfo(coordinate.latitude.toString(), coordinate.longitude.toString());
        }

        function requestMarkerChanged(index, coordinate, address) {
            markerChanged(parseInt(index), coordinate.latitude,  coordinate.longitude,
                          address.street,  address.district,  address.city,  address.county,
                          address.state, address.countryCode,  address.country,  address.postalCode);
        }

        function requestBusChanged(index, coordinate, address) {
            busChanged(parseInt(index), coordinate.latitude,  coordinate.longitude,
                          address.street,  address.district,  address.city,  address.county,
                          address.state, address.countryCode,  address.country,  address.postalCode);
        }

    }
    onChangeMapProvider: {
        createMap(provider)
        if (map.error)
            console.log("Error when changed map provider")
    }

    onNewMarkerPosition: {
        map.newPositionInMarker(index, latitude, longitude)
    }

    onNewBusPosition: {
        map.newPositionInBus(index, latitude, longitude)
    }

    onSetBusFollowMe: {
        map.setBusFollowMe(index, set)
    }

}


