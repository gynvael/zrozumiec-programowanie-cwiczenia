/* vim: set tabstop=2 expandtab: */
#include<stdio.h>
#include<unistd.h>
#include<semaphore.h>
#include<pthread.h>

int X, Y;
int r1, r2;

sem_t begin_sem1;
sem_t begin_sem2;
sem_t end_sem;


void* t1(void *p)
{
    for(;;)
    {
        sem_wait(&begin_sem1);
        X = 1;
#if USE_MFENCE
        asm volatile("mfence" ::: "memory");
#endif
        r1 = Y;
        sem_post(&end_sem);
    }
}

void* t2(void *p)
{
    for(;;)
    {
        sem_wait(&begin_sem2);
        Y = 1;
#if USE_MFENCE
        asm volatile("mfence" ::: "memory");
#endif
        r2 = X;
        sem_post(&end_sem);
    }

}

/*
 * Ten kod to uproszczona wersja kodu w pochodzacego z arta:
 * http://preshing.com/20120515/memory-reordering-caught-in-the-act/
 *
 * gcc -std=gnu99 -lpthread x86_out_of_order_exec.c  && ./a.out
 *
 * https://en.wikipedia.org/wiki/Out-of-order_execution
 * Bez bariery store/load na x86, pomimo "synchronizacji" semaforami
 * kolejnosc wykonania instrukcji moze byc rozna jesli z pnktu widzenia
 * danego core'a nie wplywaja na siebie.
 *
 * Ten problem nie wystapi przy jednym rdzeniu (nawet przy wywlaszczaniu).
 *
 * Jeśli nie stosować innych innych prymitywów synchronizacji, na x86 "popsuty"
 * bedzie Peterson Lock (software'owy mutex, nie wiem czy to można tak nazwac).
 * https://en.wikipedia.org/wiki/Peterson's_algorithm
 * Naprawic go mozna uzywajac wlasnie mfence (https://peeterjoot.wordpress.com/tag/mfence/):
 *
 * FIX:
 * gcc -DUSE_MFENCE -std=gnu99 -lpthread x86_out_of_order_exec.c  && ./a.out
 */
int  main(void)
{

    sem_init(&begin_sem1, 0, 0);
    sem_init(&begin_sem2, 0, 0);
    sem_init(&end_sem, 0, 0);

    pthread_t thread1;
    pthread_t thread2;
    pthread_create(&thread1, NULL, &t1, NULL);
    pthread_create(&thread2, NULL, &t2, NULL);

    int d, it = 0;
    printf("Waiting 2 seconds. Hit ctrl-C to exit:\n");
    sleep(2);
    printf("Start\n");
    for (;;)
    {
        it++;
        X = 0;
        Y = 0;
        sem_post(&begin_sem1);
        sem_post(&begin_sem2);
        sem_wait(&end_sem);
        sem_wait(&end_sem);
        /*
         * Jesli nie bylo by reorderow to zawsze r1 lub r2 == 1
         */
        if (r1 == 0 && r2 == 0)
        {
            d++;
            printf("Iter: %d - reorders: %d\n", it, d);
        }
    }
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_exit(NULL);
    return 0;
}
