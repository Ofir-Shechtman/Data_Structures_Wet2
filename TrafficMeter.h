#ifndef DATA_STRUCTURES_WET2_TRAFFICMETER_H
#define DATA_STRUCTURES_WET2_TRAFFICMETER_H

#include "AVLTree.h"
typedef unsigned int Traffic;
typedef unsigned int ServerID;


template<class Traffic>
class CompareTraffic : public Compare<Traffic>{
public:
    bool operator()(const Traffic& a, const Traffic& b) const;
    Compare<Traffic>* clone();
};


template<class ServerID>
class CompareServer : public Compare<ServerID>{
public:
    bool operator()(const ServerID& a, const ServerID& b) const;
    Compare<Traffic>* clone();
};


class TrafficMeter {
    AVLTree<Traffic,int> traffic_tree;
    AVLTree<ServerID, AVLTree<Traffic,int>::Iterator> map_tree;
public:
    TrafficMeter()= default;
    ~TrafficMeter()= default;
    TrafficMeter(const TrafficMeter&)= default;
    TrafficMeter(const TrafficMeter& m1, const TrafficMeter& m2);
    TrafficMeter& operator=(const TrafficMeter&)= default;
    void remove_server(ServerID);
    void set_traffic(ServerID, Traffic);
    int sum_highest_traffic_servers(int k);


};


#endif //DATA_STRUCTURES_WET2_TRAFFICMETER_H
