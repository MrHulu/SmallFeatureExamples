import QtQuick 2.15

Rectangle {
    id: control

    property int axisOffset: 12
    property real axisRotation: 60
    property real xOffset: 0
    property real yOffset: 0
    property real arc: axisRotation * Math.PI / 180
    readonly property real axisCenterX: axisCircle.x + axisCircle.width / 2
    readonly property real axisCenterY: axisCircle.y + axisCircle.height / 2
    readonly property matrix4x4 mat: tm.times(_p.m3).times(_p.m2).times(_p.m1)

    property matrix4x4 tm: Qt.matrix4x4(1, 0, 0, control.xOffset,
                                        0, 1, 0, control.yOffset,
                                        0, 0, 1, 0,
                                        0, 0, 0, 1)
    property matrix4x4 itm: Qt.matrix4x4(1, 0, 0, -control.xOffset,
                                         0, 1, 0, -control.yOffset,
                                         0, 0, 1, 0,
                                         0, 0, 0, 1)

    implicitHeight: 60
    implicitWidth: 400

    color: "transparent"
    border.width: 2
    border.color: 'red'
    radius: control.height / 2
    transform: Matrix4x4 {
        matrix: control.mat
    }

    Rectangle {
        id: axisCircle
        x: control.axisOffset
        y: control.height * 0.2
        width: control.height * 0.6
        height: control.height * 0.6
        radius: width / 2
        color: "transparent"
        border.width: 1
        border.color: control.border.color
    }

    QtObject {
        id: _p
        property matrix4x4 m1: Qt.matrix4x4(1, 0, 0, -control.axisCenterX,
                                            0, 1, 0, -control.axisCenterY,
                                            0, 0, 1, 0,
                                            0, 0, 0, 1)
        property matrix4x4 m2: Qt.matrix4x4(Math.cos(arc), -Math.sin(arc), 0, 0,
                                            Math.sin(arc),  Math.cos(arc), 0, 0,
                                                        0,              0, 1, 0,
                                                        0,              0, 0, 1)
        property matrix4x4 m3: Qt.matrix4x4(1, 0, 0, control.axisCenterX,
                                            0, 1, 0, control.axisCenterY,
                                            0, 0, 1, 0,
                                            0, 0, 0, 1)
    }
}
