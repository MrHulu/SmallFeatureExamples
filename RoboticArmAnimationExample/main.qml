import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15

Window {
    id: root
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    // MediaPlayItem {
    //     anchors.fill: parent
    //     source: "qrc:/resource/demo.avi"
        
    // }

    StickComponent {
        id: axis1
        axisOffset: 48
        axisRotation: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 80
        anchors.horizontalCenter: parent.horizontalCenter

        StickComponent {
            id: axis2
            xOffset: axis1.axisCenterX - axis2.axisCenterX
            yOffset: axis1.axisCenterY - axis2.axisCenterY
            width: 240
            height: 48
            opacity: 0.5
            border.color: 'blue'
            axisOffset: 8

            NumberAnimation on axisRotation {
                duration: 2000
                from: -80
                to: -105
                easing.type: Easing.InOutQuad
                loops: Animation.Infinite
            }

            StickComponent {
                id: axis3
                xOffset: 140
                yOffset: 8
                height: 36
                width: 80
                axisRotation: 100

            }
        }

        StickComponent {
            id: axis4
            xOffset: _p.tp.x - axisCenterX
            yOffset: _p.tp.y - axisCenterY
            height: 36
            width: 180
            arc: _p.arcs
        }

        Rectangle {
            id: steadyAxis
            x: Math.sqrt(_p.len * _p.len - _p.dy * _p.dy) + _p.tp.x - width / 2
            y: -60
            width: 60
            height: 60
            border.width: 2
            border.color: "green"
            color: "transparent"
            Rectangle {
                width: 40
                height: 40
                anchors.centerIn: parent
                radius: 20
                color: "transparent"
                border.width: 2
                border.color: "green"
            }
        }
    }

    QtObject {
        id: _p
        property vector4d p: Qt.vector4d(62, 18, 0, 1)
        property vector4d tp: axis2.mat.times(axis3.mat).times(p)
        property real len: axis4.width - axis4.axisCenterX * 2
        property real dy: _p.tp.y + steadyAxis.height / 2
        property real arcs: Math.asin(Math.abs(dy) / len)
    }
}
