import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4

Button {
    //id: startButton

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
