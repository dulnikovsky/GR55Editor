import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.15

import GR55EditorTypes 1.0

Page {
    required property PatchPcmWrapper wrapper

    width: 600
    height: 400

    header: Label {
        text: parent.title
        font.pixelSize: Qt.application.font.pixelSize
        padding: 10
    }

    ColumnLayout {
        id: layout
        anchors.fill: parent
        spacing: 2

        Button {
            checkable: true
            text: qsTr("On")
            checked: wrapper.toneswitch
            onToggled: wrapper.toneswitch = checked
        }

        SpinBox {
            value: wrapper.level
            from: 0
            to: 100
            onValueModified: wrapper.level = value
        }

        ComboBox {
            Layout.fillWidth: true
            model: wrapper.toneList
            currentIndex: wrapper.waveform
            onActivated: wrapper.waveform = currentIndex
        }
    }
}
