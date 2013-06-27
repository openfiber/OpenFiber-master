#include "cliapplication.h"
#include "utils.h"

#include <iostream>

#include <QCoreApplication>

CliApplication::CliApplication(QCoreApplication *pApp) :
    mApp(pApp)
{
}

CliApplication::~CliApplication()
{
    // Delete some internal objects
}

void CliApplication::usage()
{
    std::cout << "Usage: " << qPrintable(mApp->applicationName())
              << " [-a|--about] [-c|--command [<plugin>::]<command> <options>] [-h|--help] [-p|--plugins] [-v|--version] [<files>]"
              << std::endl;
    std::cout << " -a, --about     Display some information about OpenFiber"
              << std::endl;
    std::cout << " -c, --command   Send a command to one or all the plugins"
              << std::endl;
    std::cout << " -h, --help      Display this help information"
              << std::endl;
    std::cout << " -p, --plugins   Display the list of plugins"
              << std::endl;
    std::cout << " -v, --version   Display the version of OpenFiber"
              << std::endl;
}

void CliApplication::version()
{
    std::cout << qPrintable(getAppVersion(mApp)) << std::endl;
}


void CliApplication::about()
{
    version();

    std::cout << qPrintable(getOsName()) << std::endl;
    std::cout << qPrintable(getAppCopyright(false)) << std::endl;
    std::cout << std::endl;
    std::cout << qPrintable(mApp->applicationName())
              << " is a cross-platform modelling environment of cardiac fiber"
              << std::endl;
}

void CliApplication::loadPlugins()
{

}

void CliApplication::plugins()
{

}

bool CliApplication::command(const QStringList pArguments, int *pRes)
{
    return true;
}

bool CliApplication::run(int *pRes)
{
    *pRes = 0;   // By default, everything is fine

    // See what needs doing with the CLI options, if anything

    bool helpOption = false;
    bool aboutOption = false;
    bool versionOption = false;
    bool pluginsOption = false;
    bool commandOption = false;

    QStringList commandArguments = QStringList();

    foreach (const QString argument, mApp->arguments())
        if (!argument.compare("-h") || !argument.compare("--help")) {
            helpOption = true;
        } else if (!argument.compare("-a") || !argument.compare("--about")) {
            aboutOption = true;
        } else if (!argument.compare("-v") || !argument.compare("--version")) {
            versionOption = true;
        } else if (!argument.compare("-p") || !argument.compare("--plugins")) {
            pluginsOption = true;
        } else if (!argument.compare("-c") || !argument.compare("--command")) {
            commandOption = true;
        } else if (argument.startsWith('-')) {
            // The user provided at least one unknown option

            usage();

            *pRes = -1;

            break;
        } else if (commandOption) {
            // Not an option, so we consider it to be part of a command

            commandArguments << argument;
        }

    // Handle the option the user requested, if any

    if (!*pRes) {
        if (helpOption) {
            usage();
        } else if (aboutOption) {
            about();
        } else if (versionOption) {
            version();
        } else if (pluginsOption) {
            loadPlugins();

            plugins();
        } else if (commandOption) {
            // Make sure that we have at least one argument (which would be the
            // command itself) before loading the plugins and then sending the
            // command to the plugin(s)

            if (commandArguments.isEmpty()) {
                usage();
            } else {
                loadPlugins();

                if (!command(commandArguments, pRes))
                    usage();
            }
        } else {
            // The user didn't provide any option which requires running OpenFiber
            // as a CLI application

            return false;
        }
    }

    return true;
}



