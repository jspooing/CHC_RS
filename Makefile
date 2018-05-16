CC = gcc
CFLAGS = -pthread
TARGET = server.exe 

server : server.o netstat.o
	$(CC) -o $(TARGET) $^ $(CFLAGS)
	rm $^
debug : server.c netstat.c
	$(CC) -D_DEBUG -o $(TARGET) $^ $(CFLAGS)
	
server.o : server.c
	$(CC) -c $^

netstat.o : netstat.c
	$(CC) -c $^

clean : 
	rm $(TARGET)  
