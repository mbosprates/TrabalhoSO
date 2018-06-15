#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include "Dados.h"
#include "Processos.h"
#include "Banqueiro.h"

/* Semáforos */

sem_t mutex_init;
sem_t mutex_res;

int *avail;
int nro_processos,nro_recursos;

void * threadsTest(void *);

int main(int argc, char **argv)
{
    int i,j;
    int a;

    pthread_t *tid;

    if(argc > 5)
    {
        nro_processos = atoi(argv[2]);

        printf("Numero de Clientes:\t %i \n", nro_processos);

        nro_recursos = argc - 4;

        printf("Numero de recursos:\t %i\n", nro_recursos);

        avail = (int *)malloc(sizeof(int) * (argc - 4));            

        printf("Quantidade de recurso INICIALMENTE disponível:\n");

        for(i = 4, j = 0 ; i < argc; i++ , j++) {
            avail[j] = atoi(argv[i]);
            printf("%i ", avail[j]);
        }
        printf("\n");

        int verifica = init_Dados(avail, nro_processos, nro_recursos);
        if( verifica == 0)
        {
               fprintf(stderr," falha na alocação");
        }

        sem_init(&mutex_init, 0, 1);
        sem_init(&mutex_res, 0, 1);

        tid = (pthread_t *) malloc(sizeof(pthread_t) * nro_processos);

        for(i = 0; i < nro_processos; i ++){
            //create threads
            pthread_create(&tid[i], NULL,threadsTest, &i);
        }

        for(i = 0; i < nro_processos; i ++){
            //espera as threads
            pthread_join(tid[i], NULL);
        }
    }
}

void * threadsTest(void * arg){
    int p, i;
    int rec[nro_recursos];

    sem_wait(&mutex_init);

    p = init_Thread(avail);
    if(p == -2)
    {
           fprintf(stderr," falha na alocação");
    }        

    //printf("TOTAL de Recursos para terminar o processo:\n");
    printf("P%d:\n", p);
    //need(p);
    sem_post(&mutex_init);


    while(1){
        //Requisitar recurso
        sem_wait(&mutex_res);
        printf("P%d", p);
        if(requisicao_recursos(p,dados->processo[p].quantTotal) == -1){
            sem_post(&mutex_res);
            printf("P%d Morreu!\n", p);
            pthread_exit(NULL);
        }
        sem_post(&mutex_res);

        //Dormir sleep(random() % 3);
        sleep(rand() % 3);

        //Liberar um subconjunto de recursos alocados
        sem_wait(&mutex_res);
        for(i = 0; i < nro_recursos; i ++){
            rec[i] = randomico(avail[i]);
        }
        printf("P%d", p);
        libera_recursos(p,rec);
        sem_post(&mutex_res);
        //Dormir sleep(random() % 3);
        sleep(rand() % 3);
    }
}

