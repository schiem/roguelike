CC = g++
IFLAGS = -I. -I$(DGEN) -I$(MISC) -I$(CHAR) -I$(WORL) -I$(ASCII) -I$(GUI)
WFLAGS = -Wno-unused-variable -Wno-sign-compare
CFLAGS = -Wall $(WFLAGS) $(IFLAGS) -g -c 
LDFLAGS = -Wall $(WFLAGS) $(IFLAGS) -g

ASCII = lib/SDL-ASCII-Template
WORL = src/World
DGEN = src/World/dungeon_gen
MISC = src/misc_classes
CHAR = src/character_classes
GUI = src/gui
PATHS = $(DGEN) $(MISC) $(CHAR) $(WORL) $(ASCII) $(GUI)
VPATH= $(DGEN):$(MISC):$(CHAR):$(WORL):$(ASCII):$(GUI)
LIBS =  -lSDLmain -lSDL

SRC = $(foreach path,$(PATHS),$(wildcard $(path)/*.cpp)) src/main.cpp
HEADERS = $(foreach path,$(PATHS),$(wildcard $(path)/*.h))
OBJS = $(patsubst %.cpp,build/%.o,$(notdir $(SRC)))


all : roguelike

$(OBJS): | build

build :
	mkdir build

roguelike : $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@ $(LIBS)

build/main.o : src/main.cpp
	$(CC) $(CFLAGS) src/main.cpp -o $@


build/terrain_defs.o : terrain_defs.cpp terrain_defs.h
	$(CC) $(CFLAGS) $(WORL)/terrain_defs.cpp -o $@

build/ASCII_Lib.o : def.h ASCII_Lib.h ASCII_Lib.cpp
	$(CC) $(CFLAGS) $(ASCII)/ASCII_Lib.cpp -o $@

build/int_point.o : int_point.h int_point.cpp
	$(CC) $(CFLAGS) $(MISC)/int_point.cpp -o $@

build/DungeonBuilder.o : terrain_defs.h def.h ASCII_Lib.h ASCII_Lib.cpp DungeonBuilder.h DungeonBuilder.cpp int_point.h Room.h Dungeon.h
	$(CC) $(CFLAGS) $(DGEN)/DungeonBuilder.cpp -o $@

build/Room.o : Room.h Room.cpp int_point.h
	$(CC) $(CFLAGS) $(DGEN)/Room.cpp -o $@

build/Character.o : Dungeon.h ASCII_Lib.h ASCII_Lib.cpp def.h Character.h Character.cpp 
	$(CC) $(CFLAGS) $(CHAR)/Character.cpp -o $@

build/Main_Character.o : def.h ASCII_Lib.h ASCII_Lib.cpp Character.h Main_Character.cpp
	$(CC) $(CFLAGS) $(CHAR)/Main_Character.cpp -o $@

build/Display.o : Display.h Display.cpp def.h ASCII_Lib.h ASCII_Lib.cpp Canvas.h Canvas.cpp terrain_defs.h
	$(CC) $(CFLAGS) $(GUI)/Display.cpp -o $@

build/Canvas.o : Canvas.h Canvas.cpp DungeonBuilder.h DungeonBuilder.cpp terrain_defs.h Dungeon.h Dungeon.cpp Character.h Main_Character.cpp
	$(CC) $(CFLAGS) $(GUI)/Canvas.cpp -o $@

build/Dungeon.o :  Dungeon.h Dungeon.cpp int_point.h Room.h terrain_defs.h
	$(CC) $(CFLAGS) $(DGEN)/Dungeon.cpp -o $@

clean :
	rm -r build roguelike

what :
	echo $(OBJS)
