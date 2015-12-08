import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4

Item {

    objectName: "temperatureSettings"

    property int setTemp1: 100;
    property int setTemp2: 100;
    property int setTemp3: 100;
    property int setTemp4: 100;

    property alias buttonTemp1: buttonTemp1;
    property alias buttonTemp2: buttonTemp2;
    property alias buttonTemp3: buttonTemp3;
    property alias buttonTemp4: buttonTemp4;

    property int labelWidth: 110;
    property int textFieldWidth: 100;
    property int gapWidth: 20;

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
        implicitWidth: 585;

        ColumnLayout
        {
            anchors.fill: parent
            anchors.margins: 10

            RowLayout
            {
                Text{ id: textTemp1; Layout.preferredWidth: labelWidth; font.bold: true; text: "Temperature 1:";}

                TextField {
                    id: textFieldTemp1

                    implicitWidth: textFieldWidth

                    horizontalAlignment: TextInput.AlignRight

                    inputMethodHints: Qt.ImhDigitsOnly
                }

                Button {
                    id: buttonTemp1

                    text: "Set"

                    onClicked: {
                        setTemp1 = textFieldTemp1.text;
                    }
                }

                Item{ Layout.preferredWidth: gapWidth}

                Text{ id: setValueTemp1; text: "Current set: " + setTemp1.toString(); }
            }

            RowLayout
            {
                Text{ id: textTemp2; Layout.preferredWidth: labelWidth; font.bold: true; text: "Temperature 2:"}

                TextField {
                    id: textFieldTemp2

                    implicitWidth: textFieldWidth

                    horizontalAlignment: TextInput.AlignRight

                    inputMethodHints: Qt.ImhDigitsOnly
                }

                Button {
                    id: buttonTemp2

                    text: "Set"

                    onClicked: {
                        setTemp2 = textFieldTemp2.text;
                    }
                }

                Item{ Layout.preferredWidth: gapWidth}

                Text{ id: setValueTemp2; text: "Current set: " + setTemp2.toString(); }
            }

            RowLayout
            {
                Text{ id: textTemp3; Layout.preferredWidth: labelWidth; font.bold: true; text: "Temperature 3:"}

                TextField {
                    id: textFieldTemp3

                    implicitWidth: textFieldWidth

                    horizontalAlignment: TextInput.AlignRight

                    inputMethodHints: Qt.ImhDigitsOnly
                }

                Button {
                    id: buttonTemp3

                    text: "Set"

                    onClicked: {
                        setTemp3 = textFieldTemp3.text;
                    }
                }

                Item{ Layout.preferredWidth: gapWidth}

                Text{ id: setValueTemp3; text: "Current set: " + setTemp3.toString(); }
            }

            RowLayout
            {
                Text{ id: textTemp4; Layout.preferredWidth: labelWidth; font.bold: true; text: "Temperature 4:"}

                TextField {
                    id: textFieldTemp4

                    implicitWidth: textFieldWidth

                    horizontalAlignment: TextInput.AlignRight

                    inputMethodHints: Qt.ImhDigitsOnly
                }

                Button {
                    id: buttonTemp4

                    text: "Set"

                    onClicked: {
                        setTemp4 = textFieldTemp4.text;
                    }
                }

                Item{ Layout.preferredWidth: gapWidth}

                Text{ id: setValueTemp4; text: "Current set: " + setTemp4.toString(); }
            }
        }
    }
}
