import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import ControllerControl 1.0

Window {
    id: root
    width: 640
    height: 480
    visible: true
    title: qsTr("Hulu")


    Switch {
        id: switchControl
        anchors.bottom: sendControlSignalButton.top
        anchors.horizontalCenter: sendControlSignalButton.horizontalCenter
        text: checked ? "Qml示例 " : "C++示例 "
        checked: true
    }    
    Button {
        id: sendControlSignalButton
        anchors.centerIn: parent
        text: "Control"    
        highlighted: controllerControl.status !== ControllerControl.Pending
        onClicked: {
            if(switchControl.checked) {
                controllerControl.control()
                    .then(function() {
                        console.log("Control operation succeeded");
                        ok = true;
                        sendControlSignalButton.enabled = true;
                    }, function(error) {
                        console.error("Control operation failed:", error);
                        ok = false;
                        sendControlSignalButton.enabled = true;
                    });
            }else {
                test.test()
            }
        }
    }
    Label {
        id: statusLabel
        anchors.top: sendControlSignalButton.bottom
        anchors.horizontalCenter: sendControlSignalButton.horizontalCenter
        text: switchControl.checked ? "ControllerControl Status: " + (controllerControl.status) : "Test reslut: " + (test.result)
    }
    

}
