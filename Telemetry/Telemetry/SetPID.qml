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
        implicitWidth: 585;

        ColumnLayout
        {
            id: teszt
            RowLayout
            {
                Text{ id: textP; text: "P:";}

                TextField {
                    id: textFieldP

                    implicitWidth: 100

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

                Text{ id: currentValueP; text: "Current value: " + parameterP.toString(); }
            }

            RowLayout
            {
                Text{ id: textI; text: "I: "; }

                TextField {
                    id: textFieldI

                    implicitWidth: 100

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

                Text{ id: currentValueI; text: "Current value: " + parameterI.toString(); }
            }

            RowLayout
            {
                Text{ id: textD; text: "D:"; }

                TextField {
                    id: textFieldD

                    implicitWidth: 100

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

                Text{ id: currentValueD; text: "Current value: " + parameterD.toString(); }
            }
        }
    }
}

