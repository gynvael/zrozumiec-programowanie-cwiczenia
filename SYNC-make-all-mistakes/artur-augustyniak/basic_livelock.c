/* vim: set tabstop=2 expandtab: */
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define THREAD_ITER 100

int var1 = 0;
int var2 = 0;
int intentionally_unsafe_counter = 0;
pthread_mutex_t mutex1;
pthread_mutex_t mutex2;


void* t1(void *p)
{
    int i;
    for(i=0; i < THREAD_ITER; i++)
    {
        pthread_mutex_lock(&mutex1);

        while(pthread_mutex_trylock(&mutex2))
        {
            intentionally_unsafe_counter++;
            printf("Me be T1. Me be bored approx. %i times!\n", intentionally_unsafe_counter);

        }
        var1++;
        var2++;
        pthread_mutex_unlock(&mutex2);

        /**
         * UPS!
         */
        //pthread_mutex_unlock(&mutex1);
        printf("T1 done.\n");
    }
}

void* t2(void *p)
{
    int i;
    for(i=0; i < THREAD_ITER; i++)
    {
        pthread_mutex_lock(&mutex2);
        while(pthread_mutex_trylock(&mutex1))
        {
            intentionally_unsafe_counter++;
            printf("Me be T2. Me be bored approx. %i times!\n", intentionally_unsafe_counter);

        }
        var1++;
        var2++;
        pthread_mutex_unlock(&mutex1);
        pthread_mutex_unlock(&mutex2);
        printf("T2 done.\n");
    }
}

/* gcc -g -lpthread basic_livelock.c */
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

    /* Last thing that main(); should do */
    pthread_exit(NULL);
    return 0;
}
