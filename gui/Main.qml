import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
  width: 640
  height: 480
  visible: true
  title: qsTr("chestnut")

  RowLayout {
    anchors.fill: parent

    Text { text: applicationData.getCurrentDateTime() }

    Button {
      Layout.fillWidth: true
      Layout.fillHeight: true
      text: qsTr("Connect")
    }

    Button {
      Layout.fillWidth: true
      Layout.fillHeight: true
      text: qsTr("Disconnect")
    }
  }
}
