#include "sensornode.h"

SensorNode::SensorNode(int _a ,double _x, double _y, double _broadcastRadius) {
    x = _x;
    y = _y;
    a = _a;
    type = 1;
    broadcastRadius = _broadcastRadius;
    model = new QStandardItemModel();
}


void SensorNode::SetType(int a){
    type = a;
}

void SensorNode::collectData(){
    QDateTime tDateTime = QDateTime::currentDateTime();
    std::random_device rd;  // 用于生成随机种子
    std::mt19937 gen(rd());  // Mersenne Twister 随机数生成器
    //产生温度的随机数
    std::uniform_int_distribution<> dis1(0, 100);
    int T = dis1(gen);
    TData A = TData(a,T,"温度");
    A.currentDateTime = tDateTime;
    dataQueue.enqueue(A); //收集到一个温度数据
    std::uniform_int_distribution<> dis2(0, 100);
    int S = dis2(gen);
    TData B = TData(a,S,"湿度");
    B.currentDateTime = tDateTime;
    dataQueue.enqueue(B); //收集到一个湿度数据
    model->appendRow(new QStandardItem(A.toString()));
    model->appendRow(new QStandardItem(B.toString()));
}


void SensorNode::receiveData(const TData &data){
    dataQueue.enqueue(data);
    model->appendRow(new QStandardItem(data.toString()));
}
