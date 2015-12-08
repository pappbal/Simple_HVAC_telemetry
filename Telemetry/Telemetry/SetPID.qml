import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4

Item {

    objectName: "setPID"

    property int parameterP: 1;
    property int parameterI: 1;
    property int parameterD: 1;

    property alias buttonP: buttonP;
    property alias buttonI: buttonI;
    property alias buttonD: buttonD;

    property int labelWidth: 100;
    property int textFieldWidth: 100;
    property int gapWidth: 20;
    property int gapAfterLabel: 4;

    function getP()
    {
        return parameterP;
    }

    function getI()
    {
        return parameterI;
    }

    function getD()
    {
        return parameterD;
    }

    GroupBox
    {

        title: "Set controller parameters"
        implicitWidth: 585;

        ColumnLayout
        {
            id: teszt
            anchors.fill: parent
            anchors.margins: 10

            RowLayout
            {
                Text{ id: textP; Layout.preferredWidth: labelWidth; font.bold: true; horizontalAlignment: Text.AlignRight; text: "Parameter P:";}

                Item{ Layout.preferredWidth: gapAfterLabel}

                TextField {
                    id: textFieldP

                    implicitWidth: textFieldWidth

                    horizontalAlignment: TextInput.AlignRight

                    inputMethodHints: Qt.ImhDigitsOnly
                }

                Button {
                    id: buttonP

                    text: "Set"

                    onClicked: {
                        parameterP = textFieldP.text;
                        textFieldP.text = "";
                    }
                }

                Item{ Layout.preferredWidth: gapWidth}

                Text{ id: currentValueP; text: "Current set: " + parameterP.toString(); }
            }

            RowLayout
            {
                Text{ id: textI; Layout.preferredWidth: labelWidth; font.bold: true; horizontalAlignment: Text.AlignRight; text: "Parameter I: "; }

                Item{ Layout.preferredWidth: gapAfterLabel}

                TextField {
                    id: textFieldI

                    implicitWidth: textFieldWidth

                    horizontalAlignment: TextInput.AlignRight

                    inputMethodHints: Qt.ImhDigitsOnly
                }

                Button {
                    id: buttonI

                    text: "Set"

                    onClicked: {
                        parameterI = textFieldI.text;
                        textFieldI.text = "";
                    }
                }

                Item{ Layout.preferredWidth: gapWidth}

                Text{ id: currentValueI; text: "Current set: " + parameterI.toString(); }
            }

            RowLayout
            {
                Text{ id: textD; Layout.preferredWidth: labelWidth; font.bold: true; horizontalAlignment: Text.AlignRight; text: "Parameter D:"; }

                Item{ Layout.preferredWidth: gapAfterLabel}

                TextField {
                    id: textFieldD

                    implicitWidth: textFieldWidth

                    horizontalAlignment: TextInput.AlignRight

                    inputMethodHints: Qt.ImhDigitsOnly
                }

                Button {
                    id: buttonD

                    text: "Set"

                    onClicked: {
                        parameterD = textFieldD.text;
                        textFieldD.text = "";
                    }
                }

                Item{ Layout.preferredWidth: gapWidth}

                Text{ id: currentValueD; text: "Current set: " + parameterD.toString(); }
            }
        }
    }
}

