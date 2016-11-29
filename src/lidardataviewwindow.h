#ifndef LIDARDATAVIEWWINDOW_H
#define LIDARDATAVIEWWINDOW_H

#include <QWidget>
#include <rplidar.h>

class LidarDataViewWindow : public QWidget
{
    Q_OBJECT
public:
    explicit LidarDataViewWindow(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event);

private:
    rplidar_response_measurement_node_t * nodes;
    int countData = 0;

private slots:
    void setScan(rplidar_response_measurement_node_t * nodes, int count);

};

#endif // LIDARDATAVIEWWINDOW_H
