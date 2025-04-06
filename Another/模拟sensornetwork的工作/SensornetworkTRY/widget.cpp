#include "widget.h"
#include "ui_widget.h"
#include "sensornetwork.h"
#include <QTimer>
#include <QPainter>
#include <cmath>



// 计算欧几里得距离的函数
double euclideanDistance1(double x1, double y1, double x2, double y2) {
    return std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->label_I->setText("输入节点序号或点击节点");
    sensorCount = 100;
    broadcastRadius = 150;
    T = 5;
    nowNode = -1;
    T2 = 2;

    sensorNetwork = new SensorNetwork(sensorCount,broadcastRadius,T);

    sensorNetwork->initializeNetwork();


    ui->lineEdit_SNUM->setText(QString::number(sensorCount));
    ui->lineEdit_R->setText(QString::number(broadcastRadius));
    ui->lineEdit_T->setText(QString::number(T));
    ui->lineEdit_SNUM_2->setText(QString::number(T2));
    ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat); // 设置显示样式
    // 设置样式表使数字显示为蓝色
    ui->lcdNumber->setStyleSheet("color: blue;");

    ui->tableView->setModel(sensorNetwork->dataCollector->model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->resizeRowsToContents();
    // 设置定时器更新绘图
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&Widget::update));
    //connect(timer, &QTimer::timeout, this, &Widget::Send); //每秒发送一次数据
    timer->start(1000); // 每秒更新一次

    //设置定时器收集数据
    timer1 = new QTimer(this);
    connect(timer1, &QTimer::timeout, this, &Widget::collect);
    timer1->start(T*1000); // 每T秒更新一次

    timer2 = new QTimer(this);
    connect(timer2, &QTimer::timeout, this, &Widget::Send);
    timer2->start(T2*1000); // 每T2秒发送一次


    connect(ui->pushButton_2, &QPushButton::clicked, this, &Widget::on_pushButton_2_clicked);

    sensorNetwork->Dijkstra();

    sensorNetwork->printPathMap();

    sensorNetwork->initEdgeType();


}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *event)
{



    QPainter painter(this);
    // 设置画笔颜色 
    // 定义要绘制图片的区域和位置
    QRect targetRect(20, 10, 1200, 540); // 目标区域的位置和大小
    QPixmap pixmap(":/new/prefix1/pic/DiTu.png"); // 加载图片资源

    // 在指定区域内绘制图片
    painter.drawPixmap(targetRect, pixmap);

    // 设置画笔颜色和粗细
    QPen pen(Qt::yellow);
    pen.setWidth(1);
    painter.setPen(pen);

    QPen Tpen(Qt::blue);
    Tpen.setWidth(3);

    QPen Tpen1(Qt::red);
    Tpen1.setWidth(4);

    //**********************************************
    // 绘制符合条件的边
    for (int i = 0; i < sensorNetwork->sensors.size(); ++i) {
        for (const Edge& edge : sensorNetwork->graph.adjList[i]) {
            if (edge.to != sensorCount) {
                QPointF p1(sensorNetwork->sensors[i]->x, sensorNetwork->sensors[i]->y);
                QPointF p2(sensorNetwork->sensors[edge.to]->x, sensorNetwork->sensors[edge.to]->y);
                if(edge.type == 1){
                    painter.drawLine(p1, p2);
                }

            }
        }
    }

    for (const Edge& edge : sensorNetwork->graph.adjList.at(sensorCount)) {
        QPointF p1(sensorNetwork->dataCollector->x, sensorNetwork->dataCollector->y);
        QPointF p2(sensorNetwork->sensors[edge.to]->x, sensorNetwork->sensors[edge.to]->y);
        // qDebug() << sensorNetwork->dataCollector->x << "  " << sensorNetwork->dataCollector->y << "  ";
        // qDebug() << sensorNetwork->sensors[edge.to]->x << "  " << sensorNetwork->sensors[edge.to]->y << "  ";
        if(edge.type == 1){
            painter.drawLine(p1, p2);
        }
    }
    //*********************************************
    for (const Edge& edge : sensorNetwork->graph.adjList.at(sensorCount)) {
        QPointF p1(sensorNetwork->dataCollector->x, sensorNetwork->dataCollector->y);
        QPointF p2(sensorNetwork->sensors[edge.to]->x, sensorNetwork->sensors[edge.to]->y);
        // qDebug() << sensorNetwork->dataCollector->x << "  " << sensorNetwork->dataCollector->y << "  ";
        // qDebug() << sensorNetwork->sensors[edge.to]->x << "  " << sensorNetwork->sensors[edge.to]->y << "  ";
        if(edge.type == 2){

            painter.setPen(Tpen);

            painter.drawLine(p1, p2);

            painter.setPen(pen);
        }
    }
    for (int i = 0; i < sensorNetwork->sensors.size(); ++i) {
        for (const Edge& edge : sensorNetwork->graph.adjList[i]) {
            if (edge.to != sensorCount) { // Avoid drawing to DataCollector here
                QPointF p1(sensorNetwork->sensors[i]->x, sensorNetwork->sensors[i]->y);
                QPointF p2(sensorNetwork->sensors[edge.to]->x, sensorNetwork->sensors[edge.to]->y);
                if(edge.type == 2){
                    painter.setPen(Tpen);
                    painter.drawLine(p1, p2);
                    painter.setPen(pen);
                }

            }
        }
    }
    //**********************************************

    for (const Edge& edge : sensorNetwork->graph.adjList.at(sensorCount)) {
        QPointF p1(sensorNetwork->dataCollector->x, sensorNetwork->dataCollector->y);
        QPointF p2(sensorNetwork->sensors[edge.to]->x, sensorNetwork->sensors[edge.to]->y);
        // qDebug() << sensorNetwork->dataCollector->x << "  " << sensorNetwork->dataCollector->y << "  ";
        // qDebug() << sensorNetwork->sensors[edge.to]->x << "  " << sensorNetwork->sensors[edge.to]->y << "  ";
        if(edge.type == 3){

            painter.setPen(Tpen1);

            painter.drawLine(p1, p2);

            painter.setPen(pen);
        }
    }
    for (int i = 0; i < sensorNetwork->sensors.size(); ++i) {
        for (const Edge& edge : sensorNetwork->graph.adjList[i]) {
            if (edge.to != sensorCount) { // Avoid drawing to DataCollector here
                QPointF p1(sensorNetwork->sensors[i]->x, sensorNetwork->sensors[i]->y);
                QPointF p2(sensorNetwork->sensors[edge.to]->x, sensorNetwork->sensors[edge.to]->y);
                if(edge.type == 3){
                    painter.setPen(Tpen1);
                    painter.drawLine(p1, p2);
                    painter.setPen(pen);
                }

            }
        }
    }



    painter.setPen(Qt::black);

    painter.setBrush(QColor(66, 196, 230));  // 设置刷子颜色
    //painter.drawRect(QRectF(1210, 540, 10, 10));
    // 绘制传感器节点
    for (SensorNode *sensor : sensorNetwork->sensors) {
        if(sensor->type == 1){
            painter.drawEllipse(QPointF(sensor->x, sensor->y), 6, 6); // 画一个半径为6的圆形
        }
        else if(sensor->type == 2){
            painter.setBrush(Qt::red);  // 设置刷子颜色
            painter.drawEllipse(QPointF(sensor->x, sensor->y), 8, 8); // 画一个半径为6的圆形
            painter.setBrush(QColor(66, 196, 230));  // 设置刷子颜色
        }

    }

    // 设置刷子颜色
    painter.setBrush(QColor(255, 255, 153));
    //绘制DataCollector
    painter.drawRect(QRectF(sensorNetwork->dataCollector->x, sensorNetwork->dataCollector->y, 20, 20)); // 画一个矩形

    // 设置文本颜色和字体
    painter.setPen(Qt::black);
    QFont font = painter.font();
    font.setPointSize(10); // 设置字体大小
    painter.setFont(font);

    // 在矩形内绘制文本
    QRect rect(sensorNetwork->dataCollector->x, sensorNetwork->dataCollector->y, 20, 20); // 矩形的位置和大小
    painter.drawText(rect, Qt::AlignCenter, "DC");

    ui->tableView->setModel(sensorNetwork->dataCollector->model);
    if(nowNode != -1){
        int value = sensorNetwork->sensors[nowNode]->dataQueue.size();
        ui->lcdNumber->display(value);
    }
}



void Widget::on_pushButton_clicked()
{
    QString DNUM = ui->lineEdit_SNUM->text();
    bool ok;
    int DNUMnumber = DNUM.toInt(&ok);
    if (ok) {
        qDebug() << "更改节点数: " << DNUMnumber;
        this->sensorCount = DNUMnumber;
    } else {
        QMessageBox::warning(this, "Error", "请确保传感器数量输入的是数字");
    }


    QString R = ui->lineEdit_R->text();
    bool ok1;
    int Rnumber = R.toDouble(&ok1);
    if (ok1) {
        qDebug() << "更改半径: " << Rnumber;
        this->broadcastRadius = Rnumber;
    } else {
        QMessageBox::warning(this, "Error", "请确保半径输入的是数字");
    }


    QString T321= ui->lineEdit_T->text();
    bool ok2;
    int Tnumber = T321.toInt(&ok2);
    if (ok2) {
        qDebug() << "更改周期: " << Tnumber;
        this->T = Tnumber;
    } else {
        QMessageBox::warning(this, "Error", "请确保周期输入的是数字");
    }

    QString T123= ui->lineEdit_SNUM_2->text();
    bool ok3;
    int Tnumber2 = T123.toInt(&ok3);
    if (ok3) {
        qDebug() << "更改发送周期: " << Tnumber2;
        this->T2 = Tnumber2;
    } else {
        QMessageBox::warning(this, "Error", "请确保周期输入的是数字");
    }

    delete this->sensorNetwork;
    SensorNetwork* NewS = new SensorNetwork(this->sensorCount,this->broadcastRadius,this->T);
    this->sensorNetwork = NewS;
    sensorNetwork->initializeNetwork();

    sensorNetwork->Dijkstra();
    sensorNetwork->printPathMap();
    sensorNetwork->initEdgeType();

    //设置定时器收集数据
    timer1->deleteLater();
    timer2->deleteLater();
    timer1 = new QTimer(this);
    connect(timer1, &QTimer::timeout, this, &Widget::collect);
    timer1->start(T*1000); // 每T秒更新一次

    timer2 = new QTimer(this);
    connect(timer2, &QTimer::timeout, this, &Widget::Send);
    timer2->start(T2*1000); // 每T2秒发送一次
}


void Widget::on_pushButton_2_clicked()
{
    sensorNetwork->initEdgeType();
    // 在这里添加你的处理代码
    // qDebug() << "PushButton 2 clicked!";
    QString DNUM = ui->lineEdit_4->text();
    bool ok;
    int DNUMnumber = DNUM.toInt(&ok);
    if (ok) {
        qDebug() << "查找节点: " << DNUMnumber;
        if(DNUMnumber >= sensorCount){
            QMessageBox::warning(this, "Error", "请确保输入正确的节点查找数");
            return;
        }
        //查找最短路径
        auto result = sensorNetwork->getShortestPath(DNUMnumber);

        double distance = result.first;
        QString message = "到DataCollector的通道距离 :" + QString::number(distance);
        nowNode = DNUMnumber;
        ui->label_I->setText(message);
        QList<int> path = result.second;
        // qDebug() << "Shortest distance to node" << DNUMnumber << ":" << distance;
        // qDebug() << "Path:";
        int last = -1;
        int now = path.at(0);
        //int value = sensorNetwork->sensors[DNUMnumber]->dataQueue.size();
        //ui->lcdNumber->display(value);
        for (int i = 1; i < path.size(); ++i) {
            // qDebug() << node;
            last = now;
            now = path.at(i);
            for (Edge& edge : sensorNetwork->graph.adjList.at(now)) {
                if (edge.to == last) {
                    edge.Set(3);
                }
            }
            for (Edge& edge : sensorNetwork->graph.adjList.at(last)) {
                if (edge.to == now) {
                    edge.Set(3);
                }
            }
        }
        sensorNetwork->sensors[now]->SetType(2); // 将传感器节点设置为第二类显示点

        ui->listView->setModel(sensorNetwork->sensors[now]->model);
    } else {
        QMessageBox::warning(this, "Error", "请确保输入正确的节点查找数");
    }

}

void Widget::collect(){
    sensorNetwork->GetNew();
}

void Widget::Send(){
    sensorNetwork->StartT();
    ui->verticalSlider->setRange(0, 100);
    int sliderValue = ui->verticalSlider->value();
    qDebug() << "Path:" << sliderValue;
    sensorNetwork->multiple = sliderValue;
    sensorNetwork->Dijkstra();
}


void Widget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // 获取鼠标点击的位置
        QPointF clickPos = event->position();

        // 遍历传感器节点，检查是否点击到了某个传感器节点
        for (int i = 0; i < sensorNetwork->sensors.size(); ++i) {
            SensorNode *sensor = sensorNetwork->sensors[i];
            QPointF sensorPos(sensor->x, sensor->y);

            // 计算点击位置与传感器节点位置的距离
            double distance = euclideanDistance1(clickPos.x(), clickPos.y(), sensorPos.x(), sensorPos.y());

            // 如果距离小于传感器节点半径（这里假设传感器节点半径为固定值
            if (distance < 7) {
                ui->lineEdit_4->setText(QString::number(i));
                sensorNetwork->initEdgeType();
                // 将点击的传感器节点设置为第二类（type为2）
                //查找最短路径
                auto result = sensorNetwork->getShortestPath(i);

                double distance = result.first;
                QString message = "到DataCollector的通道距离 :" + QString::number(distance);
                ui->label_I->setText(message);
                nowNode = i;
                // int value = sensorNetwork->sensors[i]->dataQueue.size();
                // ui->lcdNumber->display(value);
                // QMessageBox::information(this,"查找成功", message);
                QList<int> path = result.second;
                // qDebug() << "Shortest distance to node" << DNUMnumber << ":" << distance;
                // qDebug() << "Path:";
                int last = -1;
                int now = path.at(0);

                for (int i = 1; i < path.size(); ++i) {
                    // qDebug() << node;
                    last = now;
                    now = path.at(i);
                    for (Edge& edge : sensorNetwork->graph.adjList.at(now)) {
                        if (edge.to == last) {
                            edge.Set(3);
                        }
                    }
                    for (Edge& edge : sensorNetwork->graph.adjList.at(last)) {
                        if (edge.to == now) {
                            edge.Set(3);
                        }
                    }
                }
                sensorNetwork->sensors[now]->SetType(2); // 将传感器节点设置为第二类显示点

                ui->listView->setModel(sensorNetwork->sensors[now]->model);

                // 将最后一次点击的传感器节点索引记录下来
                lastClickedSensorIndex = i;

                break; // 找到了点击的传感器节点，退出循环
            }
        }
    }
}
