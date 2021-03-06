#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int numOfThreads = 11;
int counter = 0;
int* buf;
int sizeOfBuf = 100;
pthread_rwlock_t rwlock;

void* threadWrite(){
	while(1){
		pthread_rwlock_wrlock(&rwlock);
		++counter;
		if(counter < sizeOfBuf) buf[counter] = counter;
		printf("СЧЕТЧИК == %d\n", counter);
		fflush(stdout);
		pthread_rwlock_unlock(&rwlock);
		sleep(2);
	}
}

void* threadRead(){
	while(1){
		pthread_rwlock_rdlock(&rwlock);

		printf("Идентификатор потока: %ld. Размер массива: %d\n", (long)pthread_self(), 
				buf[counter]);
		fflush(stdout);
		pthread_rwlock_unlock(&rwlock);
		sleep(rand() % 10);
	}
}

int main(){
	buf = (int*)calloc(sizeOfBuf, sizeof(int));
	
	int i;
	pthread_t threads[numOfThreads];

	pthread_rwlock_init (&rwlock, NULL);


	for(i = 0; i < 1; i++){
		pthread_create(&threads[i], NULL, threadWrite, NULL);
	}
	for( ; i < numOfThreads; i++){
		pthread_create(&threads[i], NULL, threadRead, NULL);
	}

	for(i = 0; i < numOfThreads; i++){
		pthread_join(threads[i], NULL);
	}

	pthread_rwlock_destroy(&rwlock);

	free(buf);
	return 0;
}
