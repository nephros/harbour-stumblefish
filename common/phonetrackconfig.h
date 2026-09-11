// SPDX-License-Identifier: MIT
#ifndef STUMBLEFISH_PHONETRACK_H
#define STUMBLEFISH_PHONETRACK_H

#include <QObject>
#include <QSettings>
#include <QDebug>

namespace Stumblefish {

static char PhoneTrackConfigFilePath[] = "/usr/share/harbour-stumblefish/phonetrack.ini";
const char LiveTrackConfigFilePath[] = ".config/harbour-livetrack/harbour-livetrack.conf";

struct PhoneTrackInfo {
     uint id;
     char* name;
     QString displayName;

     PhoneTrackInfo ()
     {
         id = -1;
         name = nullptr;
         displayName = QStringLiteral("unknown");
     }
};

class PhoneTrackConfig : public QSettings
{
    Q_OBJECT
    Q_PROPERTY(int count READ count CONSTANT);
    Q_PROPERTY(bool haveLiveTrackConfig READ haveLiveTrackConfig CONSTANT);
    Q_PROPERTY(QVariantList model READ model NOTIFY modelChanged);
public:
    explicit PhoneTrackConfig(const QString &path = QString::fromLatin1(PhoneTrackConfigFilePath),
                              Format format = QSettings::IniFormat,
                              QObject *parent = nullptr) : QSettings(path, format, parent)
        {
            if (isWritable())
                 qCritical() << "PhoneTrackConfig: ini file is writable!";
            checkLiveTrackConfig();
        };

    /* we hide all write operations: */
    bool isWritable() { return false; };

    uint count() { return childGroups().count(); };
    QStringList names() { return childGroups(); };
    QString defaultConfig() { return value(QStringLiteral("default")).toString(); };
    uint defaultConfigId();
    PhoneTrackInfo* info(const QString& name);

    QVariantList model();

    bool haveLiveTrackConfig();

public Q_SLOTS:
    QVariantMap* liveTrackConfig();

Q_SIGNALS:
    void modelChanged();

private:
    void checkLiveTrackConfig();

    /* hide all write operations: */
    //PhoneTrackConfig(const QString &organization, const QString &application = QString(), QObject *parent = Q_NULLPTR);
    //PhoneTrackConfig(Scope scope, const QString &organization, const QString &application = QString(), QObject *parent = Q_NULLPTR);
    //PhoneTrackConfig(Format format, Scope scope, const QString &organization, const QString &application = QString(), QObject *parent = Q_NULLPTR);
    //PhoneTrackConfig(QObject *parent = Q_NULLPTR);
    Format registerFormat(const QString &extension, ReadFunc readFunc, WriteFunc writeFunc, Qt::CaseSensitivity caseSensitivity = Qt::CaseSensitive);
    void setDefaultFormat(Format format);
    void setPath(Format format, Scope scope, const QString &path);

    void beginWriteArray(const QString &prefix, int size = -1);
    void clear();
    void remove(const QString &key);
    void setArrayIndex(int i);
    void setFallbacksEnabled(bool b);
    void setIniCodec(QTextCodec *codec);
    void setIniCodec(const char *codecName);
    void setValue(const QString &key, const QVariant &value);
    void sync();

    QVariantMap* m_liveTrackConfig = nullptr;
};

} // namespace

#endif // STUMBLEFISH_PHONETRACK_H
