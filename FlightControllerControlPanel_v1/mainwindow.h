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

private slots:
    void on_PID_tuning_checkBox_clicked(bool checked);

    void on_checkBox_clicked(bool checked);

    void on_writeParamsAuto_checkBox_clicked(bool checked);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
