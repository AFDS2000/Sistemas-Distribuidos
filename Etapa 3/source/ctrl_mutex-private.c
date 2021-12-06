/* Grupo 31
 * André Cabaço 53457
 * André Seixas 53870
 * Miguel Agostinho 53475
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "ctrl_mutex-private.h"

static int countTicket = 1;
static int read_mutexcount = 0;

//Dedicados ao multi read_mutexers e writers
static pthread_mutex_t read_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t write_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

//Dedicado ao stats
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void lock_read(int ticket)
{
    pthread_mutex_lock(&read_mutex);

    //verifca se é a sua vez
    while (countTicket != ticket)
        pthread_cond_wait(&cond, &read_mutex);

    countTicket++;
    read_mutexcount++;

    //"Acorda" outras thread_mutexs
    pthread_cond_broadcast(&cond);

    //Caso existam reader a ler, o writer fica bloqueado.
    if (read_mutexcount == 1)
        pthread_mutex_lock(&write_mutex);

    pthread_mutex_unlock(&read_mutex);
}

void unlock_read()
{
    pthread_mutex_lock(&read_mutex);
    read_mutexcount--;

    //Quando não existir mais redears, desbloqueia o lock do writer.
    if (read_mutexcount == 0)
        pthread_mutex_unlock(&write_mutex);

    pthread_mutex_unlock(&read_mutex);
}

void lock_write(int ticket)
{
    pthread_mutex_lock(&write_mutex);

    //verifca se é a sua vez
    while (countTicket != ticket)
        pthread_cond_wait(&cond, &write_mutex);

    pthread_mutex_unlock(&write_mutex);
}

void unlock_write()
{
    pthread_mutex_lock(&write_mutex);
    countTicket++;

    //"Acorda" outras thread_mutexs
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&write_mutex);
}

void lock_stats()
{
    pthread_mutex_lock(&mutex);
}

void unlock_stats()
{
    pthread_mutex_unlock(&mutex);
}