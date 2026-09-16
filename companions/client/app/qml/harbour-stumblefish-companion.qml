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
    property bool phoneTrackingAvailable: false
    property bool glassFishAvailable: false
    property bool jollaPassAvailable: false
    function companionAvailable(name) {
        return (stumblefishcompanion.availableCompanions().indexOf(name) != -1)
    }
    Component.onCompleted: {
        var c = stumblefishcompanion.availableCompanions()
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
