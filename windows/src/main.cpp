#include "common.h"

#include <iostream>

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QPointer>
#include <QProcess>

int main(int pArgC, char *pArgV[])
{
    // Create the application

    QCoreApplication *app = new QCoreApplication(pArgC, pArgV);

    // Some general initialisations

    initApplication(app);

    // Try to run OpenFiber as a CLI application

    int res;

    if (!cliApplication(app, &res)) {
        // OpenFiber isn't meant to be run as a CLI application, so start its GUI
        // version instead

        static const QString dotExe = ".exe";

        if (app->applicationFilePath().right(dotExe.size()) == dotExe) {
            // This is a safeguard from accidentally running a non-renamed (to
            // '.com') CLI version of OpenFiber

            error(app, "the CLI version of "+app->applicationName()+" has the wrong extension ('.exe' instead of '.com').");

            res = -1;
        } else {
            QString guiAppFilePath = app->applicationDirPath()+QDir::separator()+app->applicationName()+dotExe;

            if (!QFileInfo(guiAppFilePath).exists()) {
                // We can't find the GUI version of OpenFiber, so...

                error(app, "the GUI version of "+app->applicationName()+" cannot be found.");

                res = -1;
            } else {
                QProcess().startDetached(guiAppFilePath, app->arguments(), QProcess().workingDirectory());

                res = 0;
            }
        }
    }

    // Release some memory

    delete app;

    // We are done, so...

    return res;
}
