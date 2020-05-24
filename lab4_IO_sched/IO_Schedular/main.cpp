//
//  main.cpp
//  IO_Schedular
//
//  Created by Zhuocheng Xu on 12/1/19.
//  Copyright © 2019 Zhuocheng Xu. All rights reserved.
//

#include "main.h"
#include <list>
#include <iostream>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>
#include <limits>


using namespace std;

list<IO_Operation*> all_operations;
list<IO_Operation*> active_operations;
list<IO_Operation*> expired_operations;
IO_Schedular* schedular;
int time_count = 0;
int frame_move = 0;

void readInput(string filename){
    int counter = 0;
    string line, start, track;
    ifstream file(filename.c_str());
    //skip all # add all IO Operations into all_operations list
    while((getline(file, line))){
        if(line.at(0) != '#'){
            stringstream ss(line);
            ss >> start >> track;
            
            all_operations.push_back(new IO_Operation(counter, atoi(start.c_str()), atoi(track.c_str())));
            counter++;
        }
    }
}

void check_io_arrival(){
    if(all_operations.size() == 0){
        return;
    }
    for(auto it = all_operations.begin(); it != all_operations.end(); ++it){
//        cout<< (*it)->id << ";" << (*it)->arriving_time<< "; size = "<< all_operations.size() << "\n";
        if((*it)->arriving_time == time_count){
            active_operations.push_back(*it);
            all_operations.erase(it);
            return;
        }
    }
}

void printSummary(){
    double turn_around_sum = 0;
    double waiting_time_sum = 0;
    int max_waiting_time = 0;
    for(auto it = expired_operations.begin(); it != expired_operations.end(); ++it){
        printf("%5d: %5d %5d %5d\n",(*it)->id, (*it)->arriving_time, (*it)->start_time, (*it)->finishing_time);
        turn_around_sum += (*it)->finishing_time - (*it)->arriving_time;
        waiting_time_sum += (*it)->start_time - (*it)->arriving_time;
        if((*it)->start_time - (*it)->arriving_time > max_waiting_time){
            max_waiting_time = (*it)->start_time - (*it)->arriving_time;
        }
    }
    printf("SUM: %d %d %.2lf %.2lf %d\n",
           time_count, frame_move, turn_around_sum/expired_operations.size(), waiting_time_sum/expired_operations.size(), max_waiting_time);
}



void simulate(){
    
    int current_frame = 0;
    int destination_frame = numeric_limits<int>::max();
    int direction = 0;
    IO_Operation* current_operation = nullptr;
    while(true){
        check_io_arrival();
//        cout<< current_frame << "\n";

        if(current_operation == nullptr){
            current_operation = schedular->get_next_operation(current_frame);
            if(current_operation != nullptr){
                destination_frame = current_operation->track;
                direction = destination_frame > current_frame? 1 : -1;
                current_operation->start_time = time_count;
            }else if(all_operations.size() == 0){
                return;
            }else{
                time_count++;
                continue;
            }
            
        }else{
            current_frame += direction;
            time_count++;
            frame_move++;
        }
        
        if(current_frame == destination_frame){
            current_operation->finishing_time = time_count;
            current_operation = nullptr;
        }
        
    }
}

int main(int argc, char **argv) {
    int c;
    string pager_string, option_string;
    c = getopt(argc, argv, "s:");
    string algorithm = optarg;
    switch (algorithm.at(0)) {
        case 'i':
            schedular = new FIFO();
            break;
        case 'j':
            schedular = new SSTF();
            break;
        case 's':
            schedular = new LOOK();
            break;
        case 'c':
            schedular = new CLOOK();
            break;
        case 'f':
            schedular = new FLOOK();
            break;
//        case 'w':
//            pager = new Working_set();
//            break;
        default:
            break;
    }
    readInput(argv[argc-1]);

        simulate();
        printSummary();
    
    
    return 0;
}
