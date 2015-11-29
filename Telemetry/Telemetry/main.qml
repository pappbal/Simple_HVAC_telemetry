import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

ApplicationWindow {
    title: qsTr("Hello World")
    width: 1300
    height: 600
    visible: true

    objectName: "ApplicationWindow"

    signal connectSignal();
    signal disconnectSignal();
    signal startSignal();
    signal stopSignal();

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            MenuItem {
                text: qsTr("&Connect")
                onTriggered: connectSignal();
            }
            MenuItem {
                text: qsTr("&Disconnect")
                onTriggered: disconnectSignal();
            }
            MenuItem {
                text: qsTr("E&xit")
                onTriggered: Qt.quit();
            }
        }
    }

    MainForm {
        startButton.onClicked: startSignal();
        stopButton.onClicked: stopSignal();
    }
}
