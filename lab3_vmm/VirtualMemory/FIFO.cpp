//
//  FIFO.cpp
//  VirtualMemory
//
//  Created by Zhuocheng Xu on 11/21/19.
//  Copyright © 2019 Zhuocheng Xu. All rights reserved.
//

#include "main.h"
#include <list>
#include <unordered_map>

using namespace std;
FIFO::FIFO(){
    
}

frame_t FIFO::select_victim_frame(unordered_map<frame_t*, pte_t*>* frame_map, list<frame_t*>* frame_list){

    frame_t t = *(frame_map->begin()->first);
    pte_t* pte = frame_map->begin()->second;
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





