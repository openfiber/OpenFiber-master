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

//
static const QString SettingsOrganization = "IIP";
static const QString SettingsApplication = "OpenFiber";
//
static const QString SystemLocale  = "";
static const QString EnglishLocale = "en";
static const QString RussianLocale  = "ru";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mLocale(QString())
{
    // Create our settings object

    mSettings = new QSettings(SettingsOrganization, SettingsApplication);

    // Set up the GUI

    ui->setupUi(this);

    // Title of our main window

    setWindowTitle(qApp->applicationName());

    // A connection to handle the status bar

    connect(ui->actionStatusBar, SIGNAL(triggered(bool)),
            statusBar(), SLOT(setVisible(bool)));

    // Some connections to handle our various menu items

    connect(ui->actionExit, SIGNAL(triggered()),
            this, SLOT(close()));
    connect(ui->actionResetAll, SIGNAL(triggered()),
            this, SLOT(resetAll()));

    // Set the shortcuts of some actions
    // Note: we do it here, so that we can use standard shortcuts (whenever
    //       possible)...

    #if defined(Q_OS_WIN)
        // Note: QKeySequence::Quit corresponds to Alt+F4 on Windows, but it doesn't
        //       get shown in the menu item, not to mention that we would also like
        //       to support Ctrl+Q, so...

        ui->actionExit->setShortcuts(QList<QKeySequence>()
                                           << QKeySequence(Qt::ALT|Qt::Key_F4)
                                           << QKeySequence(Qt::CTRL|Qt::Key_Q));
    #elif defined(Q_OS_LINUX)
        ui->actionExit->setShortcut(QKeySequence::Quit);
    #else
        #error Unsupported platform
    #endif

        ui->actionFullScreen->setShortcut(QKeySequence::FullScreen);

    // Retrieve our default settings

    loadSettings();

    // Initialise the checked state of our full screen action, since OpenFiber may
    // (re)start in full screen mode

    ui->actionFullScreen->setChecked(isFullScreen());
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

void MainWindow::changeEvent(QEvent *pEvent)
{
    // Default handling of the event

    QMainWindow::changeEvent(pEvent);

    // Do a few more things for some changes

    if (   (pEvent->type() == QEvent::LocaleChange)
        && (ui->actionSystem->isChecked()))
        // The system's locale has changed, so update OpenFiber's locale in case
        // the user wants to use the system's locale

        setLocale(SystemLocale,true);
}

//==============================================================================

void MainWindow::closeEvent(QCloseEvent *pEvent)
{
    // Check with our plugins that it's OK to close

    bool canClose = true;

    // Close ourselves

    if (canClose) {
        // Keep track of our default settings
        // Note: it must be done here, as opposed to the destructor, otherwise
        //       some settings (e.g. docked windows) won't be properly saved...

        saveSettings();

        // Accept the event

        pEvent->accept();
    } else {
        // Ignore the event, i.e. don't close ourselves

        pEvent->ignore();
    }
}

//==============================================================================

void MainWindow::showEvent(QShowEvent *pEvent)
{
    // Default handling of the event

    QMainWindow::showEvent(pEvent);

    // Things which need to be done and can only be done once the main window is
    // fully created

    static bool firstTime = true;

    if (firstTime) {
        firstTime = false;

        // The first time we show OpenFiber, we want to make sure that its menu is
        // fine (i.e. it respects OpenFiber's settings that were loaded in the
        // constructor). Various connections (set in the constructor) take care
        // of this, but there is still one menu item (which tells us whether the
        // status bar is to be shown) for which no connection can be set. So, we
        // have to 'manually' set the status of that menu item here (as opposed
        // to, say, the constructor), since we may need (on Windows at least)
        // all of OpenFiber to be visible in order to be able to determine whether
        // the status bar is visible...

        ui->actionStatusBar->setChecked(statusBar()->isVisible());
    }
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

void MainWindow::saveSettings() const
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

    // Keep track of the new locale, if needed

    if (pLocale.compare(mLocale) || pForceSetting) {
        mLocale = pLocale;

        // Also keep a copy of the new locale in our global settings (so that it
        // can be retrieved from any plugin)

        QSettings settings(SettingsOrganization, SettingsApplication);

        settings.beginGroup(SettingsGlobal);
            settings.setValue(SettingsLocale, locale());
        settings.endGroup();
    }

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

        // Retranslate OpenFiber

        ui->retranslateUi(this);
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

void MainWindow::on_actionFullScreen_triggered()
{
    // Switch to / back from full screen mode

    if (isFullScreen())
        showNormal();
     else
        showFullScreen();
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

void MainWindow::restart(const bool &pSaveSettings) const
{
    // Restart OpenFiber after saving its settings, if required

    if (pSaveSettings)
        saveSettings();

    qApp->exit(NeedRestart);
}

void MainWindow::resetAll()
{
    if (QMessageBox::question(this, qApp->applicationName(),
                              tr("You are about to reset <strong>all</strong> of your settings. Do you wish to proceed?"),
                              QMessageBox::Yes|QMessageBox::No,
                              QMessageBox::Yes) == QMessageBox::Yes ) {
        // We want to reset everything, so clear all the user settings and
        // restart OpenFiber (indeed, a restart will ensure that the various dock
        // windows are, for instance, properly reset with regards to their
        // dimensions)

        mSettings->clear();

        // Restart OpenFiber without first saving its settings

        restart(false);
    }
}
