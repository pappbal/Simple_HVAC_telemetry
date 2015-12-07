import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4

Item {

    objectName: "temperatureSettings"

    property int setTemp1: 1;
    property int setTemp2: 1;
    property int setTemp3: 1;
    property int setTemp4: 1;

    property int currentTemp1: 1;
    property int currentTemp2: 1;
    property int currentTemp3: 1;
    property int currentTemp4: 1;

    property alias buttonTemp1: buttonTemp1;
    property alias buttonTemp2: buttonTemp2;
    property alias buttonTemp3: buttonTemp3;
    property alias buttonTemp4: buttonTemp4;

    function getTemp1()
    {
        return setTemp1;
    }

    function getTemp2()
    {
        return setTemp2;
    }

    function getTemp3()
    {
        return setTemp3;
    }

    function getTemp4()
    {
        return setTemp4;
    }

    GroupBox
    {

        title: "Temperature settings"

        ColumnLayout
        {
            anchors.top: parent.top
            anchors.fill: parent
            anchors.margins: 10
            RowLayout
            {
                Text{ id: textTemp1; text: "Temp1:";}

                TextField {
                    id: textFieldTemp1

                    implicitWidth: 100

                    anchors.left: textTemp1.right
                    anchors.leftMargin: 7

                    horizontalAlignment: TextInput.AlignRight

                    inputMethodHints: Qt.ImhDigitsOnly
                }

                Button {
                    id: buttonTemp1

                    anchors.left: textFieldTemp1.right
                    anchors.leftMargin: 5

                    text: "Set"

                    onClicked: {
                        setTemp1 = textFieldTemp1.text;
                    }
                }

                Text{ anchors.left: buttonTemp1.right; anchors.leftMargin: 10; id: currentValueTemp1; text: "Current value: " + currentTemp1.toString(); }
            }

            RowLayout
            {
                Text{ id: textTemp2; text: "Temp2:";}

                TextField {
                    id: textFieldTemp2

                    implicitWidth: 100

                    anchors.left: textTemp2.right
                    anchors.leftMargin: 7

                    horizontalAlignment: TextInput.AlignRight

                    inputMethodHints: Qt.ImhDigitsOnly
                }

                Button {
                    id: buttonTemp2

                    anchors.left: textFieldTemp2.right
                    anchors.leftMargin: 5

                    text: "Set"

                    onClicked: {
                        setTemp2 = textFieldTemp2.text;
                    }
                }

                Text{ anchors.left: buttonTemp2.right; anchors.leftMargin: 10; id: currentValueTemp2; text: "Current value: " + currentTemp2.toString(); }
            }

            RowLayout
            {
                Text{ id: textTemp3; text: "Temp3:";}

                TextField {
                    id: textFieldTemp3

                    implicitWidth: 100

                    anchors.left: textTemp3.right
                    anchors.leftMargin: 7

                    horizontalAlignment: TextInput.AlignRight

                    inputMethodHints: Qt.ImhDigitsOnly
                }

                Button {
                    id: buttonTemp3

                    anchors.left: textFieldTemp3.right
                    anchors.leftMargin: 5

                    text: "Set"

                    onClicked: {
                        setTemp3 = textFieldTemp3.text;
                    }
                }

                Text{ anchors.left: buttonTemp3.right; anchors.leftMargin: 10; id: currentValueTemp3; text: "Current value: " + currentTemp3.toString(); }
            }

            RowLayout
            {
                Text{ id: textTemp4; text: "Temp4:";}

                TextField {
                    id: textFieldTemp4

                    implicitWidth: 100

                    anchors.left: textTemp4.right
                    anchors.leftMargin: 7

                    horizontalAlignment: TextInput.AlignRight

                    inputMethodHints: Qt.ImhDigitsOnly
                }

                Button {
                    id: buttonTemp4

                    anchors.left: textFieldTemp4.right
                    anchors.leftMargin: 5

                    text: "Set"

                    onClicked: {
                        setTemp4 = textFieldTemp4.text;
                    }
                }

                Text{ anchors.left: buttonTemp4.right; anchors.leftMargin: 10; id: currentValueTemp4; text: "Current value: " + currentTemp4.toString(); }
            }
        }
    }
}
