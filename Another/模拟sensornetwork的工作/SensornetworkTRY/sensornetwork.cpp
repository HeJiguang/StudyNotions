#include "sensornetwork.h"
#include <QDebug>
#include <random>
#include <cmath>


// 计算欧几里得距离的函数
double euclideanDistance(double x1, double y1, double x2, double y2) {
    return std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}


SensorNetwork::SensorNetwork(int sensorCount, double broadcastRadius, int T) {
    this->sensorCount = sensorCount;
    this->broadcastRadius = broadcastRadius;
    this->T = T;
    multiple = 1;
    dataCollector = new DataCollector(); // 初始化DataCollector
}


void SensorNetwork::initializeNetwork()
{
    int maxX = 1210;
    int minX = 20;
    int maxY = 540;
    int minY = 10;
    int d = 30;

    std::random_device rd;  // 用于生成随机种子
    std::mt19937 gen(rd());  // Mersenne Twister 随机数生成器
    sensors.clear(); // 清空当前的传感器列表（如果有）
    std::uniform_int_distribution<> dis1(minX, maxX);
    std::uniform_int_distribution<> dis2(minY, maxY);

    for (int i = 0; i < sensorCount; ++i) {
        int x = dis1(gen);
        int y = dis2(gen);
        SensorNode *sensor = new SensorNode(i+1 ,x, y, broadcastRadius);
        qDebug() << i << ";" << x << ";" << y ;
        sensors.append(sensor);
    }

    int D_x = 20;
    int D_y = 20;
    // 创建在 1 到 4 之间生成均匀分布的整数的分布对象
    std::uniform_int_distribution<> dis3(1, 4);
    int judgeD = dis3(gen);
    if(judgeD == 1){
        std::uniform_int_distribution<> dis11(minX, maxX);
        std::uniform_int_distribution<> dis22(minY, minY + d);
        D_x = dis11(gen);
        D_y = dis22(gen);
    }
    else if(judgeD == 2){
        std::uniform_int_distribution<> dis11(minX, minX + 30);
        std::uniform_int_distribution<> dis22(minY + d, maxY - d);
        D_x = dis11(gen);
        D_y = dis22(gen);
    }
    else if(judgeD == 3){
        std::uniform_int_distribution<> dis11(maxX - d, maxX);
        std::uniform_int_distribution<> dis22(minY + d, maxY - d);
        D_x = dis11(gen);
        D_y = dis22(gen);
    }
    else if(judgeD == 4){
        std::uniform_int_distribution<> dis11(minX, maxX);
        std::uniform_int_distribution<> dis22(maxY - d, maxY);
        D_x = dis11(gen);
        D_y = dis22(gen);
    }
    qDebug() << "DXY" << ";" << D_x << ";" << D_y ;
    dataCollector->x = D_x;
    dataCollector->y = D_y;

    //将小于半径的两个节点相连接
    for (int i = 0; i < sensorCount; ++i) {
        for (int j = i + 1; j < sensorCount; ++j) {
            double distance = euclideanDistance(sensors[i]->x, sensors[i]->y, sensors[j]->x, sensors[j]->y);
            if (distance <= broadcastRadius) {
                graph.addEdge(i, j, distance);
                //for (Edge& edge : graph.adjList.at(i)) {
                //   edge.weight = edge.desister  + multiple * sensors[j]->dataQueue.size();
                //    qDebug() << "距离"  << edge.desister << "边" << edge.weight;
                //}
            }
        }
        double distanceToDC = euclideanDistance(sensors[i]->x, sensors[i]->y, dataCollector->x, dataCollector->y);
        if (distanceToDC <= broadcastRadius) {
            graph.addEdge(i, sensorCount, distanceToDC);  // 连接传感器和收集节点
            //Edge a = graph.findEdge(sensorCount,i);
            //qDebug() << sensorCount << " " << i << ";" << a.to << " " << a.weight << "*****************************************" ;
        }

    }

}

void SensorNetwork::Dijkstra() {
    // 初始化路径表
    for (int i = 0; i <= sensorCount; i++) {
        PathMap[i].Set(false, std::numeric_limits<double>::infinity(), -1); // 将距离设为无穷大
    }

    // 放入第一个节点
    PathMap[sensorCount].Set(true, 0, -1);

    // 初始化第一个节点的邻接节点的距离
    for (const Edge& edge : graph.adjList.at(sensorCount)) {
        PathMap[edge.to].Set(false, edge.weight, sensorCount);
    }

    for (int i = 0; i < sensorCount; i++) {
        // 找到未处理节点中距离最短的节点
        int minNode = -1;
        double minDist = std::numeric_limits<double>::infinity();

        for (const auto& entry : PathMap) {
            if (!entry.second.final && entry.second.dist < minDist) {
                minDist = entry.second.dist;
                minNode = entry.first;
            }
        }

        //if (minNode == -1) {
        //    break; // 所有节点已处理完毕
        //}

        PathMap[minNode].final = true; // 将该节点标记为已处理

        // 更新该节点邻接节点的距离
        for (const Edge& edge : graph.adjList[minNode]) {
            if (!PathMap[edge.to].final && PathMap[minNode].dist + edge.weight < PathMap[edge.to].dist) {
                PathMap[edge.to].Set(false, PathMap[minNode].dist + edge.weight, minNode);
            }
        }
    }
}


void Graph::addEdge(int from, int to, double weight) {
    adjList[from].push_back({to, weight});
    adjList[to].push_back({from, weight}); // 无向图需要双向边
}


Edge Graph::findEdge(int from, int to) {
    if (adjList.find(from) != adjList.end()) {
        for (const Edge& edge : adjList.at(from)) {
            if (edge.to == to) {
                return edge;
            }
        }
    }
    return Edge();
}

void SensorNetwork::printPathMap() {
    for (const auto& entry : PathMap) {
        int node = entry.first;
        const Data& data = entry.second;
        qDebug() << "Node:" << node
                 << "Final:" << data.final
                 << "Distance:" << data.dist
                 << "Previous:" << data.path;
    }
}

void SensorNetwork::initEdgeType(){
    for (const auto& entry : PathMap) {
        int node = entry.first; //节点序号
        const Data& data = entry.second; //节点数据
        auto path = data.path;
        auto D = data.dist;

        if(D != std::numeric_limits<double>::infinity()){
            // graph.findEdge(node,path).Set(2);

            for (Edge& edge : graph.adjList.at(node)) {
                if (edge.to == path) {
                    edge.Set(2);
                }
                else{
                    edge.Set(1);
                }
            }
        }
        if(D == 0){
            qDebug() << "*****************************************";
            for (Edge& edge : graph.adjList.at(sensorCount)) {
                edge.Set(2);
            }
        }
    }
    for(int i = 0;i < sensorCount;i++){
        sensors[i]->SetType(1);
    }
}


// 修改 SensorNetwork 类中的 getShortestPath 函数
QPair<double, QList<int>> SensorNetwork::getShortestPath(int targetNode) {
    QList<int> path;
    double totalDistance = PathMap[targetNode].dist;

    // 检查目标节点是否可达
    if (totalDistance == std::numeric_limits<double>::infinity()) {
        return qMakePair(totalDistance, path); // 返回空路径和无穷距离
    }

    // 使用堆栈逆序构建路径
    QStack<int> stack;
    int currentNode = targetNode;

    while (currentNode != -1) {
        stack.push(currentNode);
        currentNode = PathMap[currentNode].path;
    }

    // 将堆栈中的路径顺序放入 QList 中
    while (!stack.isEmpty()) {
        path.append(stack.pop());
    }

    return qMakePair(totalDistance, path);
}


void SensorNetwork::StartT(){
    for(int i = 0; i < sensorCount; i++){
        int per = PathMap[i].path; // 路径中前一个节点的位置
        if (!sensors[i]->dataQueue.isEmpty()) {
            auto Data = sensors[i]->dataQueue.first();
            sensors[i]->dataQueue.dequeue();
            sensors[i]->model->removeRow(0);
            if(per == sensorCount){
                dataCollector->addItem(Data);
                //GetNewD(i,per);
                // qDebug() << "向数据收集站";
            } else {
                //GetNewD(i,per);
                sensors[per]->receiveData(Data);
                // qDebug() << "普通节点传送消息";
            }
        } else {
            // 数据队列为空时的处理
        }
    }
}


// 控制节点生成数据
void SensorNetwork::GetNew(){
    for(int i = 0;i < sensorCount;i++){
        sensors[i]->collectData();
    }
}

void SensorNetwork::GetNewD(int from,int to){
    for (Edge& edge : graph.adjList.at(from)) {
        if (edge.to == to) {
            edge.weight = edge.desister  + multiple * sensors[to]->dataQueue.size();
            qDebug() << "距离"  << edge.desister << "边" << edge.weight;
        }
    }
}


