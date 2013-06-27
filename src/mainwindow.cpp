#include "mainwindow.h"
#include "ui_mainwindow.h"

#ifdef Q_WS_WIN
    #include <windows.h>
#endif

#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    // Set up the GUI

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{

    // Delete the GUI

    delete ui;

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
    notYetImplemented("MainWindow::on_actionEnglish_triggered");
}

void MainWindow::on_actionRussian_triggered()
{
    notYetImplemented("MainWindow::on_actionRussian_triggered");
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
