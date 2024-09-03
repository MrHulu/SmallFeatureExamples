import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15

Window {
    id: root
    width: 640
    height: 480
    visible: true
    title: qsTr("Hulu")

    Column {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10
        Label {  text: "basePropertyMap"  }
        TextInput {
            text: "StringValue:" + basePropertyMap.StringValue
            onEditingFinished: {
                basePropertyMap.StringValue = text
                text = Qt.binding(function() { return "StringValue:" + basePropertyMap.StringValue; })
            }
        }
        Button {
            text: "boolValue:" + (basePropertyMap.boolValue ? "true" : "false")
            onClicked: {
                basePropertyMap.boolValue = !basePropertyMap.boolValue
            }
        }
        Label {  text: "intValue： " + basePropertyMap.intValue  }
    
        Rectangle {
            width: root.width
            height: 1
            color: "black"
        }

        Label {  text: "objectPropertyMap"  }
        Button {
            text: "p1:" + (objectPropertyMap.parameter1.enabled ? "true" : "false")
            onClicked: {
                objectPropertyMap.parameter1.enabled = !objectPropertyMap.parameter1.enabled
            }
        }
        Label {  text: "p1 value:" + objectPropertyMap.parameter1.value  }
        Button {
            text: "p2:" + (objectPropertyMap.parameter2.enabled ? "true" : "false")
            onClicked: {
                objectPropertyMap.parameter2.enabled = !objectPropertyMap.parameter2.enabled
            }
        }
        Label {  text: "p2 value:" + objectPropertyMap.parameter2.value  }
    }
}
