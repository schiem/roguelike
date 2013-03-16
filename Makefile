WGEN = World/dungeon_gen
MISC = misc_classes
CHAR = character_classes
VPATH= $(WGEN):$(MISC):$(CHAR)
OBJS = DungeonBuilder.o Room.o int_point.o Character.o Main_Character.o main.o
CC = g++
IFLAGS = -I. -I$(WGEN) -I$(MISC) -I$(CHAR)
CFLAGS = -Wall $(IFLAGS) -c 
LDFLAGS = -Wall $(IFLAGS)
LIBS = -lncurses

all : roguelike
	./roguelike

roguelike : $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o roguelike $(LIBS)

main.o : main.cpp
	$(CC) $(CFLAGS) main.cpp

int_point.o : int_point.h int_point.cpp
	$(CC) $(CFLAGS) $(MISC)/int_point.cpp

DungeonBuilder.o : DungeonBuilder.h DungeonBuilder.cpp int_point.h Room.h
	$(CC) $(CFLAGS) $(WGEN)/DungeonBuilder.cpp

Room.o : Room.h Room.cpp int_point.h
	$(CC) $(CFLAGS) $(WGEN)/Room.cpp

Character.o : Character.h Character.cpp
	$(CC) $(CFLAGS) $(CHAR)/Character.cpp

Main_Character.o : Character.h Main_Character.cpp
	$(CC) $(CFLAGS) $(CHAR)/Main_Character.cpp

clean :
	\rm *.o roguelike
