#ifndef COMMON_H
#define COMMON_H

#include <QString>

class QCoreApplication;

void error(QCoreApplication *pApp, const QString &pMsg);

void initApplication(QCoreApplication *pApp);

bool cliApplication(QCoreApplication *pApp, int *pRes);

#endif

