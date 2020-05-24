//
//  FLOOK.cpp
//  IO_Schedular
//
//  Created by Zhuocheng Xu on 12/5/19.
//  Copyright © 2019 Zhuocheng Xu. All rights reserved.
//

#include "main.h"
#include <list>
#include <iostream>
#include <limits>
#include <unordered_map>

using namespace std;

FLOOK::FLOOK(){
    moving_right = 1;
}

IO_Operation* FLOOK::get_next_operation(int track){
    
    int min_margin = numeric_limits<int>::max();
    int max_value = -1;
    int min_value = numeric_limits<int>::max();
    IO_Operation* result = nullptr;
    IO_Operation* max_operation = nullptr;
    IO_Operation* min_operation = nullptr;
    for(auto it = active_operations.begin(); it != active_operations.end(); ++it){
        add_queue.push_back(*it);
    }
    active_operations.clear();
    
    if(active_queue.size() == 0){
        for(auto it = add_queue.begin(); it != add_queue.end(); ++it){
            active_queue.push_back(*it);
        }
        add_queue.clear();
    }
    
    if(active_queue.size() != 0){
        for(auto it = active_queue.begin(); it != active_queue.end(); ++it){
            if((*it)->track > max_value){max_value = (*it)->track; max_operation = *it;}
            if((*it)->track < min_value){min_value = (*it)->track; min_operation = *it;}
            if(moving_right*((*it)->track - track) >= 0 && moving_right*((*it)->track - track) < min_margin){
                min_margin = moving_right*((*it)->track - track);
                result = *it;
            }
        }
        if(result == nullptr && active_queue.size() != 0){
            if(moving_right > 0){
                result = max_operation;
            }else{
                result = min_operation;
            }
            moving_right = -moving_right;
        }
        insertExpired(result);
        active_queue.remove(result);
    }
    return result;
}
