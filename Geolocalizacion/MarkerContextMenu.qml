import QtQuick 2.5
import QtQuick.Controls 1.4

Menu {
    property variant coordinate
    property int markerCount
    signal itemClicked(string item);

    function update() {
        clear()
        addItem(qsTr("Get coordinate")).triggered.connect(function(){itemClicked("getCoordinate")})
        addSeparator()
        addItem(qsTr("Draw circle")).triggered.connect(function(){itemClicked("drawCircle")})
        addItem(qsTr("Hide circle")).triggered.connect(function(){itemClicked("hideCircle")})
        addSeparator()
        addItem(qsTr("Move to")).triggered.connect(function(){itemClicked("moveTo")});
        addItem(qsTr("Delete marker")).triggered.connect(function(){itemClicked("deleteMarker")})
        addSeparator()
        addItem(qsTr("Delete all marker")).triggered.connect(function(){itemClicked("deleteAllMarker")})
    }
}
