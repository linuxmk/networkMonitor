#ifndef __SIGNAL_H
#define __SIGNAL_H

#include "linuxsyscall.h"
#include "error.h"


typedef    void    Sigfunc(int);
Sigfunc * signal (int signo, Sigfunc *func);
Sigfunc * Signal(int signo, Sigfunc *func);


#endif //__SIGNAL_H
