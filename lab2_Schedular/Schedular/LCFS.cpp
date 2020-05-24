//
//  LCFS.cpp
//  Schedular
//
//  Created by Zhuocheng Xu on 10/9/19.
//  Copyright © 2019 Zhuocheng Xu. All rights reserved.
//

#include "main.h"
using namespace std;
LCFS::LCFS(int qtm) : Schedular(qtm){
    list<Process*> activeQueue, expiredQueue;
    quantum = qtm;
    type = "LCFS";
}

bool LCFS::isEmpty() {
    return (activeQueue.size() == 0);
}

Process* LCFS::get_next_process(int timestamp){
    
    Process* result = nullptr;
    while(!activeQueue.empty()){
        
        if(activeQueue.back()->TC == 0){
            expiredQueue.push_back(activeQueue.back());
            activeQueue.pop_back();
            continue;
        }
        
        result = activeQueue.back();
        activeQueue.pop_back();
        break;
    }
    
    return result;
}

void LCFS::add_active_process(Process* p){
    
    activeQueue.push_back(p);
}

void LCFS::add_expired_process(Process* p) {
    expiredQueue.push_back(p);
}

