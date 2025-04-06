#ifndef DATACOLLECTOR_H
#define DATACOLLECTOR_H
#include <QStandardItemModel>
#include "sensornode.h"

// 数据收集的类
class DataCollector
{
public:
    double x;
    double y;
    QStandardItemModel *model;

public:
    DataCollector();
    void addItem(TData data);
    void removeItem(TData data);
    ~DataCollector();
};

#endif // DATACOLLECTOR_H
