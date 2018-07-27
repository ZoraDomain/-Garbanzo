import QtQuick 2.5
import QtQuick.Controls 1.4

Menu {
    property variant coordinate
    signal itemClicked(string item)

    function update() {
        clear()
        addItem(qsTr("Add Maker")).triggered.connect(function(){itemClicked("addMaker")})
        addItem(qsTr("Add Bus")).triggered.connect(function(){itemClicked("addBus")})
        addSeparator()
        addItem(qsTr("Get Coordinate")).triggered.connect(function(){itemClicked("getCoordinate")})
    }

}

