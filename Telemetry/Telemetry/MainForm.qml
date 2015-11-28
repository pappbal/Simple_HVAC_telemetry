import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4

Item {
    anchors.fill: parent
    objectName: "MainForm"

    width: 640
    height: 480

    property alias startButton: startButton
    property alias stopButton: stopButton

    function showMessage(messageText)
    {
        eventLogModel.append({message: messageText});
    }

    RowLayout {
        id: baseGrid
        anchors.fill: parent
        anchors.margins: margin

        GroupBox
        {
            Layout.fillHeight: true

            ColumnLayout
            {
                id: columnLayout1
                anchors.fill: parent

                Button {
                    id: startButton
                    anchors.topMargin: 10
                    anchors.margins: 5
                    anchors.top: parent.top

                    style: ButtonStyle {
                        background: Rectangle {
                            implicitWidth: 130
                            implicitHeight: 40
                            border.width: control.activeFocus ? 2 : 1
                            border.color: "#888"
                            radius: 4
                            gradient: Gradient {
                                GradientStop { position: 0 ; color: control.pressed ? "#0c0" : "#0e0" }
                                GradientStop { position: 1 ; color: control.pressed ? "#0a0" : "#0c0" }
                            }
                        }

                        label: Text
                        {
                            color: "white"
                            text: "START"
                            anchors.centerIn: parent
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 12
                            font.bold: true

                        }
                    }
                }

                Button {
                    id: stopButton
                    anchors.top: startButton.bottom
                    anchors.topMargin: 10
                    anchors.margins: 30

                    style: ButtonStyle {
                        background: Rectangle {
                            implicitWidth: 130
                            implicitHeight: 40
                            border.width: control.activeFocus ? 2 : 1
                            border.color: "#888"
                            radius: 4
                            gradient: Gradient {
                                GradientStop { position: 0 ; color: control.pressed ? "#d00" : "#f00" }
                                GradientStop { position: 1 ; color: control.pressed ? "#b00" : "#d00" }
                            }
                        }

                        label: Text
                        {
                            color: "white"
                            text: "STOP"
                            anchors.centerIn: parent
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 12
                            font.bold: true
                        }
                    }
                }

                Text {
                    id: messagesLabel
                    text: "Üzenetek:"
                    anchors.left: parent.left
                    anchors.top: stopButton.bottom
                    anchors.topMargin:30
                    anchors.leftMargin: 5
                    font.bold: true
                }

                Rectangle{
                    Layout.fillHeight: true

                    anchors.margins: 10
                    anchors.top: messagesLabel.bottom
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 0

                    implicitWidth: 240

                    color: "white"
                    border.width: 1
                    border.color: "black"

                    ScrollView
                    {
                        anchors.fill: parent
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        Layout.minimumWidth: 250
                        Layout.preferredWidth: 150
                        Layout.maximumWidth: 250
                        Layout.minimumHeight: 250

                        anchors.top: parent.top
                        anchors.topMargin: 3

                        ListView
                        {
                            id: eventLog
                            anchors.fill: parent

                            spacing: 3

                            delegate: Component {

                                Row {
                                    anchors.left: parent.left
                                    anchors.margins: 10
                                    //spacing: 100
                                    Text {
                                        text: message
                                        anchors.verticalCenter: parent.verticalCenter
                                        horizontalAlignment: Text.AlignLeft
                                    }
                                }
                           }
                            model: ListModel {
                                id: eventLogModel
                                ListElement {
                                    message: "Indul a program..."
                                }
                                ListElement {
                                    message: "Még egy üzenet"
                                }
                            }

                            onCountChanged: {
                                eventLog.currentIndex = eventLog.count - 1;
                            }
                        }
                    }
                }
            }
        }

        GroupBox
        {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }

    }
}
