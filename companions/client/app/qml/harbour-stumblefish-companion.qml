// SPDX-License-Identifier: MIT
import QtQuick 2.0
import Sailfish.Silica 1.0
//import org.stumblefish.companions 1.0
import "cover"
import "pages"


ApplicationWindow {
    allowedOrientations: defaultAllowedOrientations

    initialPage: Component {
        MainPage {}
    }

    cover: Component {
        CoverPage {}
    }
    readonly property bool phoneTrackingAvailable: false
    readonly property bool glassFishAvailable: false
    readonly property bool jollaPassAvailable: false
    Component.onCompleted: {
        var c = companion.availableCompanions()
        phoneTrackingAvailable = (c.indexOf("PhoneTrack") != -1)
        glassFishAvailable = (c.indexOf("GlassFish") != -1)
        jollaPassAvailable = (c.indexOf("JollaPass") != -1)
    }
    /*
    Companion {
        id: companion
        Component.onCompleted: {
            console.info ("Registered components:", stumblefishcompanion.availableCompanions())
        }
    }
    */
}
