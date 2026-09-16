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
    /*
    Companion {
        id: companion
        Component.onCompleted: {
            console.info ("Registered components:", stumblefishcompanion.availableCompanions())
        }
    }
    */
}
