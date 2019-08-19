#include <stdio.h>
#include <sys/mman.h>
#include <native/task.h>
#include <native/timer.h>
#include <native/sem.h>
#include <native/mutex.h>
#include <native/heap.h>
#include <rtdk.h>
#include <sys/io.h>

void seandToHeap(void *arg){
    int inputExTimes,inputStaTimes,inputDeadlTimes,i;

    printf("Enter the time execute for 1 tasks.\n ");
    scanf("%d", &inputExTimes);

    printf( " Enter the ime start for 1 tasks.\n ");
    scanf("%d", &inputStaTimes);

    printf(" Enter the time deadline for 1 tasks.\n ");

	scanf("%d",&inputDeadlTimes);

    RT_HEAP heap;
    rt_heap_create(&heap,"myheap",100,H_SHARED|H_PRIO);
   
    RT_MUTEX mutex;
    rt_mutex_create(&mutex,"mymutex");

    char* allTime;
    rt_heap_alloc(&heap,0,TM_INFINITE,(void**)&allTime);

    rt_mutex_acquire(&mutex,TM_INFINITE);
    allTime[0]=inputExTimes;
    allTime[1]=inputStaTimes;
    allTime[2]=inputDeadlTimes;
    rt_mutex_release(&mutex);
}
int main(int argc, char *argv[])
{
    rt_print_auto_init(1);
    mlockall(MCL_CURRENT|MCL_FUTURE);
    RT_TASK seandToHeapTask;
    rt_task_create(&seandToHeapTask,"seandToHeapTask",0,50,0);
    rt_task_start(&seandToHeapTask,&seandToHeap,NULL);
    pause();
}

