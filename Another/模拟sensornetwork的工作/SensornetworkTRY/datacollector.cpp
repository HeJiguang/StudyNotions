#include "datacollector.h"

DataCollector::DataCollector() : model(new QStandardItemModel()) {
    // 设置表头
    model->setHorizontalHeaderLabels({"节点序号", "数据", "类型","时间"});
}

// 添加数据项
void DataCollector::addItem(TData data) {
    QList<QStandardItem *> rowItems;
    rowItems.append(new QStandardItem(QString::number(data.fromNode)));
    rowItems.append(new QStandardItem(QString::number(data.data)));
    rowItems.append(new QStandardItem(data.type));
    rowItems.append(new QStandardItem(data.currentDateTime.toString(Qt::ISODate)));
    model->appendRow(rowItems);
}

// 删除数据项
void DataCollector::removeItem(TData data) {
    for (int row = 0; row < model->rowCount(); ++row) {
        if (model->item(row, 0)->text().toInt() == data.fromNode &&
            model->item(row, 1)->text().toInt() == data.data &&
            model->item(row, 2)->text() == data.type) {
            model->removeRow(row);
            break;
        }
    }
}

// 析构函数中删除 model
DataCollector::~DataCollector() {
    delete model;
}
