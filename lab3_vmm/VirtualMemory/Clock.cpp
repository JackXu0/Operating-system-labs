//
//  Clock.cpp
//  VirtualMemory
//
//  Created by Zhuocheng Xu on 11/22/19.
//  Copyright © 2019 Zhuocheng Xu. All rights reserved.
//

#include "main.h"
#include <unordered_map>

using namespace std;
Clock::Clock(){
    current_pointer = nullptr;
}

frame_t Clock::select_victim_frame(unordered_map<frame_t*, pte_t*>* frame_map, list<frame_t*>* frame_list){

    current_pointer = frame_list->front();

    while(frame_map->find(current_pointer)->second->referenced){
        frame_map->find(current_pointer)->second->referenced = false;
        frame_list->push_back(current_pointer);
        frame_list->pop_front();
        current_pointer = frame_list->front();
    }
    
    frame_t t = *(frame_map->find(current_pointer)->first);
    pte_t* pte = frame_map->find(current_pointer)->second;
    cout<< " UNMAP " << pte->process_id <<":"<< pte->vma <<"\n";
    if(pte->modified){
        if(pte->file_mapped){
            cout<< " FOUT\n" ;
        }else{
            cout<< " OUT \n";
        }
        pte->pagedOut = true;
    }
    pte->referenced = false;
    pte->modified = false;
    frame_list->push_back(&t);
    frame_list->pop_front();
    
    return t;
}
