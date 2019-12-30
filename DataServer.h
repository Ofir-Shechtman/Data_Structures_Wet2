#ifndef DATA_STRUCTURES_WET2_DATASERVER_H
#define DATA_STRUCTURES_WET2_DATASERVER_H

#include <iostream>
#include "UnionFind.h"
#include "DataCenter.h"
#include "TrafficMeter.h"

using namespace std;
typedef unsigned int DataCenterID;

class DataServer {
    UnionFind<DataCenter> data_centers;
    HashTable<ServerID, DataCenterID> servers;
    TrafficMeter traffic_meter;
public:
    explicit DataServer(int n);
    ~DataServer() = default;
    void MergeDataCenters(DataCenterID dataCenter1, DataCenterID dataCenter2);
    void AddServer(DataCenterID dataCenterID, DataCenterID serverID);
    void RemoveServer(ServerID serverID);
    void SetTraffic(ServerID serverID, int traffic);
    int SumHighestTrafficServers(DataCenterID dataCenterID, int k);
};







#endif //DATA_STRUCTURES_WET2_DATASERVER_H
