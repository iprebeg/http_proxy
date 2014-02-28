CC = gcc
CFLAGS = -Wall -ggdb -I ./ #-DDEBUG
LDFLAGS = 
PROJECT = http_proxy
SOURCE = main.c wrap.c http.c
HEADERS = wrap.h http.h
OBJECTS = main.o wrap.o http.o

$(PROJECT) : $(OBJECTS)
	$(CC) $(OBJECTS) -o $(PROJECT) 

$(OBJECTS) : $(SOURCE) $(HEADERS)
	$(CC) -c $(SOURCE) $(CFLAGS)

clean:
	-rm -f $(PROJECT) $(OBJECTS) 

clean_all:
	-rm -f $(PROJECT) $(OBJECTS) *~

debug:
	valgrind --leak-check=full --show-reachable=yes --trace-children=yes  -v ./$(PROJECT) 1024 tibet debian
