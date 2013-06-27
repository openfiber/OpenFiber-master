#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Create the main window

    MainWindow win;

    // Set the name of the main window to OpenCOR

    win.setWindowTitle("OpenFiber");

    // Keep track of the main window

    app.setActiveWindow(&win);

    // Show the main window

    win.show();

    // Execute the application

    return app.exec();
}
