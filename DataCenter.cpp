//
// Created by ofir5 on 30/12/19.
//

#include "DataCenter.h"

void DataCenter::add_server(ServerID id) {
    servers.insert(id);
}

void DataCenter::remove_server(ServerID id) {
    servers.erase(id);
    traffic_meter.remove_server(id);

}

void DataCenter::set_traffic(ServerID id, Traffic traffic) {
    if(!servers.contains(id))
        throw ServerIDNotExists();
    traffic_meter.set_traffic(id, traffic);
}

int DataCenter::sum_highest_traffic_servers(int k) {
    return traffic_meter.sum_highest_traffic_servers(k);
}

DataCenter::DataCenter(const DataCenter &dc1, const DataCenter &dc2) :
    servers(dc1.servers, dc2.servers),
    traffic_meter(dc1.traffic_meter, dc2.traffic_meter){}
