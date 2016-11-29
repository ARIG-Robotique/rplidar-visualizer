#ifndef SCANTHREAD_H
#define SCANTHREAD_H

#include <QThread>
#include <rplidar.h>

using namespace rp::standalone::rplidar;

class ScanThread : public QThread
{
    Q_OBJECT

public:
    ScanThread(QObject *parent);

    void setDriver(RPlidarDriver *drv);
    void abortExecution();

signals:
    void dataGrabed(rplidar_response_measurement_node_t * nodes, int count);

protected:
    void run() Q_DECL_OVERRIDE;

private:
    bool abort;
    RPlidarDriver *drv;

};

#endif // SCANTHREAD_H
