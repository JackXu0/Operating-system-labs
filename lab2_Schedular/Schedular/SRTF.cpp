//
//  SRTF.cpp
//  Schedular
//
//  Created by Zhuocheng Xu on 10/9/19.
//  Copyright © 2019 Zhuocheng Xu. All rights reserved.
//

#include "main.h"
using namespace std;
SRTF::SRTF(int qtm) : Schedular(qtm){
    list<Process*> activeQueue, expiredQueue;
    quantum = qtm;
    type = "SRTF";
}

bool SRTF::isEmpty() {
    return (activeQueue.size() == 0);
}

Process* SRTF::get_next_process(int timestamp){
    
    
    for(list<Process*>::iterator it=activeQueue.begin(); it != activeQueue.end();){
        if ((*it) -> TC == 0 ){
            expiredQueue.push_back(*it);
            it=activeQueue.erase(it);
        }
        else
            ++it;
    }
    if(activeQueue.empty() ) return nullptr;
    int min = activeQueue.front()->TC;
    Process* result = activeQueue.front();
    for (list<Process*>::reverse_iterator it = activeQueue.rbegin(); it != activeQueue.rend(); ++it){
        if((*it)->TC < min){
            result = *it;
            min = (*it)->TC;
        }
    }
    
    activeQueue.remove(result);
    return result;
}

void SRTF::add_active_process(Process* p){
    
    activeQueue.push_back(p);
    
}

void SRTF::add_expired_process(Process* p) {
    expiredQueue.push_back(p);
}
