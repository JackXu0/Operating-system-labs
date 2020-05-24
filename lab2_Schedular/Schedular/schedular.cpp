//
//  schedular.cpp
//  Schedular
//
//  Created by Zhuocheng Xu on 10/7/19.
//  Copyright © 2019 Zhuocheng Xu. All rights reserved.
//

#include <stdio.h>
#include "main.h"
using namespace std;
Schedular::Schedular(int qtm){
    list<Process*> activeQueue, expiredQueue;
    quantum = qtm;
    blockTill = 0;
    non_overleap_io = 0;
    is_preemitive = false;
    
}

void Schedular::add_active_process(Process* p){
    activeQueue.push_back(p);
}

void Schedular::add_expired_process(Process* p){
    return;
}

Process* Schedular::get_next_process(int timestamp){
    return activeQueue.front();
}

Process* Schedular::get_next_process(){
    return activeQueue.front();
}
