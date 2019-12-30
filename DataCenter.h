#ifndef DATA_STRUCTURES_WET2_DATACENTER_H
#define DATA_STRUCTURES_WET2_DATACENTER_H

#include "HashTable.h"
#include "TrafficMeter.h"

class DataCenter {
    HashTable<ServerID, int> servers;
    TrafficMeter traffic_meter;
public:
    DataCenter()= default;
    ~DataCenter()= default;
    DataCenter(const DataCenter&)= default;
    DataCenter(const DataCenter& dc1, const DataCenter& dc2);
    DataCenter& operator=(const DataCenter&)= default;
    void add_server(ServerID);
    void remove_server(ServerID);
    void set_traffic(ServerID, Traffic);
    int sum_highest_traffic_servers(int k);
    class ServerIDNotExists : public std::exception{};

};


#endif //DATA_STRUCTURES_WET2_DATACENTER_H
