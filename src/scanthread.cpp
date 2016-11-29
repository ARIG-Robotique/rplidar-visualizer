#include "scanthread.h"

#ifndef _countof
#define _countof(_Array) (int)(sizeof(_Array) / sizeof(_Array[0]))
#endif

ScanThread::ScanThread(QObject *parent) : QThread(parent) {
    abort = false;
}

void ScanThread::setDriver(RPlidarDriver *drv) {
    this->drv = drv;
}

void ScanThread::abortExecution() {
    abort = true;
}

void ScanThread::run() {
    while (!abort) {
        rplidar_response_measurement_node_t nodes[360*2];
        size_t count = _countof(nodes);

        u_result op_result = drv->grabScanData(nodes, count);
        if (IS_OK(op_result)) {
            drv->ascendScanData(nodes, count);
            emit dataGrabed(nodes, (int) count);
        }
    }
}
