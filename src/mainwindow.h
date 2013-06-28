#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTranslator>

namespace Ui {
class MainWindow;
}

class QSettings;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QString locale() const;

private:
    Ui::MainWindow *ui;

    QSettings *mSettings;

    QString mLocale;

    QTranslator mQtTranslator;
    QTranslator mAppTranslator;

    void loadSettings();
    void saveSettings();

    void setLocale(const QString &pLocale, const bool &pForceSetting);

    void notYetImplemented(const QString& message);

private slots:
    void on_actionExit_triggered();
    void on_actionSystem_triggered();
    void on_actionEnglish_triggered();
    void on_actionRussian_triggered();
    void on_actionHelp_triggered();
    void on_actionHomePage_triggered();
    void on_actionAbout_triggered();
};

#endif // MAINWINDOW_H
