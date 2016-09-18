#include "stdclib.h"
#include "socket.h"
#include "error.h"

char * sock_ntop_host(const struct sockaddr *sa, socklen_t salen, char *h)
{

	switch(sa->sa_family)
	{
		case AF_INET:
		{
			struct sockaddr_in *sin = (struct sockaddr_in*) sa;
			
			if(inet_ntop(AF_INET, &sin->sin_addr, h, 128) == NULL)
				return NULL;
			return h;
		}
		case AF_INET6:	
		{
			struct sockaddr_in6 *sin6 = (struct sockaddr_in6*) sa;

			if(inet_ntop(AF_INET6, &sin6->sin6_addr, h, 128) == NULL)
                                return NULL;
                        return h;
		}
	}	
}


char * Sock_ntop_host(const struct sockaddr *sa, socklen_t salen, char *h)
{
	char *ptr;

	if( ( ptr = sock_ntop_host(sa, salen, h)) == NULL)
		err_sys("sock_ntop_host error");
	return ptr;
}
