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
        implicitWidth: 450;

        ColumnLayout
        {
            id: teszt
            anchors.top: parent.top
            anchors.fill: parent
            anchors.margins: 10
            RowLayout
            {
                Text{ id: textP; text: "P:";}

                TextField {
                    id: textFieldP

                    implicitWidth: 100

                    anchors.left: textP.right
                    anchors.leftMargin: 7

                    horizontalAlignment: TextInput.AlignRight

                    inputMethodHints: Qt.ImhDigitsOnly
                }

                Button {
                    id: buttonP

                    anchors.left: textFieldP.right
                    anchors.leftMargin: 5

                    text: "Set"

                    onClicked: {
                        parameterP = textFieldP.text;
                        textFieldP.text = "";
                    }
                }

                Text{ anchors.left: buttonP.right; anchors.leftMargin: 10; id: currentValueP; text: "Current value: " + parameterP.toString(); }
            }

            RowLayout
            {
                Text{ id: textI; text: "I: "; }

                TextField {
                    id: textFieldI

                    implicitWidth: 100

                    anchors.left: textI.right
                    anchors.leftMargin: 5

                    horizontalAlignment: TextInput.AlignRight

                    inputMethodHints: Qt.ImhDigitsOnly
                }

                Button {
                    id: buttonI

                    anchors.left: textFieldI.right
                    anchors.leftMargin: 5

                    text: "Set"

                    onClicked: {
                        parameterI = textFieldI.text;
                        textFieldI.text = "";
                    }
                }

                Text{ anchors.left: buttonI.right; anchors.leftMargin: 10; id: currentValueI; text: "Current value: " + parameterI.toString(); }
            }

            RowLayout
            {
                Text{ id: textD; text: "D:"; }

                TextField {
                    id: textFieldD

                    implicitWidth: 100

                    anchors.left: textD.right
                    anchors.leftMargin: 5

                    horizontalAlignment: TextInput.AlignRight

                    inputMethodHints: Qt.ImhDigitsOnly
                }

                Button {
                    id: buttonD

                    anchors.left: textFieldD.right
                    anchors.leftMargin: 5

                    text: "Set"

                    onClicked: {
                        parameterD = textFieldD.text;
                        textFieldD.text = "";
                    }
                }

                Text{ anchors.left: buttonD.right; anchors.leftMargin: 10; id: currentValueD; text: "Current value: " + parameterD.toString(); }
            }
        }
    }
}

