import QtQuick 2.0
import QtPositioning 5.8
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Item {
    id: getCoordinateForm
    property var coordinate
    property var address
    signal closeForm

    ColumnLayout {
    id: mainLayout
    anchors.fill: parent
    spacing: 15

        Rectangle {
            id: rect
            anchors.top: parent.top
            width: parent.width
            Layout.preferredHeight: parent.height / 12
            color: "#D5D5D5"
            border.width: 2
            border.color: "#EEEEEE"
            opacity: 0.7

            Text {
                anchors.left: parent.left
                anchors.leftMargin: 28
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10
                text: "Coordinate & Address"
                font.pixelSize: 28
                font.bold: true
            }
        }
        Column {
            spacing: 10
            Text {
                text: "Coordinate:"
                font.pixelSize: 25
                font.bold: true
            }
            Row {
                spacing: 10
                anchors.left: parent.left
                anchors.leftMargin: getCoordinateForm.height / 7
                Text {
                    text: "Latitude:"
                    font.pixelSize: 20
                    font.bold: true
                }
                Text {
                    text: "0.00"
                    font.pixelSize: 20
                }
            }
            Row {
                spacing: 10
                anchors.left: parent.left
                anchors.leftMargin: getCoordinateForm.height / 7
                Text {
                    text: "Longitude:"
                    font.pixelSize: 20
                    font.bold: true
                }
                Text {
                    text: "0.00"
                    font.pixelSize: 20
                }

            }
        }

        Column {
            spacing: 10
            Text {
                text: "Address:"
                font.pixelSize: 25
                font.bold: true
            }
            Row {
                spacing: 10
                anchors.left: parent.left
                anchors.leftMargin: getCoordinateForm.height / 7
                Text {
                    text: "district:"
                    font.pixelSize: 20
                    font.bold: true
                }
                Text {
                    text: "0.00"
                    font.pixelSize: 20
                }
            }
            Row {
                spacing: 10
                anchors.left: parent.left
                anchors.leftMargin: getCoordinateForm.height / 7
                Text {
                    text: "city:"
                    font.pixelSize: 20
                    font.bold: true
                }
                Text {
                    text: "0.00"
                    font.pixelSize: 20
                }

            }
            Row {
                spacing: 10
                anchors.left: parent.left
                anchors.leftMargin: getCoordinateForm.height / 7
                Text {
                    text: "country:"
                    font.pixelSize: 20
                    font.bold: true
                }
                Text {
                    text: "0.00"
                    font.pixelSize: 20
                }
            }
            Row {
                spacing: 10
                anchors.left: parent.left
                anchors.leftMargin: getCoordinateForm.height / 7
                Text {
                    text: "state:"
                    font.pixelSize: 20
                    font.bold: true
                }
                Text {
                    text: "0.00"
                    font.pixelSize: 20
                }

            }
            Row {
                spacing: 10
                anchors.left: parent.left
                anchors.leftMargin: getCoordinateForm.height / 7
                Text {
                    text: "countryCode:"
                    font.pixelSize: 20
                    font.bold: true
                }
                Text {
                    text: "0.00"
                    font.pixelSize: 20
                }
            }
            Row {
                spacing: 10
                anchors.left: parent.left
                anchors.leftMargin: getCoordinateForm.height / 7
                Text {
                    text: "postalCode:"
                    font.pixelSize: 20
                    font.bold: true
                }
                Text {
                    text: "0.00"
                    font.pixelSize: 20
                }

        }
        }
        Row {
            spacing: 10
            anchors.left: parent.left
            anchors.leftMargin: getCoordinateForm.height / 7
            Button {
                text: "Copy"
                font.pixelSize: 20
            }
            Button {
                text: "Ok"
                font.pixelSize: 20
            }
        }
        Button{}
        Button{}
        Button{}
        Button{}
        Button{}
        Button{}
    }


}
