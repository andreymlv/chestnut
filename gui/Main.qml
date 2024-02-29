import QtQuick

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    Text {
        text: "Hello World!"
        font.family: "Noto Sans"
        font.pointSize: 24
        color: "red"
    }
}
