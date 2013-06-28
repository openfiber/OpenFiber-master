#include <QApplication>


#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Create the main window

    MainWindow win;

    // Show the main window

    win.show();

    // Execute the application

    return app.exec();
}
