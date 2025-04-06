#ifndef SENSORNODE_H
#define SENSORNODE_H
#include <QQueue>
#include <QPoint>
#include <random>
#include <QDateTime>
#include <QStandardItemModel>

struct TData{
    int fromNode;
    int data;
    QString type;
    QDateTime currentDateTime;
    TData(int a,int D,QString Type){
        fromNode = a;
        data = D;
        type = Type;
    }
    QString toString() const {
        return QString("From Node: %1, Data: %2, Type: %3, Date/Time: %4")
            .arg(fromNode)
            .arg(data)
            .arg(type)
            .arg(currentDateTime.toString(Qt::ISODate));
    }
};

// 传感器节点的类
class SensorNode
{
public:
    int a; //编号
    double x; // 传感器节点的位置
    double y;
    int type;
    double broadcastRadius; // 广播半径
    QQueue<TData> dataQueue; //收集数据
    QStandardItemModel *model;


public:
    SensorNode(int a ,double x, double y, double broadcastRadius);
    void receiveData(const TData &data); //接收数据
    void collectData(); //收集数据
    void SetType(int a);
};

#endif // SENSORNODE_H
