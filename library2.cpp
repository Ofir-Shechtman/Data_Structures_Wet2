#include "DataServer.h"
#include "library2.h"

void* Init(int n){
    try{
        DataServer *DS = new DataServer(n);
        return DS;
    }
    catch(std::bad_alloc&){
        return NULL;
    }
    catch(std::exception&){
        return NULL;
    }
}

StatusType MergeDataCenters(void *DS, int dataCenter1, int dataCenter2) {
    if(DS == NULL || dataCenter1 <= 0 || dataCenter2 <= 0){
        return INVALID_INPUT;
    }
    try{
        ((DataServer*)DS)->MergeDataCenters(dataCenter1,dataCenter2);
        return SUCCESS;
    }
    catch(UnionFind<TrafficMeter>::InvalidInput&) {
        return INVALID_INPUT;
    }
    catch(std::bad_alloc&){
        return ALLOCATION_ERROR;
    }
    catch (std::exception&) {
        return FAILURE;
    }
}

StatusType AddServer(void *DS, int dataCenterID, int serverID) {
    if(DS == NULL || dataCenterID <= 0 || serverID <= 0) {
        return INVALID_INPUT;
    }
    try{
        ((DataServer*)DS)->AddServer(dataCenterID,serverID);
        return SUCCESS;
    }
    catch(UnionFind<TrafficMeter>::InvalidInput&) {
        return INVALID_INPUT;
    }
    catch(std::bad_alloc&){
        return ALLOCATION_ERROR;
    }
    catch (std::exception&) {
        return FAILURE;
    }
}

StatusType RemoveServer(void *DS, int serverID) {
    if(DS == NULL || serverID <= 0) {
        return INVALID_INPUT;
    }
    try{
        ((DataServer*)DS)->RemoveServer(serverID);
        return SUCCESS;
    }
    catch(std::bad_alloc&){
        return ALLOCATION_ERROR;
    }
    catch (std::exception&) {
        return FAILURE;
    }
}

StatusType SetTraffic(void *DS, int serverID, int traffic) {
    if(DS == NULL || serverID <= 0 || traffic<0) {
        return INVALID_INPUT;
    }
    try{
        ((DataServer*)DS)->SetTraffic(serverID,traffic);
        return SUCCESS;
    }
    catch(std::bad_alloc&){
        return ALLOCATION_ERROR;
    }
    catch (std::exception&) {
        return FAILURE;
    }
}

StatusType
SumHighestTrafficServers(void *DS, int dataCenterID, int k, int *traffic) {
    if(DS == NULL || dataCenterID < 0 || k < 0) {
        return INVALID_INPUT;
    }
    try{
        *traffic = ((DataServer*)DS)->SumHighestTrafficServers(dataCenterID,k);
        return SUCCESS;
    }
    catch(UnionFind<TrafficMeter>::InvalidInput&) {
        return INVALID_INPUT;
    }
    catch(std::bad_alloc&){
        return ALLOCATION_ERROR;
    }
    catch (std::exception&) {
        return FAILURE;
    }
}

void Quit(void **DS) {
    delete (DataServer*)*DS;
    *DS=NULL;
}

