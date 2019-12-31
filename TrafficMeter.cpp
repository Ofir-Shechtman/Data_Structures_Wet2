//
// Created by ofir5 on 29/12/19.
//

#include "TrafficMeter.h"

template<class Traffic>
bool CompareTraffic<Traffic>::operator()(const Traffic &a, const Traffic &b) const {
    return a>=b;
}

template<class Traffic>
Compare<Traffic> *CompareTraffic<Traffic>::clone() const {
    return new CompareTraffic<Traffic>();
}

template<class ServerID>
bool CompareServer<ServerID>::operator()(const ServerID &a, const ServerID &b) const {
    return a<b;
}

template<class ServerID>
Compare<ServerID> *CompareServer<ServerID>::clone() const {
    return new CompareServer<ServerID>();
}

TrafficMeter::TrafficMeter() :
        traffic_tree(CompareTraffic<Traffic>()),
        map_tree(CompareServer<ServerID>())
{}

TrafficMeter::TrafficMeter(const TrafficMeter &m1, const TrafficMeter &m2):
        traffic_tree(m1.traffic_tree, m2.traffic_tree),
        map_tree(m1.map_tree, m2.map_tree)
        {}

void TrafficMeter::set_traffic(ServerID id, Traffic traffic) {
    if(map_tree.contains(id))
        traffic_tree.erase(map_tree.get(id));
    traffic_tree.insert(traffic);
    map_tree[id]=traffic;
}

void TrafficMeter::remove_server(ServerID id) {
    auto traffic = map_tree.get(id);
    traffic_tree.erase(traffic);
    map_tree.erase(id);
}

int TrafficMeter::sum_highest_traffic_servers(int k) {
    auto iterator= traffic_tree.find_Kth_element(min(k,traffic_tree.size()));
    return traffic_tree.get_sum_less_then(iterator);
}


