// SPDX-License-Identifier: MIT
#ifndef STUMBLEFISH_STORAGE_H
#define STUMBLEFISH_STORAGE_H

#include <QObject>
#include <QSqlDatabase>
#include <QVariantList>
#include <QVariantMap>

#include "observations.h"

#ifdef FIND_JOLLA_BUDDIES
struct PassReport
{
    int id;
    qint64 timestampMs;

    PassReport()
        : id(0)
        , timestampMs(0)
    {
    }
};
#endif

class Storage : public QObject
{
    Q_OBJECT

public:
    explicit Storage(QObject *parent = 0);
    ~Storage();

    bool open();
    QString lastError() const;

    int addReport(const Report &report);
    QList<Report> recentReports(int limit) const;
    QList<Report> unuploadedReports(int limit) const;
    Report report(int id) const;
    QList<Report> uploadCandidates(int limit, int maxRetryCount = -1) const;
    qint64 lastReportTimestamp() const;
    QVariantMap mapSummary() const;
    QVariantList mapCells(double minLatitude, double minLongitude,
                          double maxLatitude, double maxLongitude, int zoom) const;
    QVariantMap counts() const;
    bool deleteReport(int id);
    int clearPendingReports();
    int pruneReportsOlderThan(qint64 cutoffMs);

    bool markUploading(const QList<int> &ids);
    bool markUploaded(const QList<int> &ids);
    bool markFailed(const QList<int> &ids, const QString &error);
    bool markPending(int id);

    static QVariantMap reportSummaryToMap(const Report &report);
    static QVariantMap reportToMap(const Report &report);

#ifdef FIND_JOLLA_BUDDIES
    int addPassReport(const PassReport &report) { m_passReports.append(report); return m_passReports.count(); };
    qint64 lastPassReportTimestamp() const { return m_passReports.isEmpty() ? 0 : m_passReports.last().timestampMs; };
#endif

Q_SIGNALS:
    void changed();

private:
    bool migrate();
    bool exec(const QString &sql) const;
    QList<WifiObservation> wifiForReport(int reportId) const;
    QList<CellObservation> cellsForReport(int reportId) const;
    QList<BleObservation> bleForReport(int reportId) const;
    bool updateStatus(const QList<int> &ids, const QString &status, const QString &error, bool setUploaded);
    bool deleteReports(const QList<int> &ids);

    QSqlDatabase m_db;
    QString m_lastError;
#ifdef FIND_JOLLA_BUDDIES
    QList<PassReport> m_passReports;
#endif
};

#endif
