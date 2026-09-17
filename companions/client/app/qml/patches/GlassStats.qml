import QtQuick 2.0
import Sailfish.Silica 1.0
Column {
    spacing: Theme.paddingSmall
    SectionHeader {
        text: qsTr('Glasses Detection')
    }
    DetailItem {
        label: qsTr('Seen')
        }
    DetailItem {
        label: qsTr('Notified')
    }
}
