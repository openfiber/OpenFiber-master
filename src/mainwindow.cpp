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
//
static const QString SettingsOrganization = "IIP";
static const QString SettingsApplication = "OpenFiber";
//
static const QString SystemLocale  = "";
static const QString EnglishLocale = "en";
static const QString RussianLocale  = "ru";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Create our settings object

    mSettings = new QSettings(SettingsOrganization, SettingsApplication);

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

    // Delete some internal objects

    delete mSettings;

    // Delete the GUI

    delete ui;
}

static const QString SettingsGlobal               = "Global";
static const QString SettingsLocale               = "Locale";
static const QString SettingsGeometry             = "Geometry";
static const QString SettingsState                = "State";
static const QString SettingsDockedWidgetsVisible = "DockedWidgetsVisible";
static const QString SettingsStatusBarVisible     = "StatusBarVisible";

void MainWindow::loadSettings()
{
    // Retrieve and set the language to be used by OpenFiber

        setLocale(mSettings->value(SettingsLocale, SystemLocale).toString(), true);

        // Retrieve the geometry and state of the main window

        if (   !restoreGeometry(mSettings->value(SettingsGeometry).toByteArray())
            || !restoreState(mSettings->value(SettingsState).toByteArray())) {
            // The geometry and/or state of the main window couldn't be retrieved,
            // so go with some default settings

            // Default size and position of the main window

            double ratio = 1.0/13.0;
            QRect desktopGeometry = qApp->desktop()->availableGeometry();
            int horizSpace = ratio*desktopGeometry.width();
            int vertSpace  = ratio*desktopGeometry.height();

            setGeometry(desktopGeometry.left()+horizSpace,
                        desktopGeometry.top()+vertSpace,
                        desktopGeometry.width()-2*horizSpace,
                        desktopGeometry.height()-2*vertSpace);
        }

        // Retrieve whether the status bar is to be shown

        ui->statusBar->setVisible(mSettings->value(SettingsStatusBarVisible, true).toBool());
}

void MainWindow::saveSettings()
{
    // Keep track of the language to be used by OpenFiber

    mSettings->setValue(SettingsLocale, mLocale);

    // Keep track of the geometry and state of the main window

    mSettings->setValue(SettingsGeometry, saveGeometry());
    mSettings->setValue(SettingsState, saveState());
}

QString MainWindow::locale() const
{
    // Return the current locale

    return !mLocale.compare(SystemLocale)?
               QLocale::system().name().left(2):
               mLocale;
}

void MainWindow::setLocale(const QString &pLocale, const bool &pForceSetting)
{

    const QString systemLocale = QLocale::system().name().left(2);

    QString oldLocale = !mLocale.compare(SystemLocale)?systemLocale:mLocale;
    QString newLocale = !pLocale.compare(SystemLocale)?systemLocale:pLocale;

    // Check whether the new locale is different from the old one and if so,
    // then retranslate everything

    if (oldLocale.compare(newLocale) || pForceSetting) {
        // Specify the language to be used by OpenFiber

        qApp->removeTranslator(&mQtTranslator);
        mQtTranslator.load(":qt_"+newLocale);
        qApp->installTranslator(&mQtTranslator);

        qApp->removeTranslator(&mAppTranslator);
        mAppTranslator.load(":app_"+newLocale);
        qApp->installTranslator(&mAppTranslator);
    }

   // Update the checked menu item
   // Note: it has to be done every single time, since selecting a menu item
   //       will automatically toggle its checked status, so...

   ui->actionSystem->setChecked(!pLocale.compare(SystemLocale));

   ui->actionEnglish->setChecked(!pLocale.compare(EnglishLocale));
   ui->actionRussian->setChecked(!pLocale.compare(RussianLocale));
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

void MainWindow::on_actionSystem_triggered()
{
    // Select the system's language as the language used by OpenFiber

    setLocale(SystemLocale, true);
}

void MainWindow::on_actionEnglish_triggered()
{
    // Select English as the language used by OpenFiber

    setLocale(EnglishLocale, true);
}

void MainWindow::on_actionRussian_triggered()
{
    // Select Russian as the language used by OpenFiber

    setLocale(RussianLocale, true);
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
