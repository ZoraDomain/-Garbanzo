import QtQuick 2.5;
import QtLocation 5.6
import QtPositioning 5.3

MapQuickItem {
    id: bus
    property int index: 0
    property bool areYouNew: true //this is for know if the marker is recently added
    property var plugin
    property bool followMe: false// this is for follow the object
    anchorPoint.x: image.width/4
    anchorPoint.y: image.height

    signal requestBusGeocode(variant plugin, variant coordinate)
    signal busGeocodeFinished(variant coordinate, variant address)
    signal busGeocodeLoading();
    signal requestBusChanged(int index, variant coordinate, variant address)
    signal newPosition(variant coordinate);
    signal changedCoordinate(variant newCoordinate)

    function setIndex(newIndex) {
        index = newIndex
    }

    function busChanged(xPos, yPos) {
        //coordinate = map.toCoordinate((Qt.point(xPos, yPos)))
        //console.log("xxxx" + xPos + "  yyyyy " + yPos)
        requestBusGeocode(plugin, coordinate)
    }

    function showCircle() {
        if (point.visible == true)
            return
        else
            point.visible = true
    }

    function hideCircle() {
        if (point.visible == false)
            return
        else
            point.visible = false
    }

    function addressGeocode() {
        var address = {
            street: geocodeBus.get(0).address.street,
            district: geocodeBus.get(0).address.district,
            city: geocodeBus.get(0).address.city,
            county: geocodeBus.get(0).address.county,
            state: geocodeBus.get(0).address.state,
            countryCode: geocodeBus.get(0).address.countryCode,
            country: geocodeBus.get(0).address.country,
            postalCode: geocodeBus.get(0).address.postalCode
        }
        return address
    }

    function coordinateGeocode() {
        var coordinate = {
            longitude: geocodeBus.get(0).coordinate.longitude,
            latitude: geocodeBus.get(0).coordinate.latitude
        };
        return coordinate
    }

    GeocodeModel {
        id: geocodeBus
        onStatusChanged: {
            if(status == GeocodeModel.Ready || status == GeocodeModel.NoError) {
                if (areYouNew == false) {
                    requestBusChanged(bus.index, coordinateGeocode(), addressGeocode())
                } else {
                    busGeocodeFinished(coordinateGeocode(), addressGeocode())
                    areYouNew = false
                }
            }

            if (status == GeocodeModel.Loading) {
                busGeocodeLoading()
            }

            if (status == GeocodeModel.Error) {
                console.log("Some is wrong with geocode (in Bus): " + GeocodeModel.errorString)
            }
        }
    }

    onRequestBusGeocode: {
        geocodeBus.plugin = plugin
        geocodeBus.query = coordinate
        geocodeBus.update()
    }

    sourceItem: Image {
        id: image
        source: "image/bus.png"
        opacity: busMouseArea.pressed ? 0.4 : 1.0
        height: 30; width: 20

        MouseArea  {
            id: busMouseArea
            anchors.fill: parent
            hoverEnabled : false
            drag.target: bus
            preventStealing: true

            onPressed: {
                giveCurrentBus()
            }
            onPressAndHold: {
                if (mouse.button == Qt.LeftButton) {
                    map.showBusContextMenu(coordinate)
                }
            }

            onReleased: {
                busChanged(mouse.x, mouse.y)
            }
        }

        Rectangle {
            id: point
            color: "blue"
            border.color: "gray"
            border.width: 3
            opacity: 0.2
            radius: 1000
            smooth: true
            visible: false
            width: 500
            height: 500
            anchors.centerIn: parent
        }
    }

    function giveCurrentBus() {
        for (var i = 0; i < map.buses.length; i++) {
            if (bus == map.buses[i]) {
                map.currentBus= i
                break
            }
        }
    }

    function setFollowMe(set) {
        disableAllFollowMe()
        bus.followMe = set;
    }

    function disableAllFollowMe () {
        for (var i = 0; i < map.buses.length; i++) {
            map.buses[i].followMe = false
        }
    }

    Component.onCompleted: {
        giveCurrentBus();
        coordinate = map.toCoordinate((Qt.point(busMouseArea.mouseX,
                                                busMouseArea.mouseY)))}

    onChangedCoordinate: {
        coordinate = newCoordinate
        busChanged()
    }
}

