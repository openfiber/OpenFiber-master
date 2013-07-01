#include "plugininfo.h"

PluginInfo::PluginInfo(const InterfaceVersion &pInterfaceVersion,
                       const Category &pCategory, const bool &pCliSupport,
                       const bool &pManageable,
                       const QStringList &pDependencies,
                       const Descriptions &pDescriptions) :
    mInterfaceVersion(pInterfaceVersion),
    mCategory(pCategory),
    mCliSupport(pCliSupport),
    mManageable(pManageable),
    mDependencies(pDependencies),
    mFullDependencies(QStringList()),
    mDescriptions(pDescriptions)
{
}

PluginInfo::InterfaceVersion PluginInfo::interfaceVersion() const
{
    // Return the interface version used by the plugin

    return mInterfaceVersion;
}

PluginInfo::Category PluginInfo::category() const
{
    // Return the plugin's category

    return mCategory;
}

bool PluginInfo::hasCliSupport() const
{
    // Return whether the plugin includes support for CLI

    return mCliSupport;
}

bool PluginInfo::isManageable() const
{
    // Return the plugin's manageability

    return mManageable;
}

QStringList PluginInfo::dependencies() const
{
    // Return the plugin's (direct) dependencies

    return mDependencies;
}

QStringList PluginInfo::fullDependencies() const
{
    // Return the plugin's full dependencies (i.e. both its direct and indirect
    // dependencies)

    return mFullDependencies;
}


void PluginInfo::setFullDependencies(const QStringList &pFullDependencies)
{
    // Set the plugin's full dependencies

    mFullDependencies = pFullDependencies;
}

QString PluginInfo::description(const QString &pLocale) const
{
    // Return the plugin's description using the provided locale or an empty
    // string if no description can be found

    QString res = mDescriptions.value(pLocale);

    if (res.isEmpty())
        // There is no description for the required locale, so retrieve the
        // english description (which, hopefully, should exist)

        res = mDescriptions.value("en");

    return res;
}

Descriptions PluginInfo::descriptions() const
{
    // Return the plugin's descriptions

    return mDescriptions;
}

