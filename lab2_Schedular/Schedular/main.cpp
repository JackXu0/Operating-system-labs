//
//  main.cpp
//  Schedular
//
//  Created by Zhuocheng Xu on 10/7/19.
//  Copyright © 2019 Zhuocheng Xu. All rights reserved.
//

#include "main.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <list>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>



using namespace std;

vector<int> randvals;
int randRange;
int ofs = 0;
bool is_prio = false;
int maxPrio = 4;

int myrandom(int burst) {
    int result = 1 + (randvals[ofs] % burst);
    ofs = (ofs+1)%randRange;
    return result;
}

void readInput(DES* des, string filename){
    fstream file;
    string AT, TC, CB, IO;
    int lineNumber = 0;
    file.open(filename.c_str());
    bool added = false;
    
    while (file >> AT >> TC >> CB >> IO){
        int prio = myrandom(maxPrio)-2;
        if(is_prio) prio++;
        Process* p = new Process(lineNumber, atoi(AT.c_str()),atoi(TC.c_str()), atoi(CB.c_str()),atoi(IO.c_str()), prio);
        p->state_ts = atoi(AT.c_str());
        p->state = STATE_CREATED;
        p->state_str = "CREATED";
    
        Event* e = new Event(atoi(AT.c_str()), p, TRANS_TO_READY);
        des->insert_event(e);
        
//        cout<<stoi(AT)<< " " <<lineNumber << " 0:CREATED -> READY";
        lineNumber++;
    }
}

void readRand(string fileName){
    fstream file;
    file.open(fileName.c_str());
    string num;
    file >> num;
    randRange = atoi(num.c_str());
    while (file >> num){
        randvals.push_back(atoi(num.c_str()));
    }
}

int getSupposedAT(DES* des, int pid){
    int supposedAT = 0;
    for(list<Event*>::iterator it=des->evt_queue.begin(); it != des->evt_queue.end();++it){
        if ((*it)->get_evt_process()->pid == pid ){
            supposedAT = (*it)->get_timestamp();
//            cout<< "LLLLLLLLLL  find time = " << supposedAT << "\n";
        }
    }
    return supposedAT;
}

void removeDuplicate(DES* des, int pid){
    for(list<Event*>::iterator it=des->evt_queue.begin(); it != des->evt_queue.end();){
        if ((*it)->get_evt_process()->pid == pid ){
            it=des->evt_queue.erase(it);
        }
        else
            ++it;
    }
}

void Simulation(DES* des, Schedular* shl, int vflag) {
    Event* evt;
    Process* proc_running = nullptr;
    int just_break_time = -1;
    int just_break_prio =0;
    string pre = "";
    string end = "";
    list<Process*> cache;
    int CURRENT_TIME = 0;
    while( (evt = des->get_event()) ) {
        des->delete_event();
        CURRENT_TIME = evt->get_timestamp();
        Process* proc = evt->get_evt_process(); // this is the process the event works on
        
        bool CALL_SCHEDULER = false;
        
        int timeInPrevState = CURRENT_TIME - proc->state_ts;
        switch(evt->get_transaction()) { // which state to transition to?
            case TRANS_TO_READY:{
                // must come from BLOCKED or from PREEMPTION
                // must add to run queue
                string pre_state = proc->state_str;
                if(proc->TC == 0){
                    if(vflag == 1)
                    cout<< CURRENT_TIME << " " << proc->pid << " " << timeInPrevState << ": Done\n";
                    CALL_SCHEDULER = true; // conditional on whether something is run
                    proc->finishing_time = CURRENT_TIME;
                    shl->add_expired_process(proc);
                    if(proc_running!=nullptr && proc->pid == proc_running->pid)
                        proc_running = nullptr;
                    break;
                }
                
                if(proc->state == STATE_BLOCKED ||proc->state == STATE_CREATED ){
                    if(vflag == 1)
                    cout<< CURRENT_TIME << " " << proc->pid << " " << timeInPrevState << ": " << proc->state_str<< " -> READY\n";
                }else{
                    if(vflag == 1)
                    cout<< CURRENT_TIME << " " << proc->pid << " " << timeInPrevState << ": " << proc->state_str<< " -> READY  cb=" << proc->cpu_burst << " rem=" << proc->TC <<" prio=" << (proc->current_prio+1)<< "\n";
                }
                
                
                if(proc_running!=nullptr && proc->pid == proc_running->pid)
                    proc_running = nullptr;
                if(just_break_time == CURRENT_TIME && shl->is_preemitive&&proc->state==STATE_BLOCKED){
                    if(vflag == 1)
                    cout<< pre << proc->pid << " ? "<< (proc->PRIO >just_break_prio)<< end;
                }
                
                proc->state = STATE_READY;
                proc->state_str = "READY";
                proc->state_ts = CURRENT_TIME;
                
                if( proc_running!=nullptr && shl->is_preemitive){
                    
                    int supposedAT = getSupposedAT(des, proc_running->pid);
//                    cout<< "proc1 = " << proc_running->current_prio << " proc2 = " <<proc->PRIO<< " supposedAT = " << supposedAT << " Current time = " << CURRENT_TIME<<"\n";
//                    cout << "boolean = " << (proc->PRIO > proc_running->current_prio && supposedAT>=CURRENT_TIME) << "\n";
                    
                    bool high_prio = (proc->PRIO > proc_running->current_prio+1);
                    if(proc->PRIO > proc_running->current_prio+1 && supposedAT > CURRENT_TIME){
//                        proc_running->state = STATE_PREEMPTED;
                        proc_running->AT = CURRENT_TIME;
                        if(supposedAT!=0){
                            proc_running-> cpu_burst += (supposedAT-CURRENT_TIME);
                            proc_running-> TC += (supposedAT-CURRENT_TIME);
                        }
                        if(proc_running-> cpu_burst > proc_running-> TC){
                            proc_running-> cpu_burst = proc_running-> TC;
                        }
                        removeDuplicate(des, proc_running->pid);
                        if(vflag == 1)
                        cout<< "---> PRIO preemption "<< proc_running->pid<<" by "<<proc->pid
                        <<" ? "<< high_prio <<" TS="<<supposedAT<<" now="<<CURRENT_TIME<<") --> YES\n";
                        des->insert_event(new Event(CURRENT_TIME, proc_running, TRANS_TO_READY));
                        for (list<Process*>::iterator it = cache.begin(); it != cache.end();++it) {
                            (*it)->AT = CURRENT_TIME;
                        }
                        cache.clear();
                        just_break_time = CURRENT_TIME;
                        pre = "---> PRIO preemption "+ to_string(proc_running->pid)+" by ";
                        end = " TS="+to_string(CURRENT_TIME)+" now="+to_string(CURRENT_TIME)+") --> NO\n";
                        proc_running = nullptr;
                    }else{
                        if(vflag == 1)
                        cout<< "---> PRIO preemption "<< proc_running->pid<<" by "<<proc->pid
                        <<" ? "<<high_prio<<" TS="<<supposedAT<<" now="<<CURRENT_TIME<<") --> NO\n";
                        if(CURRENT_TIME == supposedAT){
                            proc->AT = supposedAT;
                            for (list<Process*>::iterator it = cache.begin(); it != cache.end();++it) {
                                (*it)->AT = CURRENT_TIME;
                            }
                            cache.clear();
                        }else{
//                            proc->supposed_AT = supposedAT;
                            cache.push_back(proc);
                        }
                        
                    }
                    
                }
//                cout<< "process " << proc->pid  << " in queue; prio = " << proc->current_prio << "; AT = "<<proc->AT<< "\n";
                shl->add_active_process(proc);
//                cout<< "current not running = " <<(proc_running == nullptr)<<"\n";
//                des->insert_event(new Event(CURRENT_TIME, proc, TRANS_TO_RUN));
                CALL_SCHEDULER = true; // conditional on whether something is run
                break;
            }
            case TRANS_TO_BLOCK:{
                //create an event for when process becomes READY again
                int io_burst = myrandom(proc->IO);
                proc->current_prio = proc->PRIO;
                if(vflag == 1)
                cout<< CURRENT_TIME << " " << proc->pid << " " << timeInPrevState << ": " <<proc->state_str<<" -> BLOCK  ib=" << io_burst << " rem=" << proc->TC <<"\n";
                proc->state_ts = CURRENT_TIME;
                proc->state = STATE_BLOCKED;
                proc->state_str = "BLOCK";
                proc->io_time += io_burst;
                if(CURRENT_TIME < shl->blockTill && CURRENT_TIME + io_burst >= shl->blockTill){
                    shl->non_overleap_io += io_burst + CURRENT_TIME - shl->blockTill ;
                }else if(CURRENT_TIME+io_burst >= shl->blockTill){
                    shl->non_overleap_io += io_burst;
                }
                shl->blockTill = max(shl->blockTill, CURRENT_TIME+io_burst);
                des->insert_event(new Event(CURRENT_TIME+io_burst, proc, TRANS_TO_READY));
                CALL_SCHEDULER = true;
                if(proc_running!=nullptr && proc->pid == proc_running->pid)
                    proc_running = nullptr;
                break;
            }
            case TRANS_TO_RUN:{
                // create event for either preemption or blocking
                int cpu_burst;
                string pre_state = proc->state_str;
                if(proc-> cpu_burst == 0){
                    cpu_burst = myrandom(proc->CB);
                    proc->cpu_burst = cpu_burst;
                }else{
                    cpu_burst = proc-> cpu_burst;
                }
                
                if(cpu_burst > proc->TC){
                    cpu_burst = proc->TC;
                    proc->cpu_burst = cpu_burst;
                }
                
                proc->state_ts = CURRENT_TIME;
                proc->state = STATE_RUNNING;
                proc->state_str = "RUNNG";
                if(is_prio)
                    proc->current_prio--;
                proc->CW += timeInPrevState;
                // by cpu burst, should go to block
                if(cpu_burst <= shl->quantum){
                    if(vflag == 1)
                    cout<< CURRENT_TIME << " " << proc->pid << " " << timeInPrevState << ": " <<pre_state <<" -> RUNNG cb=" << cpu_burst << " rem=" << proc->TC <<" prio=" << (proc->current_prio+1)<< "\n";
                    
                    proc->TC = proc->TC - cpu_burst;
                    
                    
                    if(proc->TC > 0){
                        //if not finished, go to block
                        des->insert_event(new Event(CURRENT_TIME + cpu_burst, proc, TRANS_TO_BLOCK));
                        proc-> cpu_burst = 0;
                    }else{
                        des->insert_event(new Event(CURRENT_TIME + cpu_burst, proc, TRANS_TO_READY));
                    }
                    
                    CURRENT_TIME += cpu_burst;
                }else{
                    if(vflag == 1)
                    cout<< CURRENT_TIME << " " << proc->pid << " " << timeInPrevState << ": " <<pre_state<< " -> RUNNG cb=" << cpu_burst << " rem=" << proc->TC <<" prio=" << (proc->current_prio+1)<< "\n";
                    proc->TC = proc->TC - min(proc->cpu_burst,shl->quantum);
                    proc->cpu_burst = proc->cpu_burst - min(proc->cpu_burst,shl->quantum);
                    
                    if(proc->cpu_burst > 0){
                        des->insert_event(new Event(CURRENT_TIME + shl->quantum, proc, TRANS_TO_READY));
                    }else{
                        if(proc->TC > 0){
                            des->insert_event(new Event(CURRENT_TIME + min(proc->cpu_burst,shl->quantum), proc, TRANS_TO_BLOCK));
                        }else{
                            des->insert_event(new Event(CURRENT_TIME + min(proc->cpu_burst,shl->quantum), proc, TRANS_TO_READY));
                        }
                    }
                    
                    CURRENT_TIME = CURRENT_TIME + min(proc->cpu_burst,shl->quantum);
                    
                }
                
                proc_running = proc;
                
                break;
            }
                
        }
        // remove current event object from Memory
        evt = nullptr;
        
        if(CALL_SCHEDULER) {
//            cout<< " " << (des->get_event()->get_transaction()) << "\n";
            
            if (des->get_event()){
                if(des->get_event()->get_timestamp() == CURRENT_TIME){
                    continue;//process next event from Event queue
                }
                
            }
            
            CALL_SCHEDULER = false; // reset global flag

            if (proc_running == nullptr) {
                proc = shl->get_next_process(CURRENT_TIME);
                
                if (proc == nullptr){
//                    cout<< "pid = null \n" ;
                    continue;
                }else{
//                    cout<< "pid" << proc->pid << " prio = " << proc->current_prio << " at = "<< proc->AT<< "\n" ;

                }
                
                // create event to make this process runnable for same time.
                des->insert_event(new Event(max(CURRENT_TIME, proc->AT), proc, TRANS_TO_RUN));

            }
        }
    }
    
}

void printSummary(Schedular* shl){
    using std::setw;
    int prio;
    double last_finishing_time = 0;
    double cb_sum = 0;
    double io_sum = 0;
    double turn_around_sum = 0;
    double cpu_waiting_sum = 0;
    double size = shl->expiredQueue.size();
//    cout<< "size = " << size << "\n";
    
    cout<< shl->type << "\n";
    for(int i =0; i<shl->expiredQueue.size(); i++){
        for(list<Process*>::iterator it=shl->expiredQueue.begin(); it != shl->expiredQueue.end();++it){
            if ((*it) -> pid == i ){
                if((*it)->finishing_time > last_finishing_time){
                    last_finishing_time = (*it)->finishing_time;
                }
                cb_sum += (*it) ->TC_origin;
                io_sum += (*it) ->io_time;
                turn_around_sum += ((*it) ->finishing_time- (*it) ->AT_origin);
                cpu_waiting_sum += (*it) ->CW;
                if(is_prio){
                    prio = (*it) ->PRIO+1;
                }else{
                    prio = (*it) ->PRIO+2;
                }
                cout<< "000" << (*it) -> pid << ":" <<setw(5) << (*it) ->AT_origin <<setw(5) << (*it) ->TC_origin <<setw(5) << (*it) ->CB <<setw(5) << (*it) ->IO <<setw(2) <<prio << " | " <<setw(5) << (*it) ->finishing_time <<setw(6) << ((*it) ->finishing_time- (*it) ->AT_origin) <<setw(6) << (*it) ->io_time <<setw(6) << (*it) ->CW << endl ;
            }
        }
    }
    
    cout<<setiosflags(ios::fixed)<<setprecision(0);
    cout<< "SUM: " << last_finishing_time << " " << setprecision(2) << 100*cb_sum/last_finishing_time << " " << 100*shl->non_overleap_io/last_finishing_time << " " << turn_around_sum/size << " " << cpu_waiting_sum/size << " " << setprecision(3)<<size*100/last_finishing_time << endl;
}


int main(int argc, char **argv) {
    DES* des = new DES();
    int vflag = 0;
    Schedular* shl;
    string inputfile;
    string randfile;
    string schedulingAlgo;
    int c;
    while ((c = getopt (argc, argv, "vs:")) != -1){
        switch (c){
            case 'v':
                vflag = 1;
                break;
            case 's':
                schedulingAlgo = optarg;
                break;
            default:
                break;
        }
    }
    int mp =  0;
    switch (schedulingAlgo.at(0)) {
            
        case 'F':
            shl = new FCFS(100000);
            break;
        case 'L':
            shl = new LCFS(100000);
            break;
        case 'S':
            shl = new SRTF(100000);
            break;
        case 'R':
            shl = new RR(atoi(schedulingAlgo.substr(1).c_str()));
            break;
        case 'P':
            if(schedulingAlgo.length()>3){
                maxPrio = atoi(schedulingAlgo.substr(3).c_str());
            }
            shl = new PRIO(atoi(schedulingAlgo.substr(1).c_str()),maxPrio);
            is_prio = true;
            break;
        case 'E':
            if(schedulingAlgo.length()>3){
                maxPrio = atoi(schedulingAlgo.substr(3).c_str());
            }
            shl = new Pre_PRIO(atoi(schedulingAlgo.substr(1).c_str()),maxPrio);
            is_prio = true;
            break;
        default:
            break;
    }

    randfile = argv[argc-1];
    inputfile = argv[argc-2];
    
    readRand(randfile);
    readInput(des,inputfile);
    
    Simulation(des, shl, vflag);
    printSummary(shl);
    
    return 0;
}
