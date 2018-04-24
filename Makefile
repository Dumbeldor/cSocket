CC=gcc

all: build

build:
	${CC} -o server src/server.c -lpthread
	${CC} -o client src/client.c -lpthread

clean:
	rm client server
