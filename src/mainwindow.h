#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QString locale;

    void loadSettings();
    void saveSettings();

    void setLocale(const QString& newLocale);

    void notYetImplemented(const QString& message);

private slots:
    void on_actionExit_triggered();
    void on_actionEnglish_triggered();
    void on_actionRussian_triggered();
    void on_actionHelp_triggered();
    void on_actionHomePage_triggered();
    void on_actionAbout_triggered();
};

#endif // MAINWINDOW_H
