/* vim: set tabstop=2 expandtab: */
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define THREAD_ITER 10000

int var1 = 0;
int var2 = 0;

pthread_mutex_t mutex1;
pthread_mutex_t mutex2;


void* t1(void *p)
{
    int i;
    for(i=0; i < THREAD_ITER; i++)
    {
        // bledna kolejnosc mutexow, w
        // kazdym watku powinna byc identyczna
        pthread_mutex_lock(&mutex2);
        pthread_mutex_lock(&mutex1);
        var1++;
        printf("this will be %i disaster!\n", i);
        pthread_yield();
        var2++;
        pthread_mutex_unlock(&mutex2);
        pthread_mutex_unlock(&mutex1);
    }
}

void* t2(void *p)
{
    int i;
    for(i=0; i < THREAD_ITER; i++)
    {
        pthread_mutex_lock(&mutex1);
        pthread_mutex_lock(&mutex2);
        var1++;
        printf("or this will be %i disaster!\n", i);
        pthread_yield();
        var2++;
        pthread_mutex_unlock(&mutex2);
        pthread_mutex_unlock(&mutex1);
    }
}

/* gcc -g -lpthread basic_deadlock.c */
int  main(void)
{
    pthread_mutex_init(&mutex1, NULL);
    pthread_mutex_init(&mutex2, NULL);

    pthread_t thread1;
    pthread_t thread2;

    pthread_create(&thread1, NULL, &t1, NULL);
    pthread_create(&thread2, NULL, &t2, NULL);


    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("%d - %d: %d\n", var1, var2, THREAD_ITER * 2);

    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);

    pthread_exit(NULL);
    return 0;
}
