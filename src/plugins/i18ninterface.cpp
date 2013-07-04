#include "i18ninterface.h"

#include <QApplication>

void I18nInterface::setLocale(const QString &pLocale)
{
    // Update the plugin's translator

    qApp->removeTranslator(&mTranslator);
    mTranslator.load(QString(":%1_%2").arg(mPluginName, pLocale));
    qApp->installTranslator(&mTranslator);
}

void I18nInterface::setPluginName(const QString &pPluginName)
{
    // Set the name of the plugin

    mPluginName = pPluginName;
}

void I18nInterface::retranslateUi()
{
    // Nothing to do by default...
}
