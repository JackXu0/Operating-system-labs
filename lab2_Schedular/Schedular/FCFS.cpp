//
//  FCFS.cpp
//  Schedular
//
//  Created by Zhuocheng Xu on 10/9/19.
//  Copyright © 2019 Zhuocheng Xu. All rights reserved.
//

#include "main.h"
using namespace std;
FCFS::FCFS(int qtm) : Schedular(qtm){
    list<Process*> activeQueue, expiredQueue;
    quantum = qtm;
    type = "FCFS";
}

bool FCFS::isEmpty() {
    return (activeQueue.size() == 0);
}

Process* FCFS::get_next_process(int timestamp){
    
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

void FCFS::add_active_process(Process* p){
    if(activeQueue.empty()){
        activeQueue.push_back(p);
        return;
    }
    
    activeQueue.push_back(p);
}


void FCFS::add_expired_process(Process* p) {
    expiredQueue.push_back(p);
}
