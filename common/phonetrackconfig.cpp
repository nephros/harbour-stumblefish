// SPDX-License-Identifier: MIT
#include "phonetrackconfig.h"

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

} // namespace
