%bcond_without phonetrack
%bcond_without glassfish
%bcond_without jollapass

%if %{with phonetrack}
%global need_companion 1
%endif

%if %{with glassfish}
%global need_companion 1
%endif

%if %{with jollapass}
%global need_companion 1
%endif

# SPDX-License-Identifier: MIT
Name:       harbour-stumblefish
Summary:    Location report collector for Sailfish OS
Version:    0.1.8.1
Release:    1
License:    MIT
Group:      Qt/Qt
URL:        https://github.com/abranson/harbour-stumblefish
Source0:    %{name}-%{version}.tar.bz2

BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5DBus)
BuildRequires:  pkgconfig(Qt5Network)
BuildRequires:  pkgconfig(Qt5Positioning)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  pkgconfig(Qt5Sql)
BuildRequires:  pkgconfig(Qt5Test)
BuildRequires:  pkgconfig(connman-qt5)
BuildRequires:  pkgconfig(nemonotifications-qt5)
BuildRequires:  pkgconfig(qofonoext)
BuildRequires:  pkgconfig(sailfishapp)
BuildRequires:  pkgconfig(systemsettings)
BuildRequires:  desktop-file-utils
Requires:       sailfishsilica-qt5 >= 0.10.9

%if 0%{?need_companion}
BuildRequires: qt5-qttools-linguist
%endif

%description
Stumblefish collects opt-in Wi-Fi, cell tower, and Bluetooth beacon
observations with a position fix, stores the reports locally, and can submit
them to a configurable Geosubmit endpoint. BeaconDB is the default endpoint.
%if 0%{?_chum}
Title: Stumblefish
Type: desktop-application
DeveloperName: Andrew Branson
PackagedBy: Andrew Branson
Categories:
 - Utility
Custom:
  Repo: https://github.com/abranson/harbour-stumblefish
Icon: https://github.com/abranson/harbour-stumblefish/raw/master/src/icons/172x172/apps/harbour-stumblefish.png
Screenshots:
 - https://github.com/abranson/harbour-stumblefish/raw/master/Screenshot1.png
 - https://github.com/abranson/harbour-stumblefish/raw/master/Screenshot2.png
 - https://github.com/abranson/harbour-stumblefish/raw/master/Screenshot3.png
 - https://github.com/abranson/harbour-stumblefish/raw/master/Screenshot4.png
Links:
  Homepage: https://github.com/abranson/harbour-stumblefish
  Bugtracker: https://github.com/abranson/harbour-stumblefish/issues
%endif

%if %{with phonetrack}
%package -n harbour-trackfish
Summary: PhoneTrack Companion for Stumblefish
Requires: %{name} >= %{version}
Requires: harbour-stumblecompanion >= %{version}

%description -n harbour-trackfish
Submits location information to remote endpoints such as NextCloud PhoneTrack.
%endif


%if %{with glassfish}
%package -n harbour-glassfish
Summary: Smartglass detection Companion for Stumblefish
Requires: %{name} >= %{version}
Requires: harbour-stumblecompanion >= %{version}

%description -n harbour-glassfish
Smartglass detection Companion for Stumblefish
%endif


%if %{with jollapass}
%package -n harbour-passfish
Summary: Jolla User detection Companion for Stumblefish
Requires: %{name} >= %{version}
Requires: harbour-stumblecompanion >= %{version}
Obsoletes: harbour-jollapass <= %{version}

%description -n harbour-passfish
Jolla User detection Companion for Stumblefish
%endif

%if 0%{?need_companion}
%package -n harbour-stumblecompanion
Summary: Companion App for Stumblefish
Requires: %{name} >= %{version}
Obsoletes: %{name}-companion <= %{version}

%description -n harbour-stumblecompanion
Companion App for Stumblefish
%endif

%prep
%autosetup

%build
%qmake5 VERSION='%{version}' \
    QMAKE_CXX="ccache g++" \
%if %{with phonetrack}
    CONFIG+=phonetrack \
%endif
%if %{with glassfish}
    CONFIG+=glassfish \
%endif
%if %{with jollapass}
    CONFIG+=jollapass \
%endif
%{nil}

%make_build

%install
%qmake5_install

desktop-file-install --delete-original \
  --dir %{buildroot}%{_datadir}/applications \
  %{buildroot}%{_datadir}/applications/*.desktop

for size in 86 108 128 172; do
    icon_dir="%{buildroot}%{_datadir}/icons/hicolor/${size}x${size}/apps"
    mkdir -p "${icon_dir}"
    install -m 644 -p "src/icons/${size}x${size}/apps/%{name}.png" \
        "${icon_dir}/%{name}.png"
done

%check
%make_build check

%pre
if [ "$1" -gt 1 ]; then
    systemctl-user stop %{name}d || true
fi

%post
systemctl-user daemon-reload || true

%preun
if [ "$1" -eq 0 ]; then
    systemctl-user disable %{name}d || true
    systemctl-user stop %{name}d || true
fi

%postun
systemctl-user daemon-reload || true

%if %{with phonetrack}
%pre -n harbour-trackfish
if [ "$1" -gt 1 ]; then
    systemctl-user stop  harbour-trackfishd|| true
fi

%preun -n harbour-trackfish
if [ "$1" -eq 0 ]; then
    systemctl-user disable harbour-trackfishd || true
    systemctl-user stop harbour-trackfishd || true
fi
%endif

%if %{with glasshfish}
%pre -n harbour-glassfish
if [ "$1" -gt 1 ]; then
    systemctl-user stop  harbour-glassfishd|| true
fi

%preun -n harbour-glassfish
if [ "$1" -eq 0 ]; then
    systemctl-user disable harbour-glassfishd || true
    systemctl-user stop harbour-glassfishd || true
fi
%endif

%if %{with jollapass}
%pre -n harbour-passfish
if [ "$1" -gt 1 ]; then
    systemctl-user stop  harbour-passfishd|| true
fi

%preun -n harbour-passfish
if [ "$1" -eq 0 ]; then
    systemctl-user disable harbour-passfishd || true
    systemctl-user stop harbour-passfishd || true
fi
%endif

%files
%{_bindir}/%{name}
%{_bindir}/%{name}d
%{_datadir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/dbus-1/services/org.stumblefish.Collector.service
%{_datadir}/icons/hicolor/86x86/apps/%{name}.png
%{_datadir}/icons/hicolor/108x108/apps/%{name}.png
%{_datadir}/icons/hicolor/128x128/apps/%{name}.png
%{_datadir}/icons/hicolor/172x172/apps/%{name}.png
%{_sysconfdir}/sailjail/permissions/Stumblefish.permission
%{_userunitdir}/%{name}d.service
%if %{with phonetrack}
%exclude %{_datadir}/%{name}/lib/phonetrack/
%endif
%if 0%{?need_companion}
%exclude %{_datadir}/%{name}/lib/*/*.so
%exclude %{_datadir}/%{name}/lib/*/qmldir
%endif

%if %{with phonetrack}
%files -n harbour-trackfish
%{_bindir}/harbour-trackfishd
%{_datadir}/dbus-1/services/org.stumblefish.Tracker.service
%{_userunitdir}/harbour-trackfishd.service
%{_datadir}/%{name}/lib/phonetrack/
%endif

%if %{with glassfish}
%files -n harbour-glassfish
%{_bindir}/harbour-glassfishd
%{_datadir}/dbus-1/services/org.stumblefish.Lookout.service
%{_userunitdir}/harbour-glassfishd.service
#%%{_datadir}/%%{name}/lib/glassfish/
%endif

%if %{with jollapass}
%files -n harbour-passfish
%{_bindir}/harbour-passfishd
%{_datadir}/dbus-1/services/org.stumblefish.JollaPass.service
%{_userunitdir}/harbour-passfishd.service
#%%{_datadir}/%%{name}/lib/jollapass/
%endif

%if 0%{?need_companion}
%files -n harbour-stumblecompanion
%{_bindir}/%{name}-companion
%{_datadir}/applications/%{name}-companion.desktop
%{_datadir}/%{name}/lib/*/*.so
%{_datadir}/%{name}/lib/*/qmldir
%{_datadir}/icons/hicolor/86x86/apps/%{name}-companion.png
%{_datadir}/icons/hicolor/108x108/apps/%{name}-companion.png
%{_datadir}/icons/hicolor/128x128/apps/%{name}-companion.png
%{_datadir}/icons/hicolor/172x172/apps/%{name}-companion.png
#FIXME:
%exclude %{_datadir}/%{name}-companion/translations/*.qm
%endif
