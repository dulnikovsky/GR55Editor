import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.15

import GR55EditorTypes 1.0

Page {
    required property PatchPcmWrapper wrapper

    width: 600
    height: 600

//    header: Label {
//        text: parent.title
//        font.pixelSize: Qt.application.font.pixelSize
//        padding: 10
//    }

    RowLayout {
        anchors.fill: parent
        spacing: 2
        ColumnLayout {
            Layout.maximumWidth: 400

            RowLayout {

                Button {
                    checkable: true
                    checked: wrapper.toneswitch
                    text: checked?qsTr("On"):qsTr("Off")
                    onToggled: { wrapper.toneswitch = checked; text = checked?qsTr("On"):qsTr("Off"); }
                }

                Item {
                    // spacer item
                    Layout.fillWidth: true
                }

                Button {
                    checkable: true
                    checked: wrapper.chromatic
                    text: qsTr("Chromatic")
                    onToggled: { wrapper.chromatic = checked; text = checked?qsTr("On"):qsTr("Off");}
                }
                Button {
                    checkable: true
                    checked: wrapper.legato
                    text: qsTr("Legato")
                    onToggled: { wrapper.legato = checked; text = checked?qsTr("On"):qsTr("Off");}
                }
                Button {
                    checkable: true
                    checked: wrapper.nuance
                    text: qsTr("Nuance")
                    onToggled: { wrapper.nuance = checked; text = checked?qsTr("On"):qsTr("Off");}
                }
            }

            GridLayout {
                columns: 2

                Label {
                    text: qsTr("Level:")
                }
                SpinBox {
                    value: wrapper.level
                    editable: true
                    from: 0
                    to: 100
                    onValueModified: wrapper.level = value
                }

                Label {
                    text: qsTr("Tone:")
                }
                ComboBox {
                    Layout.fillWidth: true
                    model: wrapper.toneList
                    currentIndex: wrapper.waveform
                    onActivated: wrapper.waveform = currentIndex
                }

                Label {
                    text: qsTr("Octave:")
                }
                SpinBox {
                    value: wrapper.octave
                    editable: true
                    from: -3
                    to: 3
                    onValueModified: wrapper.octave = value
                }

                Label {
                    text: qsTr("Pan:")
                }
                SpinBox {
                    value: wrapper.pan
                    editable: true
                    from: -50
                    to: 50
                    onValueModified: wrapper.pan = value
                }

                Label {
                    text: qsTr("Pitch Shift:")
                }
                SpinBox {
                    value: wrapper.pitchshift
                    editable: true
                    from: -24
                    to: 24
                    onValueModified: wrapper.pitchshift = value
                }

                Label {
                    text: qsTr("Pitch Fine:")
                }
                SpinBox {
                    value: wrapper.pitchfine
                    editable: true
                    from: -50
                    to: 50
                    onValueModified: wrapper.pitchfine = value
                }

                Label {
                    text: qsTr("Portamento:")
                }
                ComboBox {
                    currentIndex: wrapper.portamento
                    model: ["Off", "On", "Tone"]
                    onCurrentIndexChanged: wrapper.portamento = currentIndex
                }

                Label {
                    text: qsTr("Portamento Time:")
                }
                SpinBox {
                    value: wrapper.portamentotime
                    editable: true
                    from: 0
                    to: 100
                    onValueModified: wrapper.portamentotime = value
                }
            }
        }
        Label {
            text: "Test"
        }
    }
}
