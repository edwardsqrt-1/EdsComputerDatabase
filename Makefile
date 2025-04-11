CC=gcc
FLAGS=$(shell pkg-config --cflags --libs gtk+-3.0) -Iinclude

default:
	mkdir -p bin
	$(CC) -o bin/compdb src/*.c $(FLAGS) 
	cp res/mainwin.glade bin/

debug:
	mkdir -p bin
	$(CC) -o bin/compdb src/*.c $(FLAGS) -g
	cp res/mainwin.glade bin/
	
install:
	mkdir /var/lib/CompDB 
	cp bin/*.* /var/lib/CompDB
	touch /var/lib/CompDB/compdb.db
