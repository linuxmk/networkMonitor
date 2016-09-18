#ifndef __WRAPTHREAD_H
#define __WRAPTHREAD_H

#include "stdclib.h"
#include <pthread.h>
#include <signal.h>


void createTcpServer(int argc, char *argv[]);

void Pthread_create(pthread_t *, const pthread_attr_t *, void *(*)(void *), void *);

void Pthread_join(pthread_t , void **);
void Pthread_detach(pthread_t );
void Pthread_kill(pthread_t, int);

#endif // UNPTHREAD_H

