import QtQuick 2.5;
import QtLocation 5.6
import QtPositioning 5.3

MapQuickItem {
    id: marker
    property int index: 0
    property bool areYouNew: true //this is for know if the marker is recently added
    property var plugin
    anchorPoint.x: image.width/4
    anchorPoint.y: image.height

    signal requestMarkerGeocode(variant plugin, variant coordinate)
    signal markerGeocodeFinished(variant coordinate, variant address)
    signal markerGeocodeLoading();
    signal requestMarkerChanged(int index, variant coordinate, variant address)
    signal newPosition(variant coordinate);
    signal changedCoordinate(variant newCoordinate)

    function setIndex(newIndex) {
        index = newIndex
    }

    function markerChanged(xPos, yPos) {
        //coordinate = map.toCoordinate((Qt.point(xPos, yPos)))
        //console.log("xxxx" + xPos + "  yyyyy " + yPos)
        requestMarkerGeocode(plugin, coordinate)
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
            street: geocodeMarker.get(0).address.street,
            district: geocodeMarker.get(0).address.district,
            city: geocodeMarker.get(0).address.city,
            county: geocodeMarker.get(0).address.county,
            state: geocodeMarker.get(0).address.state,
            countryCode: geocodeMarker.get(0).address.countryCode,
            country: geocodeMarker.get(0).address.country,
            postalCode: geocodeMarker.get(0).address.postalCode
        }
        return address
    }

    function coordinateGeocode() {
        var coordinate = {
            longitude: geocodeMarker.get(0).coordinate.longitude,
            latitude: geocodeMarker.get(0).coordinate.latitude
        };
        return coordinate
    }

    GeocodeModel {
        id: geocodeMarker
        onStatusChanged: {
            if(status == GeocodeModel.Ready || status == GeocodeModel.NoError) {
                if (areYouNew == false) {
                    requestMarkerChanged(marker.index, coordinateGeocode(), addressGeocode())
                } else {
                    markerGeocodeFinished(coordinateGeocode(), addressGeocode())
                    areYouNew = false
                }
            }

            if (status == GeocodeModel.Loading) {
                markerGeocodeLoading()
            }

            if (status == GeocodeModel.Error) {
                console.log("Some is wrong with geocode (in Marker): " + GeocodeModel.errorString)
            }
        }
    }

    onRequestMarkerGeocode: {
        geocodeMarker.plugin = plugin
        geocodeMarker.query = coordinate
        geocodeMarker.update()
    }

    sourceItem: Image {
        id: image
        source: "image/marker.png"
        opacity: markerMouseArea.pressed ? 0.4 : 1.0
        height: 30; width: 20

        MouseArea  {
            id: markerMouseArea
            anchors.fill: parent
            hoverEnabled : false
            drag.target: marker
            preventStealing: true

            onPressed: {
                giveCurrentMarker()
            }
            onPressAndHold: {
                if (mouse.button == Qt.LeftButton) {
                    map.showMarkerContextMenu(coordinate)
                }
            }

            onReleased: {
                markerChanged(mouse.x, mouse.y)
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

    function giveCurrentMarker() {
        for (var i = 0; i < map.markers.length; i++) {
            if (marker == map.markers[i]) {
                map.currentMarker = i
                break
            }
        }
    }

    Component.onCompleted: {
        giveCurrentMarker();
        coordinate = map.toCoordinate((Qt.point(markerMouseArea.mouseX,
                                                markerMouseArea.mouseY)))}

    onChangedCoordinate: {
        coordinate = newCoordinate
        markerChanged()
    }
}
