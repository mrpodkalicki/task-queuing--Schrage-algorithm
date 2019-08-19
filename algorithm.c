#include <stdio.h>
#include <sys/mman.h>
#include <native/task.h>
#include <native/timer.h>
#include <native/sem.h>
#include <native/mutex.h>
#include <native/heap.h>
#include <rtdk.h>
#include <sys/io.h>

RT_SEM sem;

int addextime;
int addstartime;
int adddeadltime;

void takeFromHeap(void *arg)
{
	int i;
	RT_HEAP heap;
	rt_heap_bind(&heap, "myheap",TM_INFINITE);

	RT_MUTEX mutex;
	rt_mutex_bind(&mutex,"mymutex",TM_INFINITE);

	char lastmod=255;
	char* all;
	char* allTasks;
	rt_heap_alloc(&heap,0,TM_INFINITE,(void**)&allTasks);
	rt_mutex_acquire(&mutex,TM_INFINITE);
	rt_printf("cos%d:",allTasks[0]);
	extern int addextime;
	extern int addstartime;
	extern int adddeadltime;
	addextime=allTasks[0];
	addstartime=allTasks[1];
	adddeadltime=allTasks[2];
	rt_mutex_release(&mutex);
	rt_heap_unbind(&heap);
	rt_heap_delete(&heap);
}
int findIndex(int array[100], int value, int sizeArray)
{
	int OutIndex, i;
	for (i = 0; i < sizeArray; i++)
	{
		if (array[i] == value)
		{
			OutIndex = i + 1;
		}
	}
	return OutIndex;
}
int *delete (int arrayDel[100], int index, int sizeArray)
{
	int i;
	for (i = index - 1; i < sizeArray - 1; i++)
	{
		arrayDel[i] = arrayDel[i + 1];
	}
	for (i = 0; i < sizeArray - 1; i++)
	{
		arrayDel[i] = arrayDel[i];
	}
	return arrayDel;
}
int *min(int array[100], int sizeArraymMin, int indexMin[100])
{
	int minimum, location;
	int i;
	int static arrayMin[2];
	minimum = array[indexMin[0] - 1];
	location = indexMin[0];
	for (i = 0; i < sizeArraymMin; i++)
	{
	}
	location = indexMin[0];
	for (i = 1; i < sizeArraymMin; i++)
	{
		if (array[indexMin[i] - 1] < minimum)
		{
			minimum = array[indexMin[i] - 1];
			location = indexMin[i];
		}
	}
	arrayMin[0] = location;
	arrayMin[1] = minimum;
	return arrayMin;
}

char *split(char array[100], int amountElements)
{
	int i;
	char separator[] = ",";
	char *SingleElement = strtok(array, separator);
	for (i = 0; i < amountElements; i++)
	{
		array[i] = atoi(SingleElement);
		SingleElement = strtok(NULL, separator);
	}
	return array;
}

int *strainer(int arrayZ[100], int sizeArray, int index[100], int t)
{
	int z, i;
	z = 0;
	static int setZindex[100];
	setZindex[0] = z;
	for (i = 0; i < sizeArray; i++)
	{
		if (arrayZ[index[i] - 1] <= t)
		{
			setZindex[z + 1] = index[i];
			z++;
		}
	}
	setZindex[0] = z;
	return setZindex;
}

int *algorithmSchrager(int arExecuteTimes[100],int arStartTimes[100],int arDeadlimeTimes[100],int amountTasks )
{
	int setA[amountTasks], k,i;
	int setZ[amountTasks], size, T, j;
	int *IndexJ, startTimes[amountTasks], endingTimes[amountTasks], orderTasks[amountTasks];
	static int outAllTime[100];

	for (i = 0; i < amountTasks; i++)
	{
		setA[i] = i + 1;
	}
	printf("A=:{");
	for (i = 0; i < amountTasks; i++)
	{
		printf("%d,", setA[i]);
	}
	printf("}");
	int *time = min(arStartTimes, amountTasks, setA);
	int t = time[1];
	printf("t:%d", t);
	size = amountTasks;
	orderTasks[0] = amountTasks;
	for (k = 1; k <= amountTasks; k++)
	{
		int j, setZ[size];
		printf("\n");
		printf("k %d\n", k);
		int *elementZ = strainer(arStartTimes, size, setA, t);
		printf("Z:{");
		for (i = 1; i <= elementZ[0]; i++)
		{
			printf("%d,", elementZ[i]);
		}
		printf("},");
		// printf("z:%d",elementZ[0]);
		if (elementZ[0] != 0)
		{
			for (i = 0; i <= elementZ[0]; i++)
			{
				setZ[i] = elementZ[i + 1];
			}
			IndexJ = min(arDeadlimeTimes, elementZ[0], setZ);
			j = IndexJ[0];
			T = t;
			t = t + arExecuteTimes[j - 1];
		}
		else
		{
			IndexJ = min(arStartTimes, size, setA);
			j = IndexJ[0];
			t = arStartTimes[j - 1];
			T = t;
			t = t + arExecuteTimes[j - 1];
		}
		printf("j:(%d),", j);
		int delJ = findIndex(setA, j, size);
		delete (setA, delJ, size);
		printf("T:(%d),", T);
		printf("A=:{");
		size--;
		for (i = 0; i < size; i++)
		{
			printf("%d,", setA[i]);
		}
		printf("}");
		printf(",t:(%d)", t);
		printf("\n");
		startTimes[k - 1] = T;
		endingTimes[k - 1] = t;
		orderTasks[k - 1] = j;
	}
	int c=0;
	for (i = 0; i <=amountTasks ; i++)
	{
		outAllTime[c]=orderTasks[i];
		outAllTime[c+1]=startTimes[i];
		outAllTime[c+2]=endingTimes[i];
		c+=3;
	}
	return outAllTime;
}
void fun1(void *arg)
{	
	int i, c, sek;
	rt_sem_p(&sem, TM_INFINITE);
	RT_TASK zadania;
	int zadno;
	char str[10];
	extern int allendtTask[100];
	extern int allstartTask[100];
	extern int amountTaskglob;
	extern int amountGlob;
	int start=allstartTask[*((int*)(arg))-1];
	int stop=allendtTask[*((int*)(arg))-1];
	rt_printf("s(%d)",start);
	rt_printf("e(%d)",stop);
	RTIME czas =  100000000LL*start;
		rt_printf("czas-zero(%d)",czas);
		while (czas <1000000000LL*start )
		{
			rt_timer_spin(100000000LL);
			czas = czas + 100000000LL;
		}
		rt_printf("czas(%d)",czas);
		rt_printf(" KONIEC %d\n", *((int*)(arg)));
}

int allendtTask[100];
int allstartTask[100];
int amountGlob;
int endtTask;
int amountTaskglob;
int start;
int stop;
int main(int argc, char *argv[])
{
	rt_print_auto_init(1);
	mlockall(MCL_CURRENT | MCL_FUTURE);
	int i, amountTasks;
	char str[10], inputExTimes[100], inputStaTimes[100], InputDeadlTimes[100];

	printf("Enter the number of tasks:");
	scanf("%d", &amountTasks);

	printf("Enter the time execute for %d tasks.\n ", amountTasks);
	scanf("%s", &inputExTimes);

	printf("Enter the time start for %d tasks.\n ", amountTasks);
	scanf("%s", &inputStaTimes);

	printf("Enter the time deadline for %d tasks.\n ", amountTasks);
	scanf("%s", &InputDeadlTimes);

	int exeTimes[amountTasks], deadlTimes[amountTasks], starTimes[amountTasks];
	int setA[amountTasks], k;
	int setZ[amountTasks], size, T, j;
	int *IndexJ, startTimes[amountTasks], endingTimes[amountTasks], orderTasks[amountTasks];
	int priorityTask[amountTasks];
	int *timeStartStop[amountTasks * 2 + 1];
	char *separateExecuteTimes = split(inputExTimes, amountTasks);
	for (i = 0; i < amountTasks; i++)
	{
		exeTimes[i] = (separateExecuteTimes[i]);
	}
	char *separateStartTasksTimes = split(inputStaTimes, amountTasks);
	for (i = 0; i < amountTasks; i++)
	{
		starTimes[i] = separateStartTasksTimes[i];
	}
	char *separateDeadlineExecuteTim = split(InputDeadlTimes, amountTasks);
	for (i = 0; i < amountTasks; i++)
	{
		deadlTimes[i] = (separateDeadlineExecuteTim[i]);
	}
	mlockall(MCL_CURRENT | MCL_FUTURE);
	rt_print_auto_init(1);
	rt_sem_create(&sem, "semafor", 0, S_FIFO);
	rt_timer_set_mode(0);
    RT_TASK takefromHeapTask;
    rt_task_create(&takefromHeapTask,"takefromHeapTask",0,10,0);
    rt_task_start(&takefromHeapTask,&takeFromHeap,NULL);
	if(addextime!=0){
		exeTimes[amountTasks]=addextime;
		starTimes[amountTasks]=addstartime;
		deadlTimes[amountTasks]=adddeadltime;
		amountTasks=amountTasks+1;
	}
	int *allTimes =algorithmSchrager(exeTimes,starTimes,deadlTimes,amountTasks);
	int c=0;
	for (i = 0; i <amountTasks; i++){

		orderTasks[i]=allTimes[c];
		c+=3;
		// printf("order:(%d)",orderTasks[i]);
	}
	amountTaskglob=amountTasks;
	int z=1;
	for (i = 0; i <amountTasks; i++){
		
		int p=0;
		for(c=0; c <amountTasks;c++ )
		{
			if(allTimes[p]==i+1)
			{
				allstartTask[i]=allTimes[p+1];
				allendtTask[i]=allTimes[p+2];
				
			}
			p=p+3;
	}	}
	c = 1;
	int lengthTime = amountTasks + amountTasks * 2;
	RT_TASK zadania[amountTasks];
	int zadno[amountTasks];
	
	for (i = 0; i <amountTasks; i++)
	{
		rt_printf("\ntworzenie zadania %d\n", i + 1);
		sprintf(str, "zadanie%d", i);
		rt_task_create(&zadania[i], str, 0, 50, 0);
		amountGlob=i;
		zadno[i] = i+1 ;
		rt_task_start(&zadania[i], &fun1, &zadno[i]);
	}
	priorityTask[0] = 1;
	for (i = 0; i < amountTasks; i++)
	{
		int indexTask = findIndex(orderTasks, i+1 , amountTasks);
		printf("nrvalue:(%d),value:(%d)",i+1,indexTask);
		rt_task_set_priority(&zadania[i], amountTasks * 10 + 10 - indexTask * 10);
	}
	rt_sem_broadcast(&sem);
	pause();
	printf("\n");


}



