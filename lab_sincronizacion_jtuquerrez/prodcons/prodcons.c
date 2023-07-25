/**
 * @file prodcons.c
 * @author Joan Sebastian Tuquerrez Gomez <jtuquerrez@unicauca.edu.ci>
 * @brief  Solucion al problema del productor consumidor
 * @version 0.1
 * @date 2023-05-11
 * 
 * @copyright Copyright (c) 2023
 */
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>

/**
 * @brief Variables_alias_del_sistema
 */
#pragma region Variables_alias_del_sistema
typedef sem_t semaphore;
#define down sem_wait;
#define up sem_post;
semaphore mutex; //= 1;
semaphore empty; //= N;
semaphore full;  // 0;
#pragma endregion

/**
 * @brief Hilo productor
 *
 * @param arg Argumento al hilo (no usado)
 * @return void* Valor de retorno (no usado)
 */
void *producer(void *arg);

/**
 * @brief Hilo consumidor
 *
 * @param arg Argumento al hilo (no usado)
 * @return void* Valor de retorno (no usado)
 */
void *consumer(void *arg);

/**
 * @brief Produce un item
 *
 * @return int Item producido
 */
int produce_item();

/**
 * @brief Inserta un item al buffer
 *
 * @param item Item a insertar
 */
void insert_item(int item);

/**
 * @brief Remueve un item del buffer
 *
 * @return int Item removido
 */
int remove_item();

/**
 * @brief Consume un item
 *
 * @param item Item a consumir
 */
void consume_item(int item);

int main(int argc, char *argv[]){
    int finished, N;
    pthread_t t_prod, t_cons;
    if (argc != 2)
    {
        printf("Ingreso el tamaño del buffer como segundo argumento \n");
        exit(EXIT_FAILURE);
    }
    N = atoi(argv[1]);
    if (N <= 0)
    {
        printf("N Ingrese un N mayor que 1 \n");
        exit(EXIT_FAILURE);
    }
    
    //iniciar los semaforos
    sem_init(&mutex,0,1);
    sem_init(&empty,0,N);
    sem_init(&full,0,0);

    //iniciar los hilos
    finished = 0;
    //crear los hilos
    printf("Creating threads \n");
    pthread_create(&t_prod, NULL, producer, (void*)&finished);
    pthread_create(&t_cons, NULL, consumer, (void*)&finished);

    //esperar a que terminen
    printf("waiting for threads \n");
    sleep(2);
    printf("finishing \n");
    finished = 1;

    //Esperar a que t_prod o el productor termine
    pthread_join(t_prod, NULL);
    //Esperar a que t_cons o el consumidor termine
    pthread_join(t_cons, NULL);

    printf("simulation termined \n");
    exit(EXIT_SUCCESS);
}

void * producer(void * arg)
{
    int item, *finished;
    finished = (int*)arg;
    while (*finished == 0)
    {
        item = produce_item();
        down(&empty);
        down(&mutex);
        insert_item(item);
        up(&mutex);
        up(&full);
    }
}

void * consumer(void * arg)
{
    int item, *finished;
    finished = (int *)arg;
    while (*finished == 0)
    {
        down(&full);
        down(&mutex);
        item = remove_item();
        up(&mutex);
        up(&empty);
        consume_item(item);
    }
}

int produce_item(){
    printf(" P ");
    fflush(stdout);
    usleep(rand() % 1000);
    return -1;
}

void insert_item(int item){
    printf(" I ");
    fflush(stdout);
};

int remove_item(){
    printf(" R ");
    fflush(stdout);
    return -1;
}

void consume_item(int item){
    printf(" C ");
    fflush(stdout);
    usleep(rand() % 1000);
}
