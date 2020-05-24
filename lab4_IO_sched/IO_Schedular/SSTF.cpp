//
//  SSTF.cpp
//  IO_Schedular
//
//  Created by Zhuocheng Xu on 12/1/19.
//  Copyright © 2019 Zhuocheng Xu. All rights reserved.
//

#include "main.h"
#include <list>
#include <limits>
#include <cmath>
#include <unordered_map>
#include <iostream>


using namespace std;

SSTF::SSTF(){
    
}

IO_Operation* SSTF::get_next_operation(int track){
    
    int min_margin = numeric_limits<int>::max();
    IO_Operation* result = nullptr;
    for(auto it = active_operations.begin(); it != active_operations.end(); ++it){
//        cout<< (*it)->id<<" track = " <<(*it)->track << "; input track = " << track << "; margin = " <<abs((*it)->track - track) <<"\n";
        if(abs((*it)->track - track) < min_margin){
            min_margin = abs((*it)->track - track);
            result = *it;
        }
    }
    
    if(result != nullptr){
//        cout << "operation" << result->id << " finished\n";
        insertExpired(result);
        active_operations.remove(result);
    }
    
    return result;
}
