#ifndef CLIAPPLICATION_H
#define CLIAPPLICATION_H

#include <QStringList>


class QCoreApplication;

class CliApplication {
public:
    explicit CliApplication(QCoreApplication *pApp);
    ~CliApplication();

    bool run(int *pRes);

private:
    QCoreApplication *mApp;

    void usage();

    void version();
    void about();

    void loadPlugins();
    void plugins();

    bool command(const QStringList pArguments, int *pRes);
};

#endif
