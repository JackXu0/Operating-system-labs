//
//  IO_Schedular.cpp
//  IO_Schedular
//
//  Created by Zhuocheng Xu on 12/1/19.
//  Copyright © 2019 Zhuocheng Xu. All rights reserved.
//

#include "main.h"

IO_Schedular::IO_Schedular(){}

void IO_Schedular::insertExpired(IO_Operation *op){
    for(auto it = expired_operations.begin(); it != expired_operations.end(); ++it){
        if((*it)->id > op->id){
            expired_operations.insert(it, op);
            return;
        }
    }
    
    expired_operations.push_back(op);
}

IO_Operation* IO_Schedular::get_next_operation(int time){
    return nullptr;
}
