// SPDX-License-Identifier: MIT
import QtQuick 2.0
import Sailfish.Silica 1.0

Column {
    id: column
    width: parent.width
    spacing: Theme.paddingMedium

//    enabled: phoneTrackAvailable
    property var settings
    Component.onCompleted: stumblecompanion.companionSettings("PhoneTrack")

    SectionHeader {
        text: qsTr("Phone Tracking")
    }
    TextSwitch { id: phoneTrackEnable
        text: qsTr("Enable phone tracking")
        //description: checked
        //             ? "Keeps the collector daemon running after stumblecompanion closes"
        //             : "Stops the collector daemon when stumblecompanion closes"
        checked: !!settings.phoneTrackEnabled
        onClicked: stumblecompanion.setCompanionSetting("PhoneTrack", "phoneTrackEnabled", checked)
    }

    ListModel { id: phoneTrackConfigModel
        Component.onCompleted: {
            var cfgs = PhoneTrackConfig.model
            cfgs.sort(function(a,b) { return a.id - b.id })
            cfgs.forEach(function(e) { phoneTrackConfigModel.append(e) })
        }
    }
    Column { id: phoneTrackCol
        width: parent.width
        enabled: phoneTrackEnable.checked

        TextSwitch { id: phoneTrackLiveMode
            text: qsTr("Enable live tracking")
            description: checked
                         ? qsTr("Locations will be submitted as they are discovered")
                         : qsTr("Location uploads will happen together with Stumble uploads")
            checked: !!stumblecompanion.settings.phoneTrackLiveMode
            onClicked: stumblecompanion.setPhoneTrackLive(checked)
        }

        ComboBox { id: phoneTrackBox
            width: parent.width
            label: qsTr("Service")
            currentIndex: stumblecompanion.settings.phoneTrackType

            menu: ContextMenu {
                Repeater {
                    model: phoneTrackConfigModel

                    delegate: MenuItem {
                        enabled: model.supported
                        opacity: enabled ? 1.0 : Theme.opacityLow
                        text: model.name
                        onClicked: if(model.supported) { stumblecompanion.setPhoneTrackType(model.id) } else { return }
                    }
                }
            }
        }

        TextField { id: phoneTrackUrlTemplate
            label: qsTr("Submission URL")
            text: !!stumblecompanion.settings.phoneTrackUrlTemplate
                   ? stumblecompanion.settings.phoneTrackUrlTemplate
                   : "" // fixme: default template
            placeholderText: phoneTrackConfigModel.get(phoneTrackBox.currentIndex).urlTemplate
            description: phoneTrackBox.currentIndex <= 1
                   ? qsTr("the app path will be added automatically")
                   : ""
            inputMethodHints: Qt.ImhUrlCharactersOnly | Qt.ImhNoPredictiveText
            EnterKey.iconSource: "image://theme/icon-m-enter-accept"
            EnterKey.onClicked: {
                text.replace(/\/$/, "")
                if (phoneTrackBox.currentIndex <= 1) {
                    text.replace(/apps\/phonetrack.*$/, "")
                }
                stumblecompanion.setPhoneTrackUrlTemplate(text);
                phoneTrackSession.focus = true
            }
        }
        PasswordField { id: phoneTrackSession
            enabled:  phoneTrackConfigModel.get(phoneTrackBox.currentIndex).hasSession
            text: stumblecompanion.settings.phoneTrackSessionID
            label: "Session ID"
            inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
            EnterKey.iconSource: "image://theme/icon-m-enter-next"
            EnterKey.onClicked: {
                stumblecompanion.setPhoneTrackSession(text);
                phoneTrackName.focus = true
            }
        }
        TextField { id: phoneTrackName
            enabled:  phoneTrackConfigModel.get(phoneTrackBox.currentIndex).hasDevice
            text: stumblecompanion.settings.phoneTrackDeviceID
            placeholderText: enabled ? label : "not required"
            label: qsTr("Device Name (optional)")
            EnterKey.onClicked: { stumblecompanion.setPhoneTrackName(text); focus = false }
        }
        Label {
            visible: importButt.visible
            x: Theme.horizontalPageMargin
            width: parent.width - 2 * Theme.horizontalPageMargin
            text: qsTr("Found configuation of the LiveTrack app. Tap the button to import.")
            color: Theme.secondaryColor
            font.pixelSize: Theme.fontSizeSmall
            wrapMode: Text.Wrap
        }
        ButtonLayout { id: importButt
            visible: (phoneTrackBox.currentIndex <= 1) && stumblecompanion.status.canApplyLiveTrackConfig
            Button {
                text: qsTr("Apply from LiveTrack")
                onClicked: Remorse.popupAction(page, qsTr("Importing config"), function() { stumblecompanion.applyLiveTrackConfig() })
            }
        }
    }
}
