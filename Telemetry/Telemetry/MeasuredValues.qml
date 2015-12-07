import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4

Item {

    objectName: "measuredValues"

    property int measuredTemp1;
    property int measuredTemp2;
    property int measuredTemp3;
    property int measuredTemp4;

    property int measuredSpeed1;
    property int measuredSpeed2;

    property int measuredActuator1;
    property int measuredActuator2;

    GroupBox
    {

        title: "Measured values"
        implicitWidth: 585
        Layout.preferredWidth: 585

        RowLayout
        {
            Layout.fillWidth: true
            ColumnLayout
            {
                Layout.preferredWidth: 180
                Text{ id: textMeasuredTemp1; text: "Temp1: " + measuredTemp1.toString(); }
                Text{ id: textMeasuredTemp2; text: "Temp2: " + measuredTemp2.toString(); }
                Text{ id: textMeasuredTemp3; text: "Temp3: " + measuredTemp3.toString(); }
                Text{ id: textMeasuredTemp4; text: "Temp4: " + measuredTemp4.toString(); }
            }

            ColumnLayout
            {
                Layout.preferredWidth: 180
                Layout.fillWidth: true
                Text{ id: textMeasuredSpeed1; text: "Speed1: " + measuredSpeed1.toString(); }
                Text{ id: textMeasuredSpeed2; text: "Speed2: " + measuredSpeed2.toString(); }
            }

            ColumnLayout
            {
                Layout.fillWidth: true
                Text{ id: textMeasuredActuator1; text: "Act1: " + measuredActuator1.toString(); }
                Text{ id: textMeasuredActuator2; text: "Act2: " + measuredActuator2.toString(); }
            }
        }
    }
}
