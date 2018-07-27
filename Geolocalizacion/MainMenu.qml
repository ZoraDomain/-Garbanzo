import QtQuick 2.5
import QtQuick.Controls 1.4

MenuBar {
    property variant file: file

    signal fileClicked(string item)
    signal viewClicked(string item)

    Menu {
        id: file
        title: qsTr("File")

        MenuItem {
            text: "New"
            onTriggered: function() {fileClicked("new")}
        }

        MenuItem {
            text: "Open"
            onTriggered: function() {fileClicked("open")}
        }

        MenuItem {
            text: "Save"
            onTriggered: function() {fileClicked("save")}
        }

        MenuItem {
            text: "Save as"
            onTriggered: function() {fileClicked("save as")}
        }

        MenuSeparator {
        }

        MenuItem {
            text: "Exit"
            onTriggered: function() {fileClicked("exit")}
        }
    }

     Menu {
        id: viewMenu
        title: qsTr("View")

        MenuItem {
            id: show_hideButton
            text: "Control"
            checkable: true
            checked: true

            onTriggered:
                    function() {
                        if (checked == false)
                            viewClicked("hideControl")
                        else
                            viewClicked("showControl")
                        }

        }
        MenuItem {
            text: "Maker"
            onTriggered: function() {viewClicked("maker")}
        }

     }
}
