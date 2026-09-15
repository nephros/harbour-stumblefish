// SPDX-License-Identifier: MIT
import QtQuick 2.0
import Sailfish.Silica 1.0

Column {
    id: column
    width: parent.width
    spacing: Theme.paddingMedium

    SectionHeader {
        text: "Phone tracking"
    }
    TextSwitch { id: phoneTrackEnable
        text: "Enable phone tracking"
        //description: checked
        //             ? "Keeps the collector daemon running after Stumblefish closes"
        //             : "Stops the collector daemon when Stumblefish closes"
        checked: !!stumblefish.settings.phoneTrackEnabled
        onClicked: stumblefish.setPhoneTrackEnabled(checked)
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
            text: "Enable live tracking"
            description: checked
                         ? "Locations will be submitted as they are discovered"
                         : "Location uploads will happen together with Stumble uploads"
            checked: !!stumblefish.settings.phoneTrackLiveMode
            onClicked: stumblefish.setPhoneTrackLive(checked)
        }

        ComboBox { id: phoneTrackBox
            width: parent.width
            label: "Service"
            currentIndex: stumblefish.settings.phoneTrackType

            menu: ContextMenu {
                Repeater {
                    model: phoneTrackConfigModel

                    delegate: MenuItem {
                        enabled: model.supported
                        opacity: enabled ? 1.0 : Theme.opacityLow
                        text: model.name
                        onClicked: if(model.supported) { stumblefish.setPhoneTrackType(model.id) } else { return }
                    }
                }
            }
        }

        TextField { id: phoneTrackUrlTemplate
            label: "Submission URL"
            text: !!stumblefish.settings.phoneTrackUrlTemplate
                   ? stumblefish.settings.phoneTrackUrlTemplate
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
                stumblefish.setPhoneTrackUrlTemplate(text);
                phoneTrackSession.focus = true
            }
        }
        PasswordField { id: phoneTrackSession
            enabled:  phoneTrackConfigModel.get(phoneTrackBox.currentIndex).hasSession
            text: stumblefish.settings.phoneTrackSessionID
            label: "Session ID"
            inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
            EnterKey.iconSource: "image://theme/icon-m-enter-next"
            EnterKey.onClicked: {
                stumblefish.setPhoneTrackSession(text);
                phoneTrackName.focus = true
            }
        }
        TextField { id: phoneTrackName
            enabled:  phoneTrackConfigModel.get(phoneTrackBox.currentIndex).hasDevice
            text: stumblefish.settings.phoneTrackDeviceID
            placeholderText: enabled ? label : "not required"
            label: "Device Name (optional)"
            EnterKey.onClicked: { stumblefish.setPhoneTrackName(text); focus = false }
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
            visible: (phoneTrackBox.currentIndex <= 1) && stumblefish.status.canApplyLiveTrackConfig
            Button {
                text: qsTr("Apply from LiveTrack")
                onClicked: Remorse.popupAction(page, qsTr("Importing config"), function() { stumblefish.applyLiveTrackConfig() })
            }
        }
    }
}
