/**
 * @file philosophers.c
 * @author Joan Sebastian Tuquerrez Gomez <jtuquerrez@unicauca.edu.ci>
 * @brief Solucion al problema de los filosofos
 * @version 0.1
 * @date 2023-05-11
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>

/**
 * @brief Variables_Del_Sistema
 * 
 */
#pragma region Variables_Del_Sistema
#define MAX_CYCLES 5
#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define down sem_wait;
#define up sem_post;
#define LEFT (i + N - 1) % N
#define RIGHT (i + 1) % N
typedef sem_t semaphore;
semaphore mutex;
semaphore *s;
int *state, N;
#pragma region 

/**
 * @brief Hilo creador de los filosofos
 * 
 * @param arg argumento al hilo (no usado)
 * @return void* valor de retorno (no usado)
 */
void *philosopher(void *arg);

/**
 * @brief Tomar los tenedores o el turno para comer
 * 
 * @param i Filosofo que tomara los tenedores
 */
void take_forks(int i);

/**
 * @brief Dejar los tenedores o el turno para comer
 * 
 * @param i Filosofo que dejara los tenedores
 */
void put_forks(int i);

/**
 * @brief Verificar si el filosofo puede comer
 * 
 * @param i Filosofo que se verificara
 */
void test(int i);

int main(int argc, char *argv[])
{
    int finished, i;
    pthread_t *t; // hilos

    // Asignar el valor de N
    N = atoi(argv[1]);
    if(N <= 2){
        printf("El numero de filosofos debe ser mayor a 2\n");
        exit(EXIT_FAILURE);
    }

    // iniciar los semaforos
    sem_init(&mutex, 0, 1);
    s = malloc(N * sizeof(semaphore));
    state = malloc(N * sizeof(int));
    for (i = 0; i < N; i++)
    {
        sem_init(&s[i], 0, 0);
        state[i] = THINKING;
    }

    // crear hilos
    t = malloc(N * sizeof(pthread_t));
    printf("Creando hilos...\n");
    for (i = 0; i < N; i++)
    {
        pthread_create(&t[i], NULL, philosopher, (void *)(__intptr_t)i);
    }

    // esperar a que los hilos terminen
    for (i = 0; i < N; i++)
    {
        pthread_join(t[i], NULL);
    }

    // liberar memoria
    free(t);
    free(s);
    free(state);

    exit(EXIT_SUCCESS);
}

void *philosopher(void *arg)
{
    __intptr_t i = (__intptr_t)arg;
    int j = 0;
    while (j < MAX_CYCLES)
    {
        printf("%d\n", j);
        printf("Philosopher %ld is thinking\n", i);
        sleep(1);
        take_forks(i);
        printf("Philosopher %ld is eating\n", i);
        sleep(2);
        put_forks(i);
        j++;
    }
    pthread_exit(NULL);
}

void take_forks(int i)
{
    down(&mutex);
    state[i] = HUNGRY;
    printf("Philosopher %d is hungry\n", i);
    test(i);
    up(&mutex);
    down(&s[i]);
}

void put_forks(int i)
{
    down(&mutex);
    state[i] = THINKING;
    printf("Philosopher %d is thinking\n", i);
    test(LEFT);
    test(RIGHT);
    up(&mutex);
}

void test(int i)
{
    if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING)
    {
        state[i] = EATING;
        printf("Philosopher %d is eating\n", i);
        up(&s[i]);
    }
}