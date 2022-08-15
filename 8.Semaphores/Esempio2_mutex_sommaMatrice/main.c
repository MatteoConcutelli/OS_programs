#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define N 20
#define M 50


pthread_mutex_t mut; /* mutex condiviso tra threads */
int a[N][M];
int sommaMat=0;

void *sommaRiga_th(void *arg){
	int i=(int)arg;
	int sommaRiga=0;
	
	for(int j=0;j<M;j++){
		sommaRiga+=a[i][j];
	}	
	
	pthread_mutex_lock(&mut); /* prologo sez. critica */
	
	sommaMat += sommaRiga;
	printf("thread %d: sommaRiga=%d sommaMatrice=%d\n",i,sommaRiga,sommaMat);
	sleep(1);
	
	pthread_mutex_unlock(&mut); /* epilogo sez. critica */
	pthread_exit(0);
}

int main () {
	int i,j;
	pthread_t th[N];
	pthread_mutex_init (&mut,NULL);
	
	srand(time(NULL)); // seme per random
	for (i=0;i<N;i++){
		for (j=0;j<M;j++)
			a[i][j]=rand()%256;	
	}
	for (i=0;i<N;i++){
		if (pthread_create(&th[i],NULL,sommaRiga_th,(int *)i)!=0){
			fprintf (stderr, "errore create thread i \n");
			exit(1);
		}
	}
	
	for (i=0;i<N;i++){
		pthread_join(th[i],NULL);
		printf("	thread %d terminato\n",i);
	}
		
	printf("---SommaMatrice = %d \n",sommaMat);
}
