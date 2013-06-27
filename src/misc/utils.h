#ifndef UTILS_H
#define UTILS_H

#include <QString>

class QCoreApplication;

QString getOsName();
QString getAppVersion(QCoreApplication *pApp);
QString getAppCopyright(const bool &pHtml);

#endif

