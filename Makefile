packetMonitor: main.o error.o host_serv.o sock_ntop_host.o wrapthread.o startDemon.o signal.o packetEngine.o
	gcc main.o host_serv.o error.o sock_ntop_host.o wrapthread.o startDemon.o signal.o packetEngine.o -o startDemon -Wall -ggdb -pthread

main.o: main.c
	gcc -ggdb -c main.c

error.o: error.c
	gcc -c error.c

host_serv.o: host_serv.c
	gcc -c host_serv.c

sock_ntop_host.o: sock_ntop_host.c
	gcc -c sock_ntop_host.c

wrapthread.o: wrapthread.c
	gcc -c wrapthread.c

startDemon.o: startDemon.c
	gcc -ggdb -c startDemon.c

signal.o: signal.c
	gcc -c signal.c
packetEngine.o: packetEngine.c
	gcc -ggdb -c packetEngine.c

clean:
	rm *.o core *~ startDemon
