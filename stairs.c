/*
Stairs crossing problem using pThreads and Semaphores
*/

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>


#ifndef tNum
// TODO
// #define tNum: which is the maximum number of customers/threads in the system to test
#define tNum 100
// #define maxStairs: how many customer can be on the stairs at the same time
#define maxStairs 3
// you can also define other constants for your "prevent deadlock" or "prevent starvation" algorithm

#endif

//TODO
// create a struct as Thread's data. Contains the thread's ID.
struct T_thread_data
{
	pthread_t thread_id;
	int thread_no;
};


int* c_time_data[tNum];
int* a_time_data[tNum];
int* f_time_data[tNum];
struct T_thread_data t_thread_data[tNum];
clock_t completion_time;
clock_t arrival_time;
clock_t first_run_time;
int tatCnt = 0;
int rtCnt = 0;
int* tat;
int* rt;



// TODO
// define semaphores you will use
sem_t sem_id_all;
sem_t sem_id_up;
sem_t sem_id_down;


/*
 *	the following variables are created to:
 *	1. prevent deadlock
 *	2. prevent starvation
 *	3. allow more than one customer to use the stairs in the same direction in an “efficient” way
       that you determine
 */
int xingCnt, xedCnt, toAwaitCnt, toBwaitCnt;
int crossCntLock;
int crossCntUnLock;

/*
 *	flag the crossing direction (0 noDir, 1 upDir, 2 downDir)
 */
int xingDir;

//Customers going toward A (i.e. up) or toward B (i.e. down) respectively
void *a(void *arg);
void *b(void *arg);

//Semaphore wait and signal functions
void semWait(sem_t *sem);
void semSignal(sem_t *sem);
void buildUniqueSet();

void buildUniqueSet(){
	int* bucket = (int*)malloc(tNum*sizeof(int));
	int* startNode = bucket;
	int* current = bucket;
	int* endNode = bucket;

	int i = 0;
	for(i=0;i<tNum;i++)
	{
		*(current+i) = -1; //set all address space inital to -1 in bucket;
	}
	int m = 0;
	srand(time(0));
	while(1){
		int id = rand() % tNum;

		bool hasBeenUsed = false;
		current = startNode;
		while(1==1){
			if(*(current) == -1){
				hasBeenUsed = false; //number has not been assigned to address space yet
				endNode = current;  //set end Node to current Node
				break;
			}
			if(*(current) == id){
				hasBeenUsed = true; //dupicate found: current value in heap is equal to id
				break;
			}
			else{

				current++; //update to next address space in heap
			}
		}
		if(hasBeenUsed == false){
			*(endNode) = id; //set non dupicate value to endNode space
			t_thread_data[m++].thread_no = *(endNode); //set thread_no to value
			endNode++; //update to next address space


		}
		if(m==tNum){
			// if m equal to tNum end loop
			break;
		}
	}

		free(bucket);

}
int main(){


	printf("Project 2: Customer crossing problem using pThreads and Semaphores\n");
	printf("*****************\n\n");
	fflush(stdout);

	// TODO
	/*
	 *	initialize your pthread, semaphore and other variables you will need
	 *	please do error check if the program fail to pthread_create()
	 */
	int i;

	if(sem_init(&sem_id_all,0,1)){
		perror("sem_init sem_id_all failed\n");
		return EXIT_FAILURE;
	}
	if(sem_init(&sem_id_up,0,0)){
		perror("sem_init sem_id_all failed\n");
		return EXIT_FAILURE;
	}
	if(sem_init(&sem_id_down,0,0)){
		perror("sem_init sem_id_all failed\n");
		return EXIT_FAILURE;
	}
	buildUniqueSet();
	srand(time(NULL));


    printf("Parent Process PID: %d \n", getpid());//parent PID

    // TODO
	/*
	 *	do pthread_create() and call your upstair/downstair func
	 */

	struct T_thread_data *p_thread_tmp ;
	tat = (int*)malloc(tNum*sizeof(int));
	rt = (int*)malloc(tNum*sizeof(int));
	for(i=0; i < tNum;i++){
		//p_thread_tmp = &t_thread_data[thread_cnt++];
		p_thread_tmp = &t_thread_data[i];
		//checks to randomize A threads from B threads

		if(p_thread_tmp->thread_no % 2 == 0){
			//arrival_time = time(&arrival_time);
			arrival_time = clock();
			a_time_data[p_thread_tmp->thread_no] = (int*) arrival_time;
			sleep(1);
			if(pthread_create(&p_thread_tmp->thread_id,NULL,a,p_thread_tmp)){
				perror("pthread_create failed\n");
				return EXIT_FAILURE;
			}
		}

		else if(p_thread_tmp->thread_no % 2 != 0){
			//arrival_time = time(&arrival_time);
			arrival_time = clock();
			a_time_data[p_thread_tmp->thread_no] = (int*) arrival_time;
			sleep(1);
			//p_thread_tmp = &t_thread_data[thread_cnt++];
			if(pthread_create(&p_thread_tmp->thread_id,NULL,b,p_thread_tmp)){
				perror("pthread_create failed\n");
				return EXIT_FAILURE;
		}
	}

	}


	for(i=0;i<tNum;i++){
		pthread_join(t_thread_data[i].thread_id,NULL);
	}

	double avgtat = 0;
	printf("Turnaround times[");
	for(i=0; i < tatCnt;i++){
		printf("%dms,", tat[i]);
		avgtat+=tat[i];
	}
	printf("]/%d",tatCnt);
	avgtat = avgtat/tatCnt;
	if(avgtat >= 1000){
		avgtat/= 1000;
	printf("\nAverage Turnaround time = %0.1fs\n", avgtat);
}else{
	printf("\nAverage Turnaround time = %0.1fms\n", avgtat);
}

	double avgrt = 0;
	printf("Response times[");
	for(i=0; i < rtCnt;i++){
		printf("%dms,", rt[i]);
		avgrt+=rt[i];
	}
	printf("]/%d",rtCnt);
	avgrt =  avgrt/rtCnt;

	if(avgrt >= 1000){
		avgrt/= 1000;
		printf("\nAverage Response time = %0.1fs\n", avgrt);

	}
	else{
		printf("\nAverage Response time = %0.1fms\n", avgrt);
	}



	sem_destroy(&sem_id_all);
	sem_destroy(&sem_id_up);
	sem_destroy(&sem_id_down);

	return EXIT_SUCCESS;
}

/*
* Function for the customer to B level
*
*/
void *b(void *arg){


	// TODO
	/*
	 *	process the toB action.
	 *	print logs to show customers status (e.g., "customer is Crossing from A to B",
	 *  "customer to B should wait", “Finished Stairs")
	 */

	int i;
	struct T_thread_data *p_thread_data = arg;
	int tIDb = p_thread_data->thread_no;
	printf("****\nThread %d: customer would like to cross from A to B\n", tIDb);

	semWait(&sem_id_all);
	//first_run_time=time(&first_run_time);
	first_run_time = clock();

	f_time_data[tIDb]= (int*)first_run_time;
	//printf("first run %d\n",  f_time_data[tIDb]);
	//printf("arrival time %d\n",a_time_data[tIDb]);
	rt[rtCnt++] = f_time_data[tIDb] - a_time_data[tIDb];
	if(xingDir == 0){
		xingDir = 2;//downDir
		crossCntLock = 0;
		crossCntUnLock = 0;
		for(i=0;i<maxStairs;i++){
			semSignal(&sem_id_down);
		}
	}

	printf("Thread %d: There are %d customers waiting\n",tIDb,toBwaitCnt);
	toBwaitCnt++;
	semSignal(&sem_id_all);

	//ready to stair
	semWait(&sem_id_down);

	semWait(&sem_id_all);
	toBwaitCnt--;
	xingCnt++;
	crossCntLock++;
	semSignal(&sem_id_all);

	int pass_time = rand()%6+1;
	printf("Thread %d: customer is Crossing from A to B\n",tIDb);
	printf("Thread %d: Using Stairs\n",tIDb);
	printf("Thread %d: The customer will finish crossing in %d seconds\n",tIDb,pass_time);

	sleep(pass_time);


	printf("Thread %d: Finished Stairs. Waiting for mutex\n",tIDb);

	//semSignal(&sem_id_down);
	//completion_time=time(&completion_time);
	completion_time = clock();

	c_time_data[tIDb]= (int*) completion_time;
	//printf("completion run %d\n",  c_time_data[tIDb]);
	//printf("arrival time %d\n",a_time_data[tIDb]);
	tat[tatCnt++] = c_time_data[tIDb] - a_time_data[tIDb];
	semWait(&sem_id_all);

	printf("Thread %d: Mutex Passed\n",tIDb);
	xingCnt--;
	xedCnt++;

	if(crossCntLock<=maxStairs || toAwaitCnt == 0){
		semSignal(&sem_id_down);
		crossCntUnLock++;
	}
	if(xingCnt == 0){
		if(toAwaitCnt == 0){
			xingDir = 0;
		}
		else{
			xingDir = 1;
			for(i=0;i<maxStairs;i++){
				semSignal(&sem_id_up);
			}
		}
		int diff = crossCntLock - crossCntUnLock;
		//printf("==Thread %d: diff=[%d]\n",tIDb,diff);

		for(i=0;i<maxStairs-diff;i++){
			semWait(&sem_id_down);
		}
		crossCntLock = 0;
		crossCntUnLock = 0;
		printf("Thread %d: Crossing Direction Reset\n",tIDb);
	}

	printf("Thread %d: Signaling Mutex\n",tIDb);
	semSignal(&sem_id_all);



	pthread_exit(NULL);
}


/*
* Function for the customer to A level
*/
void *a(void *arg){


	// TODO
	/*
	 *	process the toA action.
	 *	print logs to show customers status (e.g., "customer is Crossing from B to A",
	 *  "customer to A should wait", “Finished Stairs")
	 */
	int i;
	struct T_thread_data *p_thread_data = arg;
	int tIDa = p_thread_data->thread_no;
	printf("****\nThread %d: customer would like to cross from B to A\n", tIDa);


	semWait(&sem_id_all);
	//first_run_time=time(&first_run_time);
	first_run_time=clock();
	//printf("A: first run %ld\n",  first_run_time);
	//printf("A: arrival time %ld\n",arrival_time);
	f_time_data[tIDa] = (int*)first_run_time;
	rt[rtCnt++] = f_time_data[tIDa] - a_time_data[tIDa];
	if(xingDir == 0){
		xingDir = 1;//upDir
		crossCntLock = 0;
		crossCntUnLock = 0;
		for(i=0;i<maxStairs;i++){
			semSignal(&sem_id_up);
		}
	}

	printf("Thread %d: There are %d customers waiting\n",tIDa,toAwaitCnt);
	toAwaitCnt++;
	semSignal(&sem_id_all);

	//ready to stair
	semWait(&sem_id_up);

	semWait(&sem_id_all);
	toAwaitCnt--;
	xingCnt++;
	crossCntLock++;
	semSignal(&sem_id_all);

	int pass_time = rand()%6+1;
	printf("Thread %d: customer is Crossing from B to A\n",tIDa);
	printf("Thread %d: Using Stairs\n",tIDa);
	printf("Thread %d: The customer will finish crossing in %d seconds\n",tIDa,pass_time);

	sleep(pass_time);

	printf("Thread %d: Finished Stairs. Waiting for mutex\n",tIDa);
	//completion_time=time(&completion_time);
	completion_time=clock();
	//printf("A: completion run %ld\n",  completion_time);
	//printf("A: arrival time %ld\n",arrival_time);
	c_time_data[tIDa]= (int*) completion_time;
	tat[tatCnt++] = c_time_data[tIDa]-a_time_data[tIDa];

	//semSignal(&sem_id_up);

	semWait(&sem_id_all);

	printf("Thread %d: Mutex Passed\n",tIDa);
	xingCnt--;
	xedCnt++;

	if(crossCntLock<=maxStairs || toBwaitCnt == 0){
		semSignal(&sem_id_up);
		crossCntUnLock++;
	}
	if(xingCnt == 0){
		if(toBwaitCnt == 0){
			xingDir = 0;
		}
		else{
			xingDir = 2;
			for(i=0;i<maxStairs;i++){
				semSignal(&sem_id_down);
			}
		}
		int diff = crossCntLock - crossCntUnLock;
		//printf("Thread %d: diff=[%d]\n",tIDa,diff);

		for(i=0;i<maxStairs-diff;i++){
			semWait(&sem_id_up);
		}
		crossCntLock = 0;
		crossCntUnLock = 0;
		printf("Thread %d: Crossing Direction Reset\n",tIDa);
	}

	printf("Thread %d: Signaling Mutex\n",tIDa);
	semSignal(&sem_id_all);





	pthread_exit(NULL);
}

// helper functions
/*
 *	Error-checked semaphore wait.
 */
void semWait(sem_t *sem) {
    if (sem_wait(sem) < 0) {
        perror("sem_wait");
        exit(EXIT_FAILURE);
    }
}

/*
 *	Error-checked semaphore signal.
 */
void semSignal(sem_t *sem) {
    if (sem_post(sem) < 0) {
        perror("sem_post");
        exit(EXIT_FAILURE);
    }
}

/*
 *	Expected test result:
 * Please print logs to show the customers' status for testing. Here is an example:

Project 2: Customer crossing problem using pThreads and Semaphores
*****************

Parent Process PID: 86672
****
Thread 0: About to cross
customer is Crossing from B to A
Crossing Finished
Signaling Mutex
Using Stairs
Finished Stairs . Waiting for mutex
Mutex Passed
Crossing Direction Reset
Signaling Mutex
****
Thread 1: About to cross
customer is Crossing from B to A
Crossing Finished
Signaling Mutex
Using Stairs
Finished Stairs . Waiting for mutex
Mutex Passed
Crossing Direction Reset
Signaling Mutex
****
Thread 2: customer Crossing from A to B
customer is Crossing from A to B
Signaling Mutex
There are 0 customers waiting
Using Stairs
The customer will finish crossing in 1 seconds
Finished Stairs. Waiting for mutex
Mutex Passed
Crossing Direction Reset
****

 */
