OBJS = DungeonBuilder.o Room.o int_point.o main.o
CC = g++
CFLAGS = -Wall -c
LFLAGS = -Wall

all : roguelike

roguelike : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o roguelike

main.o : main.cpp
	$(CC) $(CFLAGS) main.cpp

int_point.o : int_point.h int_point.cpp
	$(CC) $(CFLAGS) int_point.cpp

DungeonBuilder.o : DungeonBuilder.h DungeonBuilder.cpp int_point.h Room.h
	$(CC) $(CFLAGS) DungeonBuilder.cpp

Room.o : Room.h Room.cpp int_point.h
	$(CC) $(CFLAGS) Room.cpp

clean:
	\rm *.o roguelike
