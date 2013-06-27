#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils.h"

#ifdef Q_WS_WIN
    #include <windows.h>
#endif

#include <QCloseEvent>
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QFileDialog>
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

    // Title of our main window

    setWindowTitle(qApp->applicationName());
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

    QMessageBox::warning(this, qApp->applicationName(), message+tr(" has not yet been implemented..."),
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

static const QString OpenfiberHomePageUrl = "https://github.com/openfiber";

void MainWindow::on_actionHomePage_triggered()
{
    // Look up OpenFiber's home page

    QDesktopServices::openUrl(QUrl(OpenfiberHomePageUrl));
}

void MainWindow::on_actionAbout_triggered()
{
    // Display some information about OpenFiber

    QMessageBox::about(this, tr("About"),
                        "<h1 align=center><strong>"+getAppVersion(qApp)+"</strong></h1>"
                       +"<h3 align=center><em>"+getOsName()+"</em></h3>"
                       +"<p align=center><em>"+getAppCopyright(true)+"</em></p>"
                       +"<a href=\""+QString(OpenfiberHomePageUrl)+"\"><strong>"+qApp->applicationName()+"</strong></a> "+tr("is a cross-platform modelling environment of cardiac fiber."));
}
