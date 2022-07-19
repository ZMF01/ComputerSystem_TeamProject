# CS5600-project2
# Ovierview
This is the second team project which involves using the semaphores and/or mutex locks to avoid 
deadlock in a multi-threaded program.
A department store consists of two floors that are connected by narrow stairs 
that cannot fit more than one person on any given stair. The stairs can become 
deadlocked if both a first-floor customer is trying to go up to the second floor, 
while a second-floor customer is trying to go down to the first floor. Customers 
in such situation refuse to backup leading to deadlock. 
The store manager is hiring you to design a C program using POSIX 
synchronization, that 
1. prevents deadlock
2. prevents starvation (the situation in which first-floor customers 
prevent second-floor customers from using the stairs, or vice versa)
3. and allows more than one customer to use the stairs in the same direction in an 
“efficient” way that you determine.
Represent first-floor customers and second-floor customers as separate threads. Once a 
customer is on the stairs, the associated thread will sleep for a random period of time, 
representing the time to go over all the 13 stairs between the two floors.
Design your program so that you can create several threads representing first-floor 
customers and second-floor customers, and test it using different possible scenarios.
Find the Turnaroud time and Response time for each customer

2- Documenting your project
In a readme.md file, include the following:
1. Your group number, and the name of the students in your group 
2. A short description of the project.
3. Explain how you implemented the project:
List the functions you wrote, and the purpose of each function
Explain how you tested your project and list the test cases you used 
Explain how you are guaranteeing that your code is free of deadlock and starvation. 
Find the average Turnaroud time and Response time of the examples you run, and 
explain using these performance measures how you adjusted your project to make your 
design “efficient”.
Explain how we can compile, run and test your code. 
List the contributions of each student in your group

3- Submitting your work
You should place all relevant files to the project in a folder and create a zip file by zipping the 
folder itself. You should not include object files or executable binary files that are generated 
during the compilation process. The zip file should be submitted via Canvas. Only one of the 
team members may submit the work: if one member uploaded the file on Canvas, the other team 


# Report
## Group 5: Bernard Ekezie, Shuyi Wang, Mengfei Zhang
## Description 
1. We design three semaphores to prevent deadlock:   
sem_t sem_id_all; //to block the whole stairs   
sem_t sem_id_up; //to block the go-up stair   
sem_t sem_id_down; //to block the go-down stair   

2. The Line-up Strategy: (Seen the stair as a turnable with going up/down direction) 
In the beginning, all the customers(threads) try to grab the sem_id_all semaphore, 
and the one take it will then determine the current direction of the stair. 
If the status of stair with no direction, then start the stair with the customer's direction 
and increment the count of the queue otherwise, wait for the stair 
After each customer finish crossing, determine the count on the stairs and the count at the opposite, 
if the count on the stair and at the opposite is zero: stop the stair. 
if the count on the stair is zero but the count of the opposite is non-zero: turn the stair. 
(prevent starving) 

3. Design crossCntLock and crossCntUnLock to prevent starving and be more efficient:
int crossCntLock; //After each stair turnover, the count of customers being passing through stair(owning locks)   
int crossCntUnLock; //After each stair turnover, the count of customers unlocking locks   
We design that when crossCntLock > maxStairs(the max customer can be on the stairs at the same time),
no longer accept unlocking lock, and turnover the stair.
Therefore, if there's too many customers waiting at each side, we can let each direction 
pass a few customers to prevent starving

4. Our default setting:
We design there's 10 customers want to use the stairs at the same time.   
The thread with odd number want to go downstairs (A->B)   
The thread with even number want to go upstairs (B->A)   
Each of the customers passing time is randomly created, the max number that can be on the stair is three.
We expected the have them passing in groups

## How we implemented the project:
### List the functions you wrote, and the purpose of each function
1. void *a(void *arg): Customers going toward A (i.e. up) 
2. void *b(void *arg): Customers going toward B (i.e. down)

3. void semWait(sem_t *sem): Semaphore wait functions
4. void semSignal(sem_t *sem) Semaphore signal functions

5. main()
### Explain how you tested your project and list the test cases you used 
How we test project:
1. Used Valgrind for memory debugging, memory leak detection, and profiling. 
2. Used various test cases and print statements to check for unclear results, 
and segmentation fault errors.

Test cases included:
1. Checking if we get the correct number of threads creations.
2. Making sure each thread was chooses at random for each function call by using conditional checks.
3. Check that each thread is waiting while resource is in use.
4. Check to see that every thread was treated fairly in regards to access time with resource.
5. Check switching function calls for a and b ran smoothly once all threads successfully finished using resources. 
6. Check customer max limit on stair by using a variety of thread tNum and maxStair totals.
7. Ran a number of trials to check for average turnaround and response time making sure each run was gave accurate data.
8. A number of checks for preventing any deadlocks and starvations by using semiphores and manipulating thread count.

### Explain how you are guaranteeing that your code is free of deadlock and starvation. 
In the beginning, all the customers(threads) try to grab the sem_id_all semaphore, 
and the one take it will then determine the current direction of the stair. 
If the status of stair with no direction, then start the stair with the customer's direction 
and increment the count of the queue otherwise, wait for the stair 
After each customer finish crossing, determine the count on the stairs and the count at the opposite, 
if the count on the stair and at the opposite is zero: stop the stair. 
if the count on the stair is zero but the count of the opposite is non-zero: turn the stair. 
### Find the average Turnaroud time and Response time of the examples you run, and explain using these performance measures how you adjusted your project to make your design “efficient”.
Output1:  
Average Turnaround time = 1.6s  
Average Response time = 91.9ms  
Output2:  
Average Turnaround time = 1.3s  
Average Response time = 76.5ms   
Design crossCntLock and crossCntUnLock to prevent starving and be more efficient:
int crossCntLock; //After each stair turnover, the count of customers being passing through stair(owning locks)
int crossCntUnLock; //After each stair turnover, the count of customers unlocking locks
We design that when crossCntLock > maxStairs(the max customer can be on the stairs at the same time),
no longer accept unlocking lock, and turnover the stair.
Therefore, if there's too many customers waiting at each side, we can let each direction 
pass a few customers to prevent starving
### Explain how we can compile, run and test your code. 
Compile & Run Code
1. Run the command 'gcc -v' to check if you have a compiler installed.
2. Pulled our project from github repo and store it in our system.
3. Change the working directory to where C program located in system.
4. Used Makefile commands make all and make clean to compile, run and remove compiled output file. 

Testing
1. Used Valgrind for memory debugging, memory leak detection, and profiling. 
2. Used various test cases and print statements to check for unclear results,
and segmentation fault errors.
### List the contributions of each student in your group
Bernard Ekezie: coding, report writing.  
Shuyi Wang: coding, report writing.  
Mengfei Zhang: coding, report writing.  
