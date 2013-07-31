#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QString>
#include "c64keys2usb.h"

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
    void TimerFunktion();

private:
    Ui::MainWindow *ui;
    C64Keys2USB *c64keys;
    QTimer *timer1;
};

#endif // MAINWINDOW_H
