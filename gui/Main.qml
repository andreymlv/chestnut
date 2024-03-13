import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import chestnut

ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: qsTr("chestnut")

    TabBar {
        id: bar
        width: parent.width
        TabButton {
            text: qsTr("Discover")
        }
        TabButton {
            text: qsTr("Host")
        }
        TabButton {
            text: qsTr("Connect")
        }
    }

    StackLayout {
        anchors.top: bar.bottom
        width: parent.width
        currentIndex: bar.currentIndex
        Item {
            id: discoverTab
            ListView {
                anchors.fill: parent
                model: Chestnut.rooms
                delegate: RoundButton {
                    text: modelData
                    anchors.horizontalCenter: parent.horizontalCenter
                }
            }
        }
        ColumnLayout {
            id: hostTab

            TextField {
                id: portTextField
                Layout.fillWidth: true
                Layout.topMargin: 10
                Layout.leftMargin: 10
                Layout.rightMargin: 10

                validator: IntValidator {
                    bottom: 1024
                    top: 65535
                }
                placeholderText: qsTr("Enter port")
                cursorVisible: true
                visible: true
            }

            Button {
                Layout.fillWidth: true
                Layout.topMargin: 10
                Layout.leftMargin: 10
                Layout.rightMargin: 10

                text: qsTr("Start")
                onClicked: Chestnut.run_room(portTextField.text)
            }

            Button {
                Layout.fillWidth: true
                Layout.leftMargin: 10
                Layout.rightMargin: 10

                text: qsTr("Stop")
                onClicked: Chestnut.stop_room(portTextField.text)
            }

            Item {
                Layout.fillHeight: true
            }
        }
        ColumnLayout {
            id: connectTab

            TextField {
                id: addressTextField
                Layout.fillWidth: true
                Layout.topMargin: 10
                Layout.leftMargin: 10
                Layout.rightMargin: 10

                placeholderText: qsTr("Enter IP address and port")
                cursorVisible: true
                visible: true
                // validator: RegularExpressionValidator {
                //     regularExpression: /\b\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}:\d{1,5}\b/
                // }
            }

            Button {
                Layout.fillWidth: true
                Layout.leftMargin: 10
                Layout.rightMargin: 10

                text: qsTr("Connect")
                onClicked: Chestnut.client_connect(addressTextField.text)
            }

            Button {
                Layout.fillWidth: true
                Layout.leftMargin: 10
                Layout.rightMargin: 10

                text: qsTr("Disconnect")
                onClicked: Chestnut.client_disconnect()
            }

            Item {
                Layout.fillHeight: true
            }
        }
    }
}
