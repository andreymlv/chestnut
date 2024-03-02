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
                id: serverListView
                anchors.fill: parent
                required model
                model: serverModel
                delegate: serverDelegate
            }
            RoundButton {
                id: addHostButton
                text: "+"
                anchors.bottom: serverListView.bottom
                anchors.bottomMargin: 10
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: bar.currentIndex = 1
            }
        }
        ColumnLayout {
            id: hostTab

            Button {
                Layout.fillWidth: true
                Layout.topMargin: 10
                Layout.leftMargin: 10
                Layout.rightMargin: 10

                text: qsTr("Start")
            }

            Button {
                Layout.fillWidth: true
                Layout.leftMargin: 10
                Layout.rightMargin: 10

                text: qsTr("Stop")
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
            }

            Button {
                Layout.fillWidth: true
                Layout.leftMargin: 10
                Layout.rightMargin: 10

                text: qsTr("Disconnect")
            }

            Item {
                Layout.fillHeight: true
            }
        }
    }
}
