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
        Label {  text: "basePropertyMap（基础属性的Map）"  }
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
        Row {
            spacing: 10
            Text { text: "遍历basePopertyMap:"  }
            // 遍历 basePropertyMap 的key
            Repeater {
                model: basePropertyMap.keys()
                Text { text: modelData + ":" + basePropertyMap[modelData] }                    
            }
        }
    
        Rectangle {
            width: root.width
            height: 1
            color: "black"
        }

        Label {  text: "objectPropertyMap（类属性的Map）"  }
        Button {
            text: "p1.enabled:" + (objectPropertyMap.parameter1.enabled ? "true" : "false")
            onClicked: {
                objectPropertyMap.parameter1.enabled = !objectPropertyMap.parameter1.enabled
            }
        }
        Label {  text: "p1.value:" + objectPropertyMap.parameter1.value  }
        Button {
            text: "p2.enabled:" + (objectPropertyMap.parameter2.enabled ? "true" : "false")
            onClicked: {
                objectPropertyMap.parameter2.enabled = !objectPropertyMap.parameter2.enabled
            }
        }
        Label {  text: "p2.value:" + objectPropertyMap.parameter2.value  }
        Row {
            spacing: 10
            Text { text: "遍历objectPropertyMap:"  }
            // 遍历 basePropertyMap 的key
            Repeater {
                model: objectPropertyMap.keys()
                Text { text: modelData + ":" + objectPropertyMap[modelData] }                    
            }
        }
    }
}
