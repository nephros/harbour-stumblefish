import QtQuick 2.0
import Sailfish.Silica 1.0
Column {
    spacing: Theme.paddingSmall
    SectionHeader {
        text: qsTr('Jolla Buddies')
    }
    DetailItem {
        label: qsTr('Seen')
        }
    DetailItem {
        label: qsTr('WiFi')
    }
    DetailItem {
        label: qsTr('BT')
    }
    DetailItem {
        label: qsTr('JollaPass Beacon')
    }
}

