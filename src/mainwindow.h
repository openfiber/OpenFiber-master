#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTranslator>

namespace Ui {
class MainWindow;
}

class QSettings;
class PluginManager;

enum {
    NeedRestart = 1789
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void restart(const bool &pSaveSettings) const;

    QString locale() const;

protected:
    virtual void changeEvent(QEvent *pEvent);
    virtual void closeEvent(QCloseEvent *pEvent);
    virtual void showEvent(QShowEvent *pEvent);

private:
    Ui::MainWindow *ui;

    QSettings *mSettings;

    PluginManager *mPluginManager;

    QString mLocale;

    QTranslator mQtTranslator;
    QTranslator mAppTranslator;

    void loadSettings();
    void saveSettings() const;

    void setLocale(const QString &pLocale, const bool &pForceSetting);

    void notYetImplemented(const QString& message);

private slots:
    void on_actionFullScreen_triggered();
    void on_actionSystem_triggered();
    void on_actionEnglish_triggered();
    void on_actionRussian_triggered();
    void on_actionHelp_triggered();
    void on_actionHomePage_triggered();
    void on_actionAbout_triggered();

    void resetAll();
};

#endif // MAINWINDOW_H
