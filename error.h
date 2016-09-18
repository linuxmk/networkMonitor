#ifndef ERROR_H
#define ERROR_H

#include <stdarg.h>
#include <errno.h>

#define errQuit(text) do { \
	char msg[100]; \
	snprintf(msg,100, "%s at \"%s\":%d %s\n", \
		text, __FILE__, __LINE__, strerror(errno)); \
	err_quit(msg); \
	}while(0)

#define errSys(text) do { \
	char msg[100]; \
	snprintf(msg, 100, "%s at \"%s\":%d %s\n", \
		text, __FILE__, __LINE__, strerror(errno)); \
	err_sys(msg); \
	}while(0)

#define errMsg(text) do { \
	char msg[100]; \
	snprintf(msg, 100, "%s at \"%s\":%d %s\n", \
		text, __FILE__, __LINE__, strerror(errno)); \
	err_msg(msg); \
	}while(0)

void err_quit(const char *fmt, ...);
void err_ret(const char *fmt, ...);
void err_sys(const char *fmt, ...);
void err_dump(const char *fmt, ...);
void err_msg(const char *fmt, ...);
void err_quit(const char *fmt, ...);


#endif // ERROR_H

