import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import Backend 1.0

Window {
    title: qsTr("Hulu PyQml Example")
    visible: true

    width: 300
    height: 100
    property Backend backend: Backend {}

    ColumnLayout {
        spacing: 10
        anchors.fill: parent        
        
        Text {
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredWidth: 100
            Layout.preferredHeight: 30
            id: text
        }
        Button {
            id: button
            Layout.preferredWidth: 100
            Layout.preferredHeight: 30
            Layout.alignment: Qt.AlignHCenter
            text: "Click me"
            onClicked: {
                backend.onButtonClicked(button.text)
            }
        }
    }

    Connections {
        target: backend
        function onMySignal(str) {
            text.text = str
        }
    }
}