//
//  DES.cpp
//  Schedular
//
//  Created by Zhuocheng Xu on 10/8/19.
//  Copyright © 2019 Zhuocheng Xu. All rights reserved.
//

#include "main.h"
using namespace std;
DES::DES(){
    list<Event*> evt_queue;
}


void DES::insert_event(Event* e) {
    if(evt_queue.empty()){
        evt_queue.push_back(e);
        return;
    }
    for (list<Event*>::iterator it = evt_queue.begin(); it != evt_queue.end();) {
        if((*it)->get_timestamp() > e->get_timestamp()) {
            evt_queue.insert(it, e);
            return;
        }
        else it++;
    }
    
    evt_queue.push_back(e);
    
}

Event* DES::get_event() {
    
    return evt_queue.front();
}


void DES::delete_event() {
    evt_queue.pop_front();
}
