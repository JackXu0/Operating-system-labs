//
//  RR.cpp
//  Schedular
//
//  Created by Zhuocheng Xu on 10/9/19.
//  Copyright © 2019 Zhuocheng Xu. All rights reserved.
//

#include "main.h"
using namespace std;
RR::RR(int qtm) : Schedular(qtm){
    list<Process*> activeQueue, expiredQueue;
    quantum = qtm;
    type = "RR "+to_string(qtm);
}

bool RR::isEmpty() {
    return (activeQueue.size() == 0);
}

Process* RR::get_next_process(int timestamp){
    
    Process* result = nullptr;
    while(!activeQueue.empty()){
        
        if(activeQueue.front()->TC == 0){
            expiredQueue.push_back(activeQueue.front());
            activeQueue.pop_front();
            continue;
        }
        
        result = activeQueue.front();
        activeQueue.pop_front();
        break;
    }
    
    return result;
    
}

void RR::add_active_process(Process* p){
    activeQueue.push_back(p);
}

void RR::add_expired_process(Process* p) {
    expiredQueue.push_back(p);
}

