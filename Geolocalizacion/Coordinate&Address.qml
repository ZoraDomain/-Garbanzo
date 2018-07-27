import QtQuick 2.0
import QtPositioning 5.8
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Item {
    id: coordinateAddress
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

        Column { //coordinate Column
            spacing: 10
            Text {
                text: "Coordinate:"
                font.pixelSize: 25
                font.bold: true
            }
            Row {
                spacing: 10
                anchors.left: parent.left
                anchors.leftMargin: coordinateAddress.height / 7
                Text {
                    text: "Latitude:"
                    font.pixelSize: 20
                    font.bold: true
                }
                Text {
                    id: latitude
                    text: "0.00"
                    font.pixelSize: 20
                }
            }
            Row {
                spacing: 10
                anchors.left: parent.left
                anchors.leftMargin: coordinateAddress.height / 7
                Text {
                    text: "Longitude:"
                    font.pixelSize: 20
                    font.bold: true
                }
                Text {
                    id: longitude
                    text: "0.00"
                    font.pixelSize: 20
                }

            }
        }

        Column { //Address Column
            spacing: 10
            Text {
                text: "Address:"
                font.pixelSize: 25
                font.bold: true
            }
            Row {
                spacing: 10
                anchors.left: parent.left
                anchors.leftMargin: coordinateAddress.height / 7
                Text {
                    text: "Street:"
                    font.pixelSize: 20
                    font.bold: true
                }
                Text {
                    id: street
                    text: "0.00"
                    font.pixelSize: 20
                }
            }

            Row {
                spacing: 10
                anchors.left: parent.left
                anchors.leftMargin: coordinateAddress.height / 7
                Text {
                    text: "District:"
                    font.pixelSize: 20
                    font.bold: true
                }
                Text {
                    id: district
                    text: "0.00"
                    font.pixelSize: 20
                }
            }
            Row {
                spacing: 10
                anchors.left: parent.left
                anchors.leftMargin: coordinateAddress.height / 7
                Text {
                    text: "City:"
                    font.pixelSize: 20
                    font.bold: true
                }
                Text {
                    id: city
                    text: "0.00"
                    font.pixelSize: 20
                }

            }
            Row {
                spacing: 10
                anchors.left: parent.left
                anchors.leftMargin: coordinateAddress.height / 7
                Text {
                    text: "Country:"
                    font.pixelSize: 20
                    font.bold: true
                }
                Text {
                    id: country
                    text: "0.00"
                    font.pixelSize: 20
                }
            }
            Row {
                spacing: 10
                anchors.left: parent.left
                anchors.leftMargin: coordinateAddress.height / 7
                Text {
                    text: "State:"
                    font.pixelSize: 20
                    font.bold: true
                }
                Text {
                    id: state
                    text: "0.00"
                    font.pixelSize: 20
                }

            }
            Row {
                spacing: 10
                anchors.left: parent.left
                anchors.leftMargin: coordinateAddress.height / 7
                Text {
                    text: "Country Code:"
                    font.pixelSize: 20
                    font.bold: true
                }
                Text {
                    id: countryCode
                    text: "0.00"
                    font.pixelSize: 20
                }
            }
            Row {
                spacing: 10
                anchors.left: parent.left
                anchors.leftMargin: coordinateAddress.height / 7
                Text {
                    text: "Postal Code:"
                    font.pixelSize: 20
                    font.bold: true
                }
                Text {
                    id: postalCode
                    text: "0.00"
                    font.pixelSize: 20
                }

        }
        }
        Row { //Button
            spacing: 10
            anchors.left: parent.left
            anchors.leftMargin: coordinateAddress.height / 7
            Button {
                text: "Copy"
                font.pixelSize: 20
            }
            Button {
                text: "Ok"
                font.pixelSize: 20
                onClicked: closeForm()
            }
        }

        Item {
            id: space
            Layout.fillHeight: parent.height/2
        }
    }

    Component.onCompleted:  {
        latitude.text = coordinate.latitude.toString()
        longitude.text = coordinate.longitude.toString()
        street.text = address.street
        district.text = address.district
        city.text = address.city
        country.text = address.country
        state.text = address.state
        countryCode.text = address.countryCode
        country.text = address.country
        postalCode.text = address.postalCode

    }

}
