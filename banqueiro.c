#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

struct processo
{
    int * quantTotal;
    int * quantAlloc;
};

int **need;
int *avail;
int nro_processos,nro_recursos;
struct processo * p;


void libera();
void requisicao();
void showAll();
int randomico(int a);

int main(int argc, char **argv)
{
    int i,j;
    int a;

    if(argc > 5)
    {


        nro_processos = atoi(argv[2]);

        printf("Numero de Clientes\t %i \n", nro_processos);

        nro_recursos = argc - 4;

        printf("Numero de recursos\t %i\n", nro_recursos);

        p = malloc(sizeof(struct processo) * nro_processos);

        if(p == NULL){
            printf("Ocorreu um erro ao na alocaçao\n");
            exit(-1);
        }
        
        avail = (int *)malloc(sizeof(int) * (argc - 4));            
    
        printf("Quantidade de recurso INICIALMENTE disponível\n");

        for(i = 4, j = 0 ; i < argc; i++ , j++) {
            avail[j] = atoi(argv[i]);
            printf("%i ", avail[j]);
        }
        printf("\n");

        printf("Recursos TOTAL ainda necessários\n");
        for(i=0; i<nro_processos; i++)
        {
            p[i].quantTotal = malloc(sizeof(int) * nro_recursos);
            p[i].quantAlloc = malloc(sizeof(int) * nro_recursos);
            for(j=0; j<nro_recursos; j++)
            {
                p[i].quantTotal[j] = randomico(avail[j]);
                p[i].quantAlloc[j] = 0;
                printf("%d ",p[i].quantTotal[j]);
            }
            printf("\n");
        }


        printf("Quantidade de recurso Alocado\n");
        for(i=0; i<nro_processos; i++)
        {
            for(j=0; j<nro_recursos; j++)
            {

                int a = randomico(avail[j]);

                if(a <= p[i].quantTotal[j])
                {
                    p[i].quantAlloc[j] = a;
                    printf("%d ",p[i].quantAlloc[j]);
                    avail[j] = avail[j] - p[i].quantAlloc[j];
                }
                else
                    j--;
            }
            printf("\n");
        }

        printf("Quantidade de recursos que cada Cliente precisa\n");
        for(i=0; i<nro_processos; i++)
        {
            for(j=0; j<nro_recursos; j++)
            {

                printf("%d ",p[i].quantTotal[j] - p[i].quantAlloc[j]);
            }
            printf("\n");
        }
        printf("Quantidade de recurso FINAL disponível\n");

        for(i = 0; i < nro_recursos; i++)
            printf("%i ", avail[i]);
        printf("\n");



        showAll();
        requisicao();
        libera();
        return 0;
    }
}


int randomico(int a)
{

    sleep(1);
    int x;
    srand( (unsigned)time(NULL));

    x =  ( rand() % a );

    return x;
}


void showAll()
{
    int i,j;
    printf("Proc.\t Alocado\t Maximo\t Disp.\t");
    for(i=0; i<nro_processos; i++)
    {
        printf("\nP%d\t   ",i);
        for(j=0; j<nro_recursos; j++)
        {
            printf("%d ",p[i].quantAlloc[j]);
        }
        printf("\t");
        for(j=0; j<nro_recursos; j++)
        {
            printf("%d ",p[i].quantTotal[j]);
        }
        printf("\t");
        if(i==0)
        {
            for(j=0; j<nro_recursos; j++)
                printf("%d ",avail[j]);
        }
    }
}




void requisicao()
{
    int i,j;
    
    need = malloc (nro_processos * sizeof(int*));
  

    for(i=0; i<nro_processos; i++)
    {
        need[i] = (int*) malloc (nro_recursos * sizeof(int));

        for(j=0; j<nro_recursos; j++)
        {

            need[i][j]=p[i].quantTotal[j]-p[i].quantAlloc[j];
        }
    }
    printf("\n");

}


void libera()
{
    int i,j;
    int *finalizado,temp,flag=1,k,nro_processos_finalizados=0;

    finalizado = malloc(nro_processos * sizeof(int));

    for(i=0; i<nro_processos; i++)
    {
        finalizado[i]=0;
    }

    printf("\n\n");


    while(flag)
    {
        flag=0;

        for(i=0; i<nro_processos; i++)
        {
            int percorreu_todos_recursos=0;

            for(j=0; j<nro_recursos; j++)
            {
                if((finalizado[i]==0)&&(need[i][j] <= avail[j]))
                {
                    percorreu_todos_recursos++;
                    if(percorreu_todos_recursos == nro_recursos)
                    {
                        for(k=0; k<nro_recursos; k++)
                        {
                            avail[k]+=p[i].quantAlloc[j];
                            finalizado[i]=1;
                            flag=1;
                        }

                        printf("P%d->",i);
                        printf("Finalizado\n");
                        if(finalizado[i]==1)
                        {
                            i=nro_processos;
                        }
                    }
                }
            }
        }
    }

    for(i=0; i<nro_processos; i++)
    {
        if(finalizado[i]==1)
        {
            nro_processos_finalizados++;
        }
        else
        {
            printf("P%d->",i);
            printf("NaoFinalizado\n");
        }
    }




    if(nro_processos_finalizados==nro_processos)
    {
        printf("\n Estado seguro\n");
    }
    else
    {
        printf("\n Estado inseguro\n");
    }
}
