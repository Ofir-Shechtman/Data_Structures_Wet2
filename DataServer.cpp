#include "DataServer.h"

DataServer::DataServer(int n): data_centers(UnionFind<DataCenter>(n)),
                               traffic_meter(TrafficMeter()) {}

void DataServer::MergeDataCenters(DataCenterID dataCenter1, DataCenterID dataCenter2) {
    data_centers.Union(dataCenter1,dataCenter2);
}

void DataServer::AddServer(DataCenterID dataCenterID, ServerID serverID) {
    DataCenter dc = data_centers.Find(dataCenterID);
    dc.add_server(serverID);
    servers.insert(serverID);
}

void DataServer::RemoveServer(ServerID serverID) {
    DataCenterID dataCenterID = servers.find(serverID);
    DataCenter dc = data_centers.Find(dataCenterID);
    dc.remove_server(serverID);
    traffic_meter.remove_server(serverID);
    servers.erase(serverID);
}

void DataServer::SetTraffic(ServerID serverID, int traffic) {
    if(!servers.contains(serverID)) throw ServerIDNotExists();
    traffic_meter.set_traffic(serverID, traffic);
    DataCenterID dataCenterID = servers.find(serverID);
    DataCenter dc = data_centers.Find(dataCenterID);
    dc.set_traffic(serverID,traffic);
}

int DataServer::SumHighestTrafficServers(DataCenterID dataCenterID, int k) {
    Array<int> a(k);
    if(dataCenterID == 0)
        return traffic_meter.sum_highest_traffic_servers(k);
    DataCenter dc = data_centers.Find(dataCenterID);
    return dc.sum_highest_traffic_servers(k);
}