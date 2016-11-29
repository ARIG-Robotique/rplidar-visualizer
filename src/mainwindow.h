#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <rplidar.h>
#include "scanthread.h"
#include "lidardataviewwindow.h"

using namespace rp::standalone::rplidar;

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
    RPlidarDriver *drv = NULL;
    ScanThread *scanThread = NULL;
    LidarDataViewWindow *lidarView = NULL;

    void cleanRpLidarDriver();
    void cleanScanThread();

private slots:
    void connectToRplidar();
    void startMotor();
    void stopMotor();
    void startScan();
    void stopScan();
    void setSpeed(int speed);
};

#endif // MAINWINDOW_H
