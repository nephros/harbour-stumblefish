// SPDX-License-Identifier: MIT
import QtQuick 2.0
import Sailfish.Silica 1.0

Column {
    id: column
    width: parent.width
    spacing: Theme.paddingSmall

    SectionHeader {
        text: qsTr("Phone Tracking")
    }
    DetailItem {
        label: qsTr("Status")
        value:  stumblefish.status.phoneTrackEnabled
                ? stumblefish.status.phoneTrackLiveMode ? qsTr("Enabled (live)") : qsTr("Enabled (collect)")
                : qsTr("Disabled")
    }
    DetailItem {
        label: qsTr("Uploaded/Skipped")
        value: stumblefish.status.phoneTrackSubmissions + "/" + stumblefish.status.phoneTrackSubmissionsSkipped
    }
}
