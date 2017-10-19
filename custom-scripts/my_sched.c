#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/sched.h>
#include <string.h>

char *buffer;
int thread_size;
int buf_ptr_idx;
pthread_mutex_t mutex;
pthread_mutex_t prio_mutex;
pthread_barrier_t barrier;

void* thread_run(void *thread_idx){

	pthread_mutex_lock(&prio_mutex);
	pthread_mutex_unlock(&prio_mutex);

	int idx = *((int *) thread_idx);

	for (int i = 0; i < thread_size; i++){

		pthread_mutex_lock(&mutex);
		*(buffer+buf_ptr_idx++) = idx+'A';
		pthread_mutex_unlock(&mutex);

	}
	pthread_barrier_wait(&barrier);
}

void print_sched(int policy){
	switch(policy){
		case SCHED_FIFO: 
			printf("SCHED_FIFO\n");
			break;
		case SCHED_RR: 
			printf("SCHED_RR\n");
			break;
		case SCHED_OTHER: 
			printf("SCHED_OTHER\n");
			break;
		default:
			printf("unknown\n");
	}
}

void set_priority(pthread_t *thr, int newpolicy, int newpriority){
	
	int policy, ret;
    struct sched_param param;
    pthread_t t;
    
    if(newpriority > sched_get_priority_max(newpolicy) ||
	   newpriority < sched_get_priority_min(newpolicy)){
		printf("Invalid priority: MIN: %d, MAX: %d\n", sched_get_priority_min(newpolicy), sched_get_priority_max(newpolicy));
	}

    pthread_getschedparam(*thr, &policy, &param);  
    param.sched_priority = newpriority;
    //print_sched(newpolicy);    
    
    ret = pthread_setschedparam(*thr, newpolicy, &param);
    
    if (ret != 0){
		perror("perror(): ");
	}
}

int main(int argc, char **argv){

    int num_threads = atoi(*(argv+1));
	int buf_size = 1000*atoi(*(argv+2));
    buffer = (char *) malloc(buf_size*sizeof(char));
    char *policy = *(argv+3);
    int prio = atoi(*(argv+4));
    int new_policy;

	if(!strcmp(policy,"SCHED_FIFO")){
		new_policy = SCHED_FIFO;
	}
	else if(!strcmp(policy,"SCHED_RR")){
		new_policy = SCHED_RR;
	}
	else if(!strcmp(policy,"SCHED_OTHER")){
		new_policy = SCHED_OTHER;
	}

	thread_size = buf_size/num_threads;
	buf_ptr_idx = 0;	
	
	pthread_mutex_init(&mutex,NULL);
	pthread_mutex_init(&prio_mutex,NULL);

	pthread_t *threads;
	threads = (pthread_t *) malloc(num_threads * sizeof(pthread_t));
	pthread_barrier_init(&barrier,NULL,num_threads+1);
	int *thread_indices = (int *) malloc(num_threads * sizeof(int));
	
	for (int i = 0; i < num_threads; ++i){
		*(thread_indices+i) = i;

		pthread_mutex_lock(&prio_mutex);

		pthread_create(threads+i, NULL, thread_run, thread_indices+i);	
		set_priority(threads+i,new_policy,prio);

		pthread_mutex_unlock(&prio_mutex);
	}
	pthread_barrier_wait(&barrier);	

	int *scheds = (int *) malloc(num_threads * sizeof(int));

	(*(scheds + *buffer - 'A'))++;

	
		printf("%c",*buffer);
		for (int i = 1; i < buf_ptr_idx; i++){
			if (*(argv+5)){
				printf("%c",*(buffer+i));
			}
			if(*(buffer+i)!=*(buffer+i-1)){
				(*(scheds + *(buffer+i) - 'A'))++;
			}
		}
		printf("\n");

	
	for (int i = 0; i < num_threads; i++){
		printf("%c: %d\n",i+'A',*(scheds+i));
	}

	return 0;   
}

