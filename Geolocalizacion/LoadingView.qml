import QtQuick 2.0

Rectangle {
    id: rectBase
    color: "#c0b6b6"
    property int count: 1
    signal close()
    signal begin()

    Image {
        id: load
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 350
        height: 200
        width:  200
        source: "image/gear.png"

        Behavior on rotation {
            NumberAnimation {
                duration: 500
            }
        }
    }

    Text {
        id: loadText
        text: qsTr("Loading")
        anchors.top: load.bottom
        anchors.left: load.left
        height: 400
        width:  400
        font.pixelSize: 40
        font.family: "Courier New"
        color:  "#1b2631"
    }

    Timer {
      id:  timer
      interval: 500; running: false; repeat: true

      onTriggered: rectBase.update()
    }

    onBegin: timer.running = true

    function update() {
        load.rotation += 55;
        if (count == 1) {
            loadText.text =  qsTr("Loading.")
            count = count + 1
            return
        }
        if (count == 2) {
            loadText.text =  qsTr("Loading..")
            count = count + 1
            return
        }
        if (count == 3) {
            loadText.text =  qsTr("Loading...")
            count = 1
            return
        }


    }
}
