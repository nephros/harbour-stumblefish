import QtQuick 2.0
import Sailfish.Silica 1.0
Column {
    spacing: Theme.paddingSmall
    SectionHeader {
        text: qsTr('Phone Tracking')
    }
    DetailItem {
        label: qsTr('Status')
        }
    DetailItem {
        label: qsTr('Uploaded/Skipped')
    }
}
