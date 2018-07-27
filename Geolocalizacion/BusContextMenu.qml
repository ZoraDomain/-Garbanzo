import QtQuick 2.5
import QtQuick.Controls 1.4

Menu {
    property variant coordinate
    property int busCount
    signal itemClicked(string item);

    function update() {
        clear()
        addItem(qsTr("Get coordinate")).triggered.connect(function(){itemClicked("getCoordinate")})
        addSeparator()
        addItem(qsTr("Draw circle")).triggered.connect(function(){itemClicked("drawCircle")})
        addItem(qsTr("Hide circle")).triggered.connect(function(){itemClicked("hideCircle")})
        addSeparator()
        addItem(qsTr("Move to")).triggered.connect(function(){itemClicked("moveTo")});
        addItem(qsTr("Delete bus")).triggered.connect(function(){itemClicked("deleteBus")})
        addSeparator()
        addItem(qsTr("Delete all buses")).triggered.connect(function(){itemClicked("deleteAllBuses")})
    }
}
