WGEN = world_gen
MISC = misc_classes
VPATH= $(WGEN):$(MISC)
OBJS = DungeonBuilder.o Room.o int_point.o main.o
CC = g++
IFLAGS = -I. -I$(WGEN) -I$(MISC)
CFLAGS = -Wall $(IFLAGS) -c 
LFLAGS = -Wall $(IFLAGS)

all : roguelike
	cd 

roguelike : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o roguelike

main.o : main.cpp
	$(CC) $(CFLAGS) main.cpp

int_point.o : int_point.h int_point.cpp
	$(CC) $(CFLAGS) $(MISC)/int_point.cpp

DungeonBuilder.o : DungeonBuilder.h DungeonBuilder.cpp int_point.h Room.h
	$(CC) $(CFLAGS) $(WGEN)/DungeonBuilder.cpp

Room.o : Room.h Room.cpp int_point.h
	$(CC) $(CFLAGS) $(WGEN)/Room.cpp

clean :
	\rm *.o roguelike
