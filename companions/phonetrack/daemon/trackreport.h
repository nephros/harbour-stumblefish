// SPDX-License-Identifier: MIT
#ifndef TRACKFISH_REPORT_H
#define TRACKFISH_REPORT_H

#include <QDateTime>
#include <QMetaType>
#include <QString>
#include <QStringList>
#include <QVariantMap>

#include <cfloat>

namespace Trackfish {

struct PositionFix
{
    bool valid;
    qint64 timestampMs;
    double latitude;
    double longitude;
    double altitude;
    double accuracy;
    double speed;
    double direction;
    int satellites;

    PositionFix()
        : valid(false)
        , timestampMs(0)
        , latitude(0.0)
        , longitude(0.0)
        , altitude(0.0)
        , accuracy(-1.0)
        , speed(DBL_MAX)
        , direction(DBL_MAX)
        , satellites(0)
    {
    }
};

struct Report
{
    int id;
    qint64 timestampMs;
    PositionFix position;
    QString mode;
    QString uploadStatus;
    int retryCount;
    QString lastError;
    QString endpoint;
    qint64 uploadedAtMs;
    int battery;

    Report()
        : id(0)
        , timestampMs(0)
        , retryCount(0)
        , uploadedAtMs(0)
        , battery(0)
    {
    }
};


} // namespace

Q_DECLARE_METATYPE(Trackfish::PositionFix)

#endif
