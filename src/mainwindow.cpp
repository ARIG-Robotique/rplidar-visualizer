#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->statusbar->addWidget(new QLabel("RPLIDAR SDK VERSION : " RPLIDAR_SDK_VERSION));

    // Initialisation des valeurs PWM
    ui->pwmValue->setMaximum(MAX_MOTOR_PWM);

    // Fermeture de l'application lors du click sur quitter
    connect(ui->actionQuitter, SIGNAL(triggered()), qApp, SLOT(quit()));

    // RPLIDAR
    connect(ui->connectRplidarButton, SIGNAL(clicked()), this, SLOT(connectToRplidar()));
    connect(ui->startMotorButton, SIGNAL(clicked()), this, SLOT(startMotor()));
    connect(ui->stopMotorButton, SIGNAL(clicked()), this, SLOT(stopMotor()));
    connect(ui->startScanButton, SIGNAL(clicked()), this, SLOT(startScan()));
    connect(ui->stopScanButton, SIGNAL(clicked()), this, SLOT(stopScan()));
    connect(ui->pwmValue, SIGNAL(valueChanged(int)), this, SLOT(setSpeed(int)));
}

MainWindow::~MainWindow()
{
    cleanScanThread();
    cleanRpLidarDriver();
    delete ui;
}

void MainWindow::connectToRplidar() {
    cleanRpLidarDriver();
    drv = RPlidarDriver::CreateDriver(RPlidarDriver::DRIVER_TYPE_SERIALPORT);
    if (drv == NULL) {
        QMessageBox::critical(this, "Driver RPLIDAR - create", "Impossible d'initialisé le driver.");
        return;
    }

    const char * devicePort = ui->devicePort->text().toStdString().c_str();
    if (IS_FAIL(drv->connect(devicePort, 115200))) {
        QMessageBox::critical(this, "Driver RPLIDAR - connect", "Impossible de se connecter sur le port " + ui->devicePort->text());
        return;
    }

    rplidar_response_device_info_t deviceInfo;
    if (IS_FAIL(drv->getDeviceInfo(deviceInfo))) {
        QMessageBox::critical(this, "Driver RPLIDAR - info", "Impossible de recuperer les infos");
        return;
    }

    QString serialNum;
    for (int i = 0 ; i < 16 ; i++) {
        _u8 v = deviceInfo.serialnum[i];
        if (v < 10) {
            serialNum.append("0");
        }
        serialNum.append(QString::number(v, 16));
    }
    ui->serialNumberLabelValue->setText(serialNum.toUpper());
    ui->hardwareRevLabelValue->setText(QString::number(deviceInfo.hardware_version));

    QString firmwareVer;
    firmwareVer.sprintf("%d.%02d", deviceInfo.firmware_version >> 8, deviceInfo.firmware_version & 0xFF);
    ui->firmwareVersionLabelValue->setText(firmwareVer.toUpper());


    rplidar_response_device_health_t healthinfo;
    if (IS_FAIL(drv->getHealth(healthinfo))) {
        QMessageBox::critical(this, "Driver RPLIDAR - health", "erreur de status");
        return;
    }

    if (healthinfo.status == RPLIDAR_STATUS_ERROR) {
        ui->healthLabelValue->setText("ERROR");
    } else if (healthinfo.status == RPLIDAR_STATUS_WARNING) {
        ui->healthLabelValue->setText("WARNING");
    } else {
        ui->healthLabelValue->setText("OK");
    }
}

void MainWindow::startMotor() {
    if (drv != NULL) {
        drv->startMotor();
        ui->pwmValue->setValue(DEFAULT_MOTOR_PWM);
    }
}

void MainWindow::stopMotor() {
    if (drv != NULL) {
        drv->stopMotor();
        ui->pwmValue->setValue(0);
    }
}

void MainWindow::startScan() {
    if (drv != NULL) {
        drv->startScan();
        if (scanThread == NULL) {
            scanThread = new ScanThread(this);
            scanThread->setDriver(drv);
        }
        if (lidarView == NULL) {
            lidarView = new LidarDataViewWindow();
        }
        connect(scanThread, SIGNAL(dataGrabed(rplidar_response_measurement_node_t *,int)), lidarView, SLOT(setScan(rplidar_response_measurement_node_t *,int)));
        scanThread->start();
        lidarView->show();
    }
}

void MainWindow::stopScan() {
    if (drv != NULL) {
        drv->stop();
        cleanScanThread();
    }
}

void MainWindow::setSpeed(int speed) {
    if (drv != NULL) {
        drv->setMotorPWM(speed);
    }
}

void MainWindow::cleanRpLidarDriver() {
    if (drv != NULL) {
        drv->stop();
        drv->stopMotor();
        RPlidarDriver::DisposeDriver(drv);
    }
}

void MainWindow::cleanScanThread() {
    if (scanThread != NULL) {
        scanThread->abortExecution();
        disconnect(scanThread, SIGNAL(dataGrabed(rplidar_response_measurement_node_t,size_t)), this, SLOT(setScan(rplidar_response_measurement_node_t,size_t)));
    }
}
