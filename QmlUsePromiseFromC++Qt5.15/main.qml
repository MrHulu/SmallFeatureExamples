import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15

Window {
    id: root
    width: 640
    height: 480
    visible: true
    title: qsTr("Hulu")

    Button {
        id: sendControlSignalButton
        anchors.centerIn: parent        
        text: "Send Control Signal"
        
        property bool ok: false
        
        onClicked: {
            sendControlSignalButton.enabled = false;
            controllerControl.sendControlSignal()
                .then(function() {
                    console.log("Control operation succeeded");
                    ok = true;
                    sendControlSignalButton.enabled = true;
                }, function(error) {
                    console.error("Control operation failed:", error);
                    ok = false;
                    sendControlSignalButton.enabled = true;
                });
        }
    }
    Label {
        id: statusLabel
        anchors.top: sendControlSignalButton.bottom
        anchors.horizontalCenter: sendControlSignalButton.horizontalCenter
        text: "Status: " + (sendControlSignalButton.ok ? "OK" : "Failed")
    }

}
