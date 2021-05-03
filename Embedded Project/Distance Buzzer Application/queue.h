#ifndef QUEUE
#define QUEUE

#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>


typedef void task_function(void);
typedef struct task t;
typedef struct queue q;




typedef struct task
{
    task_function* f; /*IMPORTANT: for testing purposes, the pointer to a function was replaced by a c string*/
    //char * string;
    t * next_task;  /*points to the next task in the queue*/
    int priority;   //implementations of checks for the priority
    int delay;      //only implemented in the case of the delay_queue
}t;


typedef struct queue
{
    t * head_of_queue;
}q;

void init(void);

void Dispatch(void);
void ReRunMe(int delay);
void QueTask(task_function * t_f, /*pointer to the function*/    /*IMPORTANT: This was changed to a c string for testing purposes*/
            //char * string_new, 
             int prio /*priority of the task entered into the queue*/);
#endif
