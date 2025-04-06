#ifndef SENSORNETWORK_H
#define SENSORNETWORK_H
#include <QList>
#include "SensorNode.h"
#include "DataCollector.h"
#include <QStack>


// 结构定义图中的边
struct Edge {
    int to;
    double weight;
    int type;
    double desister;

    Edge() : to(-1), weight(0),type(1){}
    Edge(int t, double w){
        to = t;
        weight = w;
        type = 1;
        desister = weight;
    }
    void Set(int a){
        type = a;
    }
};


// 图类
class Graph {
public:
    std::unordered_map<int, std::vector<Edge>> adjList;

    // 添加边的方法
    void addEdge(int from, int to, double weight);

    // 查找边的方法
    Edge findEdge(int from, int to);
};

// 结构定义迪杰斯特拉算法的储存
struct Data {
    bool final; //是否找到最短路径
    double dist; //最短路径长度
    int path; //路径上的前驱

    void Set(bool f, double d, int p){
        final = f;
        dist = d;
        path = p;
    }
};

// 管理整个传感器网络，包含传感器和数据收集器
class SensorNetwork
{
public:
    QList<SensorNode*> sensors;  // 节点数组
    DataCollector *dataCollector;  // 收集器
    int sensorCount;
    double broadcastRadius;
    int T;
    Graph graph;
    int multiple;
    std::unordered_map<int, Data> PathMap;  //路径地图

    SensorNetwork(int sensorCount, double broadcastRadius, int T);
    QPair<double, QList<int>> getShortestPath(int targetNode);
    void printPathMap();
    void initializeNetwork();
    void Dijkstra();
    void initEdgeType();
    void StartT();
    void GetNew();
    void GetNewD(int A,int B);
};



#endif // SENSORNETWORK_H
