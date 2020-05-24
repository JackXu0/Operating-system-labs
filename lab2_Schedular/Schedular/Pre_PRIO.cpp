//
//  Pre_PRIO.cpp
//  Schedular
//
//  Created by Zhuocheng Xu on 10/9/19.
//  Copyright © 2019 Zhuocheng Xu. All rights reserved.
//

#include "main.h"
using namespace std;
Pre_PRIO::Pre_PRIO(int qtm, int maxp) : Schedular(qtm){
    list<Process*> activeQueue, expiredQueue;
    quantum = qtm;
    is_preemitive = true;
    maxprio = maxp;
    type = "PREPRIO "+to_string(qtm);;
}

bool Pre_PRIO::isEmpty() {
    return (activeQueue.size() == 0);
}

Process* Pre_PRIO::get_next_process(int timestamp){
    
    if(activeQueue.empty()){
        list<Process*> t = activeQueue;
        activeQueue = expiredQueue;
        expiredQueue = t;
    }
    
    for(list<Process*>::iterator it=activeQueue.begin(); it != activeQueue.end();){
        if ((*it) -> TC == 0 ){
            add_expired_process(*it);
            it=activeQueue.erase(it);
        }
        else
            ++it;
    }
    
    if(activeQueue.empty() ) return nullptr;
    int max = activeQueue.front()->current_prio;
    Process* result = activeQueue.front();
    for (list<Process*>::iterator it = activeQueue.begin(); it != activeQueue.end(); ++it){
       if((*it)->current_prio > max){
            result = *it;
            max = (*it)->current_prio;
        }
    }
    activeQueue.remove(result);
    return result;
    
}

void Pre_PRIO::add_active_process(Process* p) {
    if(p->current_prio == -1){
        p->current_prio = p->PRIO;
        add_expired_process(p);
        return;
    }
    
    activeQueue.push_back(p);
}

void Pre_PRIO::add_expired_process(Process* p) {
    expiredQueue.push_back(p);
}
