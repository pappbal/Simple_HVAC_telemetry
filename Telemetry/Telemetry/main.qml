import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

ApplicationWindow {
    title: qsTr("Telemetry")
    width: 1800
    height: 985
    visible: true

    objectName: "ApplicationWindow"

    signal connectSignal();
    signal disconnectSignal();
    signal startSignal();
    signal stopSignal();

    signal setPSignal();
    signal setISignal();
    signal setDSignal();

    signal setTemp1();
    signal setTemp2();
    signal setTemp3();
    signal setTemp4();

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

        buttonP.onClicked: setPSignal();
        buttonI.onClicked: setISignal();
        buttonD.onClicked: setDSignal();

        buttonTemp1.onClicked: setTemp1();
        buttonTemp2.onClicked: setTemp2();
        buttonTemp3.onClicked: setTemp3();
        buttonTemp4.onClicked: setTemp4();
    }
}
