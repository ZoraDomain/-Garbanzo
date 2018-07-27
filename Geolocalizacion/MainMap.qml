import QtQuick 2.7
import QtQuick.Controls 1.4
import QtLocation 5.9
import QtPositioning 5.5
import QtQuick.Layouts 1.1

Map {
    id: map
    property int lastX
    property int lastY
    property int mousePosX
    property int mousePosY
    property var infoCoordinate

    property var resquests //this array is used for know who need the geocode model and emit his signal
                           // 0 is for Coordinate&Address

    signal showContextMenu(variant coordinate)
    signal geocodeFinished()

    signal coordinateAndAddressReady(variant coordinate, variant address) // this is for show Coordinate And address View

    property alias geocodeModel: geocodeModel

    ///////////////////////////////////////////////////// Marker
    property variant markers
    property int markersCount: 0
    property int currentMarker: -1

    signal markerGeocodeLoading()
    signal markerReady(variant coordinate, variant address)
    signal mouseClicked(variant coordinate)
    signal showMarkerContextMenu(variant coordinate)
    signal markerGeocodeFinished()// this is use for the first time that the marker is add
    //signal markerGeocodeChanged(variant coordinate, variant address)// and this is when the marker changed your current position
    signal markerHasChanged(int index, variant coordinate, variant address)
    signal requestEraseMarker(int index)
    signal changedMapMarkerFocus(int index)
    signal newPositionInMarker(int index, double latitude, double longitude)
    ////////////////////////////////////////////////////////////


    ///////////////////////////////////////////////////// Bus
    property variant buses
    property int busCount: 0
    property int currentBus: -1

    signal busGeocodeLoading()
    signal busReady(variant coordinate, variant address)
    signal busClicked(variant coordinate)
    signal showBusContextMenu(variant coordinate)
    signal busGeocodeFinished()// this is use for the first time that the marker is add
    //signal busGeocodeChanged(variant coordinate, variant address)// and this is when the marker changed your current position
    signal busHasChanged(int index, variant coordinate, variant address)
    signal requestEraseBus(int index)
    signal changedMapBusFocus(int index)
    signal newPositionInBus(int index, double latitude, double longitude)
    ////////////////////////////////////////////////////////////

    function addBus() {
        var count = buses.length
        busCount++
        currentBus = busCount - 1

        var bus = Qt.createQmlObject('Bus {}', map)
        map.addMapItem(bus)
        bus.coordinate = mapMouseArea.lastCoordinate
        bus.plugin = map.plugin
        bus.busGeocodeFinished.connect(busReady)
        bus.requestBusChanged.connect(busHasChanged)
        bus.busGeocodeLoading.connect(busGeocodeLoading)

        var array = new Array
        for (var i = 0; i < count; i++) {
            array.push(buses[i])
        }
        array.push(bus)
        buses = array

        bus.requestBusGeocode(map.plugin, mapMouseArea.lastCoordinate)
    }

    function addMarker() {
        var count = markers.length
        markersCount++
        currentMarker = markersCount - 1;

        var marker = Qt.createQmlObject('Marker {}', map)
        map.addMapItem(marker)
        marker.coordinate = mapMouseArea.lastCoordinate
        marker.plugin = map.plugin
        marker.markerGeocodeFinished.connect(markerReady)
        marker.requestMarkerChanged.connect(markerHasChanged)
        marker.markerGeocodeLoading.connect(markerGeocodeLoading)

        var array = new Array
        for(var i = 0; i < count; i++) {
            array.push(markers[i])
        }
        array.push(marker)
        markers = array

        marker.requestMarkerGeocode(map.plugin, mapMouseArea.lastCoordinate)

    }

    function changedMarkerCoordinate(index, latitude, longitude) {
        var coordinate = QtPositioning.coordinate(latitude, longitude)
        for (var i = 0; i < map.markers.length; i++) {
            if (map.markers[i].index == index) {
                map.markers[i].changedCoordinate(coordinate);
                return
            }

        }
        console.log("This marker has error when change coordinate with index:" + index); ;
    }

    function changedBusCoordinate(index, latitude, longitude) {
        var coordinate = QtPositioning.coordinate(latitude, longitude)
        for (var i = 0; i < map.buses.length; i++) {
            if (map.buses[i].index == index) {
                map.buses[i].changedCoordinate(coordinate);
                return
            }

        }
        console.log("This bus has error when change coordinate with index:" + index); ;
    }

    function deleteAllMarker() { ////// this remove all marker
        for (var i = 0; i < map.markers.length; i++) {
            map.removeMapItem(map.markers[i])
            requestEraseMarker(map.markers[i].index)
        }

        var myArray = new Array()

        map.markers = myArray
        map.markersCount = 0;
    }

    function deleteAllBus() { ////// this remove all marker
        for (var i = 0; i < buses.length; i++) {
            map.removeMapItem(map.buses[i])
            resquestEraseBus(map.buses[i].index)
        }

        var myArray = new Array

        map.buses = myArray
        map.busCount = 0
    }

    function deleteMarker(indexArray) {
        var myArray = new Array()

        for (var i = 0; i < map.markers.length; i++) {
            if(indexArray != i) myArray.push(map.markers[i])
        }

        map.removeMapItem(map.markers[indexArray])
        requestEraseMarker(map.markers[indexArray].index)
        map.markers[indexArray].destroy()
        map.markers = myArray
        map.markersCount = map.markers.length

    }

    function deleteBus(indexArray) {
        var myArray = new Array
        console.log(map.buses.length + " " + busCount + " " + indexArray)
        for (var i = 0; i < map.buses.length; i++) {
            if (indexArray != i) myArray.push(map.buses[i])
        }
        map.removeMapItem(map.buses[indexArray]);
        resquestEraseBus(map.buses[indexArray].index)
        map.buses[indexArray].destroy()
        map.buses = myArray
        map.busCount = map.buses.length
    }

    function setMarkerIndex(index, indexMarker) {
        map.markers[indexMarker].setIndex(index)
    }

    function setBusIndex(index, indexBus) {
        map.buses[indexBus].setIndex(index)
    }

    function showMarkerCircle(index) {
        map.markers[index].showCircle()
    }

    function hideMarkerCircle(index) {
        map.markers[index].hideCircle()
    }

    function zoomIn() {
        zoomSlider.value = Math.round(map.zoomLevel)
        map.zoomLevel = Math.round(map.zoomLevel)
        map.zoomLevel = map.zoomLevel + 1
    }

    function zoomOut() {
        zoomSlider.value = Math.round(map.zoomLevel)
        map.zoomLevel = Math.round(map.zoomLevel)
        map.zoomLevel = map.zoomLevel - 1
    }

    function geocode(fromAddress) {
        geocodeModel.query = fromAddress
        geocodeModel.update();
    }

    function addressGeocode() {

        var address = {
            street: geocodeModel.get(0).address.street,
            district: geocodeModel.get(0).address.district,
            city: geocodeModel.get(0).address.city,
            county: geocodeModel.get(0).address.county,
            state: geocodeModel.get(0).address.state,
            countryCode: geocodeModel.get(0).address.countryCode,
            country: geocodeModel.get(0).address.country,
            postalCode: geocodeModel.get(0).address.postalCode
        }
        return address
    }

    function coordinateGeocode() {
        var coordinate = {
            latitude: geocodeModel.get(0).coordinate.latitude,
            longitude: geocodeModel.get(0).coordinate.longitude
        }
        return coordinate
    }

    function updateCoordinateInfo() {
        geocode(infoCoordinate)
    }

    function proceedToAllResquest() {
        if (resquests[0] === 1) {
            coordinateAndAddressReady(coordinateGeocode(), addressGeocode())
            resquests[0] = 0;
        }

    }

    function setBusFollowMe(index, set) {
        for (var i = 0; i < buses.length; i++) {
            if (buses[i].index == index)
                buses[i].setFollowMe(set)
        }
    }

    function updateCoordinateForBus(index, coordinate) {
        for (var i = 0; i < buses.length; i++) {
            if (buses[i].index == index) {
                if (buses[i].followMe == true) {
                   map.center = QtPositioning.coordinate(coordinate.latitude, coordinate.longitude)
                }
            }
        }
    }

    function update() {
        //point.radius -= map.zoomLevel * 2
    }

    function initResquests () {
        var myArray = new Array
        var lenght = 2 //this is the number of resquests

        for (var i = 0; i <= lenght; i++) {
            myArray[i] = 0;
            resquests = myArray
        }
    }

    function resquestCoordinateAndAddress() {
        resquests[0] = 1
        geocodeModel.query = mapMouseArea.lastCoordinate
        geocodeModel.update()
    }

    zoomLevel: 10

    center: QtPositioning.coordinate(18.50, -69.30)

    Behavior on center {
        CoordinateAnimation {
            duration: 700
            easing.type: Easing.InBack
        }
    }

    Behavior on zoomLevel {
        NumberAnimation {duration: 300}
    }

    onZoomLevelChanged: {
        //restart
        mouseAreaTimer.running = false
        mouseAreaTimer.running = true
        //
        update()
    }

    Component.onCompleted: {
        markers = new Array()
        buses = new Array()
        resquests = new Array()
        initResquests()
    }

    GeocodeModel {
        id: geocodeModel
        plugin: map.plugin
        autoUpdate: false

        onStatusChanged:
            if(status == GeocodeModel.Ready || status == GeocodeModel.NoError) {
                map.geocodeFinished()
            } else if (status == GeocodeModel.Error) {
                console.log("Error has ocurred in main map geocode" + geocodeModel.errorString)
            }
        onLocationsChanged:
            if(count == 1) {
                //map.point.radius = map.point.radius -300//(1000 / map.zoomLevel)
            }

    }

    Component {
        id: pointDelegate
        MapCircle {
            id: point
            color: "black"
            border.color: "gray"
            border.width: 6
            center: infoCoordinate
            radius: 300
            smooth: false
            opacity: 0.25

            Timer {
                interval: 100; running: true; repeat: false
                onTriggered: {
                    point.radius -= point.radius
                    point.opacity -= point.opacity
                }

            }

            Behavior on radius {
                NumberAnimation {
                    duration: 260
                }
            }

            Behavior on opacity {
                NumberAnimation {
                    duration: 100
                }
            }

            onRadiusChanged: {
                if (radius == 0 || radius < 1 ) {
                    point.visible = false
                } else {
                    point.visible = true
                }
            }
        }
    }

    MapItemView {
        model: geocodeModel
        //delegate: pointDelegate
    }

    MouseArea {
        id: mapMouseArea
        anchors.fill: parent
        property variant lastCoordinate
        property bool mouseEnable
        acceptedButtons: Qt.LeftButton | Qt.RightButton

        onPressed: {
            map.lastX = mouse.x
            map.lastY = mouse.y
            lastCoordinate = map.toCoordinate(Qt.point(mouse.x, mouse.y))
            //this is for info
            updateMousePos();   updateCoordinatePos();   updateCoordinateInfo()
            mapMouseArea.enabled = false
            mapMouseArea.enabled = true
        }

        onDoubleClicked: {
            if (mouse.button == Qt.LeftButton) {
                map.zoomIn()
                //map.center = map.toCoordinate(Qt.point(mouse.x, mouse.y))
                mouseAreaTimer.running = true

            }

            if (mouse.button == Qt.RightButton) {
                map.zoomOut()
                mouseAreaTimer.running = true
            }
       }

        onPressAndHold: {
            if (mouse.button == Qt.RightButton)
                showContextMenu(lastCoordinate)
            problemFeedback()
        }

        onMouseXChanged: {
            map.mousePosX = mouse.x
            map.lastX = mouse.x
            updateMousePos()
        }

        onMouseYChanged: {
            map.mousePosY = mouse.y
            map.lastY = mouse.y
            updateMousePos()
        }
        
        Timer {
            id: mouseAreaTimer
            interval: 1000; running: false; repeat: false
            onTriggered: {
                map.zoomLevel = Math.round(map.zoomLevel)
            }
        }

        function updateCoordinatePos() {
            var coor = map.toCoordinate(Qt.point(map.lastX, map.lastY));
            coordinatePos.text = "-Coordinate- Latitude: " + coor.latitude + " Longitude: " + coor.longitude;
            infoCoordinate = coor
            mouseClicked(infoCoordinate)
        }
        
        function updateMousePos() {
            pos.text = "Mouse Pos: X " + map.mousePosX + " Y: " + map.mousePosY +
                    " Mouse last Pos: X " + lastX + " Y: " + lastY;
        }

        function problemFeedback() {
            mapMouseArea.focus = false
        }
    }

    //this is for extra infomation
    ColumnLayout {
        id: infoLayout
        anchors.left: parent
        visible: false

        Label {
            id: pos
            color: "red"
        }

        Label {
            id: coordinatePos
            color: "blue"
        }

        Label {
            id: geocoding
            color: "red"
        }
    }

    //this is a Slider
    ColumnLayout {
        id: mapControlLayout
        anchors.left: parent.left
        anchors.margins: 20
        height: parent.height
        visible: false

        RowLayout {
            id: mapbutton
            anchors.top: parent.top
            anchors.margins: 25
            anchors.bottom: parent.bottom
            Button {
            }

            ColumnLayout {

                Button {

                }

                Button {

                }

                Button {


                }
            }

            Button {

            }

        }

        ColumnLayout {
            anchors.top: mapbutton.top
            anchors.margins: 100
            anchors.left: mapbutton.left
            anchors.right: mapbutton.right           

            Slider {
                id: zoomSlider
                minimumValue: 5
                maximumValue: 17
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                implicitHeight: 300
                orientation: Qt.Vertical
                value: map.zoomLevel
                onValueChanged: {
                    if (value >= 0) {
                        map.zoomLevel = value;
                    }
                }

        }

       }
    }

    function controlShow () {
        mapControlLayout.visible = true
    }
    function controlHide() {
        mapControlLayout.visible = false
    }


    Column {
        id: toolButton
        anchors.left: map.left
        anchors.top: map.top
        anchors.topMargin: 30

        Button {
            id: zoomInButton
            implicitHeight: 50
            implicitWidth: 50
            onClicked: map.zoomIn()
        }

        Button {
            id: zoomOutButton
            implicitHeight: 50
            implicitWidth: 50
            onClicked: map.zoomOut()
        }
    }

    onGeocodeFinished: proceedToAllResquest()
    onMarkerGeocodeFinished: markerReady(mapMouseArea.lastCoordinate, reverseGeocode())
    onBusGeocodeFinished: busReady(mapMouseArea.lastCoordinate, reverseGeocode())
    //onMarkerGeocodeChanged: markerHasChanged(coordinate, address);
    onCurrentMarkerChanged: changedMapMarkerFocus(map.markers[currentMarker].index)
    onCurrentBusChanged: changedMapBusFocus(map.buses[currentBus].index)
    onNewPositionInMarker: changedMarkerCoordinate(index, latitude, longitude)
    onNewPositionInBus: changedBusCoordinate(index, latitude, longitude)
    onBusHasChanged: updateCoordinateForBus(index, coordinate)

}

