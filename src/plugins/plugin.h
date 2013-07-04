#ifndef PLUGIN_H
#define PLUGIN_H

#include "plugininfo.h"

#include <QObject>

static const QString SettingsOrganization = "IIP";
static const QString SettingsApplication = "OpenFiber";
static const QString SettingsPlugins = "Plugins";

#if defined(Q_OS_WIN)
    static const QString PluginPrefix = "";
    static const QString PluginExtension = ".dll";
#elif defined(Q_OS_LINUX)
    static const QString PluginPrefix = "lib";
    static const QString PluginExtension = ".so";
#else
    #error Unsupported platform
#endif

class Plugin;
class PluginManager;

typedef QList<Plugin *> Plugins;

class Plugin : public QObject
{
    Q_OBJECT

public:
    enum Status {
        UndefinedStatus,
        NotFound,
        InvalidInterfaceVersion,
        NotWanted,
        NotNeeded,
        Loaded,
        NotLoaded,
        NotPlugin,
        MissingOrInvalidDependencies
    };

    explicit Plugin(const QString &pFileName, const bool &pLoad,
                    const PluginInfo::InterfaceVersion &pExpectedInterfaceVersion,
                    const QString &pPluginsDir, PluginManager *pPluginManager);
    ~Plugin();

    QString name() const;
    PluginInfo * info() const;
    QObject * instance() const;
    Status status() const;
    QString statusErrors() const;
    int statusErrorsCount() const;

    static QString name(const QString &pFileName);
    static QString fileName(const QString &pPluginsDir, const QString &pName);
    static PluginInfo * info(const QString &pFileName);

    static bool load(const QString &pName);
    static void setLoad(const QString &pName, const bool &pToBeLoaded);

    static QStringList requiredPlugins(const QString &pPluginsDir,
                                       const QString &pName,
                                       const int &pLevel = 0);

private:
    QString mName;
    PluginInfo *mInfo;
    QObject *mInstance;
    Status mStatus;
    QString mStatusErrors;
};

#endif
