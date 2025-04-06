#ifndef WIDGET_H
#define WIDGET_H
#include "sensornetwork.h"
#include <QWidget>
#include <QTimer>
#include <QMessageBox>
#include <QMouseEvent>


QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:

    SensorNetwork* sensorNetwork;
    QTimer* timer;
    int sensorCount;
    double broadcastRadius;
    int T ;
    int T2;
    bool Judge;
    QTimer *timer1;
    QTimer *timer2;
    int lastClickedSensorIndex = -1;
    int nowNode;


    Widget(QWidget *parent = nullptr);
    ~Widget();

    void paintEvent(QPaintEvent *event) override;
    void collect();
    void Send();
    void mousePressEvent(QMouseEvent *event) override;
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
