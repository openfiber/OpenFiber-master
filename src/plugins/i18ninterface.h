#ifndef I18NINTERFACE_H
#define I18NINTERFACE_H

#include "interface.h"

#include <QTranslator>

class I18nInterface : public Interface
{
public:
    void setLocale(const QString &pLocale);
    void setPluginName(const QString &pPluginName);

    virtual void retranslateUi();

private:
    QString mPluginName;

    QTranslator mTranslator;
};

Q_DECLARE_INTERFACE(I18nInterface, "I18nInterface")

#endif
