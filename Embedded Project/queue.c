
#include "queue.h"

q * ready_queue;   /*Global definition for both queues used, there is an implicit assumption that the head_of_queue is initialized to NULL*/
q * delay_queue;
t * current_running_task;

int volatile current_running_task_p;
int volatile current_running_task_d;
task_function * volatile current_running_task_f;

void init(void){
	/*IMPORTANT: if any new data structures are created, they must be initialized here*/
	ready_queue = (q*)malloc(sizeof(q));
	delay_queue = (q*)malloc(sizeof(q));
}

void Dispatch(void){
	t * temporary;
	if(ready_queue->head_of_queue==NULL) return;
	current_running_task = ready_queue->head_of_queue;/*IMPORTANT: This points to the task currently running so that it can be called from within ReRunMe */
	current_running_task_p = current_running_task->priority;
	current_running_task_d = current_running_task->delay;
	current_running_task_f = current_running_task->f;

	ready_queue->head_of_queue->f();
	ready_queue->head_of_queue = ready_queue->head_of_queue->next_task;
	free(current_running_task);
}



void ReRunMe(int delay){
	bool delay_flag = false;
	t * current_task;
	t * last_task;
	t * new_task;
	
	new_task = (t*)malloc(sizeof(t));
	new_task->f = current_running_task_f;
	new_task->priority = current_running_task_p;
	new_task->delay = delay;

	if(delay==0){ 
		QueTask(current_running_task_f, current_running_task_p);
		return;
	}
	
	if(delay_queue->head_of_queue==NULL){    /*special case: the first element in the queue*/
		delay_queue->head_of_queue = new_task;
		new_task->next_task = NULL;
	    }else{
		if(delay_queue->head_of_queue->delay > delay){ /*special case: the case of having a priority higher than the head of the queue*/
		    new_task->next_task = delay_queue->head_of_queue;
		    delay_queue->head_of_queue = new_task;
		}else{
		    
		    current_task = delay_queue->head_of_queue;
		    while(delay_flag == false && current_task!=NULL){
		        if(current_task->delay > delay){
		            delay_flag = true;
		            new_task->next_task = current_task;
		            last_task->next_task = new_task;
		        }else{
		            last_task = current_task;
		            current_task = current_task->next_task;
		        }
		
		    }
		    /*special case: if the element is inserted at the end of the queue
		    if the element is inserted at the end of the queue, then the priority_flag must be false
		    last_task will be pointing to the last task in the queue, also new_task has next_task=NULL*/
		    if(delay_flag==false){ 
		        last_task->next_task = new_task;
		    }
		}
	    }
}

/*Assumption: if 2 tasks have the same priority, FCFS*/
void QueTask(task_function * t_f, /*pointer to the function*/    /*IMPORTANT: This was changed to a c string for testing purposes*/
            //char * string_new, 
             int prio /*priority of the task entered into the queue*/)
{   
    bool priority_flag = false;
    t * current_task;
    t * last_task; 
    t * new_task;
	
    new_task = (t*)malloc(sizeof(t));
    //new_task->string = string_new;
    new_task->f = t_f;
    new_task->delay = -1; //-1 is the default value of the delay, since a task in ready queue has no delay
    new_task->priority = prio; //can choose to make validations on the priority entered later  
    if(ready_queue->head_of_queue==NULL){    /*special case: the first element in the queue*/
        ready_queue->head_of_queue = new_task;
        new_task->next_task = NULL;
    }else{
        if(ready_queue->head_of_queue->priority > prio){ /*special case: the case of having a priority higher than the head of the queue*/
            new_task->next_task = ready_queue->head_of_queue;
            ready_queue->head_of_queue = new_task;
        }else{
            
            current_task = ready_queue->head_of_queue;
            while(priority_flag == false && current_task!=NULL){
                if(current_task->priority > prio){
                    priority_flag = true;
                    new_task->next_task = current_task;
                    last_task->next_task = new_task;
                }else{
                    last_task = current_task;
                    current_task = current_task->next_task;
                }
        
            }
            /*special case: if the element is inserted at the end of the queue
            if the element is inserted at the end of the queue, then the priority_flag must be false
            last_task will be pointing to the last task in the queue, also new_task has next_task=NULL*/
            if(priority_flag==false){ 
                last_task->next_task = new_task;
            }
        }
    }


}



