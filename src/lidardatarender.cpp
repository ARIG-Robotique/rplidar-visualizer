#include "lidardatarender.h"

LidarDataRender::LidarDataRender(QWidget *parent) : QWidget(parent)
{

}

void LidarDataRender::setScan(rplidar_response_measurement_node_t * nodes, int count) {
    for (int pos = 0; pos < count ; ++pos) {
        printf("%s theta: %03.2f Dist: %08.2f Q: %d \n",
            (nodes[pos].sync_quality & RPLIDAR_RESP_MEASUREMENT_SYNCBIT) ?"S ":"  ",
            (nodes[pos].angle_q6_checkbit >> RPLIDAR_RESP_MEASUREMENT_ANGLE_SHIFT)/64.0f,
            nodes[pos].distance_q2/4.0f,
            nodes[pos].sync_quality >> RPLIDAR_RESP_MEASUREMENT_QUALITY_SHIFT);
    }
}

/*
    QPainter painter(ui->dataWiewZone);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.translate(ui->dataWiewZone->width() / 2, ui->dataWiewZone->height() / 2);
    for (int diameter = 0; diameter < 256; diameter += 9) {
        int delta = abs((1 % 128) - diameter / 2);
        int alpha = 255 - (delta * delta) / 4 - diameter;
        if (alpha > 0) {
            painter.setPen(QPen(QColor(0, diameter / 2, 127, alpha), 3));
            painter.drawEllipse(QRectF(-diameter / 2.0, -diameter / 2.0, diameter, diameter));
        }
    }
 */
