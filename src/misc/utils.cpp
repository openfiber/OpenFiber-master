#include "utils.h"

#include <QCoreApplication>
#include <QDate>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QResource>


QString exec(const QString &pProgram, const QStringList &pArgs = QStringList())
{
    // Execute and return the output of a program given its arguments

    QProcess process;

    process.start(pProgram, pArgs);
    process.waitForFinished();

    return process.readAll().trimmed();
}


QString getOsName()
{
#if defined(Q_OS_WIN)
    switch (QSysInfo::WindowsVersion) {
    case QSysInfo::WV_NT:
        return "Microsoft Windows NT";
    case QSysInfo::WV_2000:
        return "Microsoft Windows 2000";
    case QSysInfo::WV_XP:
        return "Microsoft Windows XP";
    case QSysInfo::WV_2003:
        return "Microsoft Windows 2003";
    case QSysInfo::WV_VISTA:
        return "Microsoft Windows Vista";
    case QSysInfo::WV_WINDOWS7:
        return "Microsoft Windows 7";
    case QSysInfo::WV_WINDOWS8:
        return "Microsoft Windows 8";
    default:
        return "Microsoft Windows";
    }
#elif defined(Q_OS_LINUX)
    QString os = exec("uname", QStringList() << "-o");

    if (os.isEmpty())
        // We couldn't find uname or something went wrong, so...

        return "Linux";
    else
        return os+" "+exec("uname", QStringList() << "-r");
#else
    #error Unsupported platform
#endif
}

QString getAppVersion(QCoreApplication *pApp)
{
    QString appVersion = pApp->applicationVersion();
    QString bitVersion;
    static const int SizeOfPointer = sizeof(void *);

    switch (SizeOfPointer) {
    case 4:
        bitVersion = "32-bit";

        break;
    case 8:
        bitVersion = "64-bit";

        break;
    default:
        // Not a size that we could recognise, so...

        bitVersion = "";
    }

    bool snapshot = appVersion.contains("-");
    QString res = pApp->applicationName()+" ";

    if (snapshot)
        res += "[";

    res += appVersion;

    if (snapshot)
        res += "]";

    if (!bitVersion.isEmpty())
        res += " ("+bitVersion+")";

    return res;
}

QString getAppCopyright(const bool &pHtml)
{
    return QObject::tr("Copyright")+" "+QString(pHtml?"&copy;":"")+"2013-"+QString::number(QDate().currentDate().year());
}



