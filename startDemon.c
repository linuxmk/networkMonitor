#include "startDemon.h"
#include "wrapthread.h"
#include "linuxsyscall.h"
#include "signal.h"
#include "packetEngine.h"

int stopThreads = 0;
int stopLog = 0;
int stopPktCpt = 0;

void ih(int s)
{
	fprintf(stderr, "Signal catched\n");
	stopThreads = 1;
}


void startDemon(int argc, char *argv[])
{

	pthread_t pkt,log;

	Pthread_create(&pkt, NULL, packetCapture, NULL);
	Pthread_create(&log, NULL, logging, NULL);

	Signal(SIGINT, ih);
	
	while(1)
	{
		if(stopPktCpt && stopLog)
		{
			fprintf(stderr, "exiting startDemon()\n");
			break;
			
		}
		sleep(1);
	}
}

void * packetCapture(void *arg)
{
	Pthread_detach(pthread_self());

	initSnifferRaw();
	while(1 /*!stopPktCpt*/)
	{
		if(stopThreads)
		{
			stopPktCpt = 1;
			fprintf(stderr, "Exiting packet capture thread\n");
			break;
		}
		sleep(1);
	}
	
return NULL;
}


void * logging(void *arg)
{
	Pthread_detach(pthread_self());

	while(1 /*!stopLog*/)
	{
		if(stopThreads)
		{
			stopLog = 1;
			fprintf(stderr, "Exiting logging thread\n");
			break;
		}
		sleep(1);
	}
	
return NULL;
}
