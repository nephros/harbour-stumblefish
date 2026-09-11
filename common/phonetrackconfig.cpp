// SPDX-License-Identifier: MIT
#include "phonetrackconfig.h"

#include <QDir>

namespace Stumblefish {

uint PhoneTrackConfig::defaultConfigId() {
    const auto group = value(QStringLiteral("default")).toString();
    beginGroup(group);
    return value("id").toInt();
};

PhoneTrackInfo* PhoneTrackConfig::info(const QString& name) {
    PhoneTrackInfo* i = new PhoneTrackInfo;
    beginGroup(name);
    i->id = value(QStringLiteral("id")).value<uint>();
    i->name = name.toLocal8Bit().data();
    i->displayName = value(QStringLiteral("name")).toString();
    endGroup();
    return i;
};

QVariantList PhoneTrackConfig::model() {
    QVariantList list;
    list.reserve(childGroups().length());
    const QStringList supported = value("supported").toStringList();

    for (const QString& group : childGroups()) {
        QVariantMap map;
        map.insert("supported", supported.contains(group));
        beginGroup(group);
        map.insert("id", value(QStringLiteral("id")).value<uint>());
        map.insert("name", value(QStringLiteral("name")).toString());
        map.insert("urlTemplate", value(QStringLiteral("urlTemplate")).toString());
        map.insert("hasSession", value(QStringLiteral("hasSession")).toString());
        map.insert("hasDevice", value(QStringLiteral("hasDevice")).toString());
        endGroup();
        list.append(map);
    }
    return list;
}

QVariantMap* PhoneTrackConfig::liveTrackConfig() {
    if(!m_liveTrackConfig)
        checkLiveTrackConfig();
    return m_liveTrackConfig;
}

bool PhoneTrackConfig::haveLiveTrackConfig()
{
    if(!m_liveTrackConfig)
        checkLiveTrackConfig();
    return !m_liveTrackConfig->isEmpty()
            && m_liveTrackConfig->value("SessionID").isValid()
            && m_liveTrackConfig->value("UrlTemplate").isValid();
};

void PhoneTrackConfig::checkLiveTrackConfig() {
    QSettings ltconfig(QDir::homePath() + "/" + QString::fromLatin1(LiveTrackConfigFilePath), QSettings::IniFormat);
    if(ltconfig.contains("traccar") && ltconfig.value("traccar").toBool())
        return; // false; // FIXME: support traccar type

    if(ltconfig.contains("ID")) {
        const auto id = ltconfig.value("ID").toString().split("/");
        m_liveTrackConfig->insert("SessionID", id.first());
        m_liveTrackConfig->insert("DeviceID",  id.last());
    }
    if(ltconfig.contains("URL"))
        m_liveTrackConfig->insert("UrlTemplate",  ltconfig.value("URL").toString());
    qDebug() << "Found liveTrack config:" << ltconfig.allKeys();
}


} // namespace
