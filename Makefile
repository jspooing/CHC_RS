CC = gcc
CFLAGS = -pthread
TARGET = server.exe 

server : server.o netstat.o log.o
	$(CC) -o $(TARGET) $^ $(CFLAGS)
	rm $^
debug : server.c netstat.c log.c
	$(CC) -D_DEBUG -o $(TARGET) $^ $(CFLAGS)
	
server.o : server.c
	$(CC) -c $^

netstat.o : netstat.c
	$(CC) -c $^

log.o : log.c
	$(CC) -c $^

log.out : log.c netstat.c
	$(CC) -D_DEBUG $^ -o $@

clean : 
	rm $(TARGET)  
