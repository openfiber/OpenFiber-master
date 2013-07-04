#include <QApplication>
#include <QPointer>
#include <QProcess>
#include <QSettings>

#include "common.h"
#include "mainwindow.h"
#include "pluginmanager.h"

void removeInstances()
{
    // Remove all the 'global' information shared among OpenFiber and the
    // different plugins

    QSettings(SettingsOrganization, SettingsApplication).remove("Global");
}

int main(int pArgc, char *pArgv[])
{
    int res;

    QPointer<QApplication> app;
    QPointer<MainWindow> win;

    // Create the application

    app = new QApplication(pArgc, pArgv);

    // Some general initialisations

    initApplication(app);

    // Remove all 'global' instances, in case OpenFiber previously crashed or
    // something (and therefore didn't remove all of them before quitting)

    removeInstances();

    // Create the main window

    win = new MainWindow;

    // Show the main window

    win->show();

    // Execute the application

    res = app->exec();

    // Keep track of the application file and directory paths (in case we need
    // to restart OpenFiber)

    QString appFilePath = app->applicationFilePath();
    QString appDirPath  = app->applicationDirPath();

    // Delete the main window

    delete win;

    // Remove all 'global' instances that were created and used during this
    // session

    removeInstances();

    // Delete the application

    delete app;

    // We are done with the execution of the application, so now the question is
    // whether we need to restart


    if (res == NeedRestart)
        // Restart OpenFiber, but without providing any of the argument with which
        // OpenFiber was originally started, since we want to reset everything

        QProcess::startDetached(appFilePath, QStringList(), appDirPath);

    // We are done, so...

    return res;
}
