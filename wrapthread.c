#include "wrapthread.h"
#include "error.h"



void Pthread_create(pthread_t *tid , const pthread_attr_t *attr, void *(*func)(void *), void *arg)
{
        int n;

        if ( (n = pthread_create(tid,attr,func,arg) ) == 0 )
            return;
        errno = n;
            err_sys("pthread_create error");
}

void Pthread_join(pthread_t tid , void **status)
{
        int n;

        if( ( n = pthread_join(tid, status)) == 0)
            return;
        errno = n;
        err_sys("pthread_join error");
}

void Pthread_detach(pthread_t  tid)
{
        int n;

        if( ( n = pthread_detach(tid)) == 0)
            return ;
        errno = n;
        err_sys("pthread_detach error");
}

void Pthread_kill(pthread_t tid, int signo)
{
        int n;

        if( ( n = pthread_kill(tid, signo)) == 0)
            return;
        errno = n;
        err_sys("pthread_kill error");
}
