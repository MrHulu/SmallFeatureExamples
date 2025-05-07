// main.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
import QtQml 2.15

ApplicationWindow {
    visible: true
    width: 400
    height: 300
    title: "翻译Json里的内容"

    ColumnLayout {
        width: parent.width
        height: parent.height
        spacing: 10
        Switch {
            Layout.alignment: Qt.AlignCenter
            text: test.isChinese ? "zh_CN" : "en_US"
            checked: test.isChinese
            onCheckedChanged: test.isChinese = checked
        }
        Label {
            text: "测试：" + qsTranslate("HULU", "高达")
            Layout.alignment: Qt.AlignCenter
        }
        Label {
            text: "GD:" + qsTranslate("HULU", test.ts.GD)
            Layout.alignment: Qt.AlignCenter
        }
        Label {
            text: "JW:" + qsTranslate("HULU", test.ts.JW)
            Layout.alignment: Qt.AlignCenter
        }
        Label {
            text: "EFSF:" + qsTranslate("HULU", test.ts.EFSF)
            Layout.alignment: Qt.AlignCenter
        }
    }
}
