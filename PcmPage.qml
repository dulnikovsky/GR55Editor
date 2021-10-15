import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.15

import GR55EditorTypes 1.0

Page {
    required property PatchPcmWrapper wrapper

    width: 600
    height: 400

//    header: Label {
//        text: parent.title
//        font.pixelSize: Qt.application.font.pixelSize
//        padding: 10
//    }

    ColumnLayout {
        id: layout
        anchors.fill: parent
        spacing: 2

        Button {
            checkable: true
            checked: wrapper.toneswitch
            text: checked?qsTr("On"):qsTr("Off")
            onToggled: { wrapper.toneswitch = checked; text = checked?qsTr("On"):qsTr("Off"); }
        }

        SpinBox {
            value: wrapper.level
            editable: true
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

        SpinBox {
            value: wrapper.octave
            editable: true
            from: -3
            to: 3
            onValueModified: wrapper.octave = value
        }

        Button {
            checkable: true
            checked: wrapper.chromatic
            text: checked?qsTr("On"):qsTr("Off")
            onToggled: { wrapper.chromatic = checked; text = checked?qsTr("On"):qsTr("Off");}
        }
        Button {
            checkable: true
            checked: wrapper.legato
            text: checked?qsTr("On"):qsTr("Off")
            onToggled: { wrapper.legato = checked; text = checked?qsTr("On"):qsTr("Off");}
        }
        Button {
            checkable: true
            checked: wrapper.nuance
            text: checked?qsTr("On"):qsTr("Off")
            onToggled: { wrapper.nuance = checked; text = checked?qsTr("On"):qsTr("Off");}
        }
    }
}
