//
//  Event.cpp
//  Schedular
//
//  Created by Zhuocheng Xu on 10/8/19.
//  Copyright © 2019 Zhuocheng Xu. All rights reserved.
//

#include "main.h"
using namespace std;

Event::Event(int ts, Process* p, TRANSACTION t) {
    timestamp = ts;
    evtProcess = p;
    transaction = t;
//    cout << "proc TC = " << p->TC << "\n";
}

int Event::get_timestamp() const{
    return timestamp;
};

TRANSACTION Event::get_transaction(){
    return transaction;
}

Process* Event::get_evt_process() const{
    return evtProcess;
};


