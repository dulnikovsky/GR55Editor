import QtQuick 2.12
import QtQuick.Controls 2.5

ApplicationWindow {
    width: 800
    height: 480
    visible: true
    title: qsTr("GR-55 Editor")

    header : TextField {
        id: nameTextField
        width: 180
        maximumLength: 16
        text: patchnamewrapper.name
        onTextChanged: patchnamewrapper.name = text
        }

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        PcmPage {
            wrapper: patchpcmwrapper1
            title : qsTr("PCM 1")
        }
        PcmPage {
            wrapper: patchpcmwrapper2
            title : qsTr("PCM 2")
        }
    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex

        TabButton {
            text: qsTr("PCM 1")
        }
        TabButton {
            text: qsTr("PCM 2")
        }
    }
}
