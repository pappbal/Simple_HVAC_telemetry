import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4

Item {
    anchors.fill: parent
    objectName: "MainForm"

    property alias startButton: startButton
    property alias stopButton: stopButton

    property alias buttonP: setPid.buttonP
    property alias buttonI: setPid.buttonI
    property alias buttonD: setPid.buttonD

    property alias buttonTemp1: temperatureSettingsID.buttonTemp1;
    property alias buttonTemp2: temperatureSettingsID.buttonTemp2;
    property alias buttonTemp3: temperatureSettingsID.buttonTemp3;
    property alias buttonTemp4: temperatureSettingsID.buttonTemp4;

    property bool isRunning: false;
    property bool isConnected: false;

    function setIsRunning(value)
    {
        isRunning = value;
    }

    function setIsConnected(value)
    {
        isConnected= value;
    }

    function showMessage(messageText)
    {
        eventLogModel.append({message: messageText});
    }

    RowLayout {
        id: baseGrid
        anchors.fill: parent

        GroupBox
        {
            id: leftGroup
            Layout.fillHeight: true

            ColumnLayout
            {
                id: columnLayout1
                anchors.fill: parent

                RowLayout
                {
                    ColumnLayout
                    {
                        StartButton
                        {
                            id: startButton
                        }

                        StopButton
                        {
                            id: stopButton
                        }
                    }

                    GroupBox
                    {
                        title: "Status"

                        Layout.fillWidth: true

                        ColumnLayout
                        {
                            Text{ Layout.fillHeight: true; text: isConnected ? "Connected" : "Disconnected" }
                            Text{ Layout.fillHeight: true; text: isRunning ? "Running" : "Stopped" }
                        }
                    }

                }

                Text {
                    id: messagesLabel
                    text: "Messages:"
                    font.bold: true
                }

                Rectangle{
                    Layout.fillHeight: true
                    implicitWidth: 340

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
                                    Text {
                                        text: message
                                        anchors.verticalCenter: parent.verticalCenter
                                        horizontalAlignment: Text.AlignLeft
                                    }
                                }
                           }
                            model: ListModel { id: eventLogModel }

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
            id: middleGroup
            Layout.fillHeight: true
            Layout.fillWidth: true

            ColumnLayout
            {
                id: columnLayout2

                SetPID
                {
                    id: setPid
                    Layout.preferredHeight: 125;
                }

                TemperatureSettings
                {
                    id: temperatureSettingsID
                    Layout.preferredHeight: 160;
                }

                MeasuredValues
                {
                    id: measuredValuesID
                    Layout.preferredHeight: 120;

                    measuredTemp1: valueMeasuredTemp1
                    measuredTemp2: valueMeasuredTemp2
                    measuredTemp3: valueMeasuredTemp3
                    measuredTemp4: valueMeasuredTemp4

                    measuredSpeed1: valueMeasuredSpeed1
                    measuredSpeed2: valueMeasuredSpeed2

                    measuredActuator1: valueMeasuredActuator1
                    measuredActuator2: valueMeasuredActuator2
                }

                GroupBox
                {
                    title: "Temperatures column diagram"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.preferredWidth: 585
                    Layout.preferredHeight: 200
                    ColumnDiagramTemperature{
                        id: columnDiagramTempID
                        Layout.preferredHeight: 100
                    }
                }

            }
        }

        ColumnLayout
        {
            id: rightColumn
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: middleGroup.right

            GroupBox
            {
                title: "Temperatures"

                ColumnLayout
                {
                    RowLayout
                    {
                        id: tempRadioButtons
                        Layout.fillWidth: true

                        RadioButton
                        {
                            id: selectTemp1
                            text: "Temp1"
                            checked: true

                            onClicked:
                            {
                                historyGraphTemperature.showTemp1 = !historyGraphTemperature.showTemp1;
                                historyGraphTemperature.requestPaint();
                            }
                        }
                        RadioButton
                        {
                            id: selectTemp2
                            text: "Temp2"
                            checked: true


                            onClicked:
                            {
                                historyGraphTemperature.showTemp2 = !historyGraphTemperature.showTemp2;
                                historyGraphTemperature.requestPaint();
                            }
                        }
                        RadioButton
                        {
                            id: selectTemp3
                            text: "Temp3"
                            checked: true


                            onClicked:
                            {
                                historyGraphTemperature.showTemp3 = !historyGraphTemperature.showTemp3;
                                historyGraphTemperature.requestPaint();
                            }
                        }
                        RadioButton
                        {
                            id: selectTemp4
                            text: "Temp4"
                            checked: true


                            onClicked:
                            {
                                historyGraphTemperature.showTemp4 = !historyGraphTemperature.showTemp4;
                                historyGraphTemperature.requestPaint();
                            }
                        }
                    }

                    HistoryGraphTemperature {
                        id: historyGraphTemperature

                        objectName: "historyGraphTemperature"

                        Layout.fillWidth: true
                        Layout.minimumWidth: 200
                        Layout.preferredWidth: 400
                        Layout.minimumHeight: 150
                        Layout.preferredHeight: 250

                        graphVelocities: historyGraphVelocity

                        graphTemperatures1: historyGraphTemperatures1
                        graphTemperatures2: historyGraphTemperatures2
                        graphTemperatures3: historyGraphTemperatures3
                        graphTemperatures4: historyGraphTemperatures4
                    }
                }
            }

            GroupBox
            {
                title: "Speeds"
                id: speeds

                ColumnLayout
                {
                    RowLayout
                    {
                        id: speedRadioButtons
                        Layout.fillWidth: true

                        anchors.bottom: historyGraphTemperature.top

                        RadioButton
                        {
                            id: selectSpeed1
                            text: "Speed1"
                            checked: true

                            onClicked:
                            {
                                historyGraphSpeed.showSpeed1 = !historyGraphSpeed.showSpeed1;
                                historyGraphSpeed.requestPaint();
                            }
                        }
                        RadioButton
                        {
                            id: selectSpeed2
                            text: "Speed2"
                            checked: true


                            onClicked:
                            {
                                historyGraphSpeed.showSpeed2 = !historyGraphSpeed.showSpeed2;
                                historyGraphSpeed.requestPaint();
                            }
                        }
                    }

                    HistoryGraphSpeed {
                        id: historyGraphSpeed

                        objectName: "historyGraphSpeed"

                        Layout.fillWidth: true
                        Layout.minimumWidth: 200
                        Layout.preferredWidth: 400
                        Layout.minimumHeight: 150
                        Layout.preferredHeight: 270

                        graphVelocities: historyGraphVelocity

                        graphSpeed1: historyGraphSpeeds1
                        graphSpeed2: historyGraphSpeeds2
                    }
                }
            }

            GroupBox
            {
                title: "Actuators"
                id: actuators

                ColumnLayout
                {
                    RowLayout
                    {
                        id: actuatorRadioButtons
                        Layout.fillWidth: true

                        anchors.bottom: historyGraphTemperature.top

                        RadioButton
                        {
                            id: selectAct1
                            text: "Actuator1"
                            checked: true

                            onClicked:
                            {
                                historyGraphActuator.showAct1 = !historyGraphActuator.showAct1;
                                historyGraphActuator.requestPaint();
                            }
                        }
                        RadioButton
                        {
                            id: selectAct2
                            text: "Actuator2"
                            checked: true


                            onClicked:
                            {
                                historyGraphActuator.showAct2 = !historyGraphActuator.showAct2;
                                historyGraphActuator.requestPaint();
                            }
                        }
                    }

                    HistoryGraphActuator {
                        id: historyGraphActuator

                        objectName: "historyGraphActuator"

                        Layout.fillWidth: true
                        Layout.minimumWidth: 200
                        Layout.preferredWidth: 400
                        Layout.minimumHeight: 150
                        Layout.preferredHeight: 270

                        graphVelocities: historyGraphVelocity

                        graphAct1: historyGraphAct1
                        graphAct2: historyGraphAct2
                    }
                }
            }
        }
    }
}

