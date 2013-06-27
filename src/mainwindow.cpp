#include "mainwindow.h"
#include "ui_mainwindow.h"

#ifdef Q_WS_WIN
    #include <windows.h>
#endif

#include <QDesktopServices>
#include <QMessageBox>
#include <QSettings>
#include <QUrl>

#define SETTINGS_INSTITUTION "World"
#define SETTINGS_LOCALE "Locale"
#define SETTINGS_GEOMETRY "Geometry"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    // Set up the GUI

    ui->setupUi(this);

    // Retrieve our default settings

    loadSettings();
}

MainWindow::~MainWindow()
{
    // Keep track of our default settings

    saveSettings();

    // Delete the GUI

    delete ui;
}

void MainWindow::loadSettings()
{
    QSettings settings(SETTINGS_INSTITUTION, qApp->applicationName());

    // Retrieve the geometry of the main window

    restoreGeometry(settings.value(SETTINGS_GEOMETRY).toByteArray());

    // Retrieve the language to be used by OpenFiber, which by default is based
    // on the system's locale

    setLocale(settings.value(SETTINGS_LOCALE, QLocale::system().name()).toString());
}

void MainWindow::saveSettings()
{
    QSettings settings(SETTINGS_INSTITUTION, qApp->applicationName());

    // Keep track of the geometry of the main window

    settings.setValue(SETTINGS_GEOMETRY, saveGeometry());

    // Keep track of the language to be used by OpenFiber

    settings.setValue(SETTINGS_LOCALE, locale);
}

void MainWindow::setLocale(const QString& newLocale)
{
    if (newLocale != locale)
    {
        locale = newLocale;
    }

    // Update the checked menu item
    // Note: it has to be done every single time, since selecting a menu item
    //       will automatically toggle its checked status, so...

    ui->actionEnglish->setChecked(newLocale.startsWith("en"));
    ui->actionRussian->setChecked(newLocale.startsWith("ru"));
}

void MainWindow::notYetImplemented(const QString& message)
{
    // Display a warning message about a particular feature having not yet been
    // implemented

    QMessageBox::warning(this, "OpenFiber", message+tr(" has not yet been implemented..."),
                         QMessageBox::Ok, QMessageBox::Ok);
}

void MainWindow::on_actionExit_triggered()
{
    // Exit OpenFiber

    close();
}

void MainWindow::on_actionEnglish_triggered()
{
    // Select English as the language used by OpenFiber

        setLocale("en");
}

void MainWindow::on_actionRussian_triggered()
{
    // Select Russsian as the language used by OpenFiber

        setLocale("ru");
}

void MainWindow::on_actionHelp_triggered()
{
    notYetImplemented("MainWindow::on_actionHelp_triggered");
}

void MainWindow::on_actionHomePage_triggered()
{
    // Look up the OpenFiber home page

    QDesktopServices::openUrl(QUrl("https://github.com/openfiber"));
}

void MainWindow::on_actionAbout_triggered()
{
    notYetImplemented("MainWindow::on_actionAbout_triggered");
}
