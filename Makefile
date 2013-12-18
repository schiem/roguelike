CC = g++
IFLAGS = -I. -I$(DGEN) -I$(MISC) -I$(CHAR) -I$(WORL) -I$(ASCII) -I$(GUI)
#WFLAGS = -Wno-unused-variable -Wno-sign-compare
CFLAGS = -Wall $(WFLAGS) $(IFLAGS) -g -c 
LDFLAGS = -Wall $(WFLAGS) $(IFLAGS) -g

ASCII = lib/SDL-ASCII-Template
WORL = src/world
DGEN = src/world/dungeon_gen
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

build/dungeonbuilder.o : def.h ASCII_Lib.h ASCII_Lib.cpp dungeonbuilder.h dungeonbuilder.cpp dungeon.h
	$(CC) $(CFLAGS) $(DGEN)/dungeonbuilder.cpp -o $@

build/procedurally_blind_db.o : procedurally_blind_db.h procedurally_blind_db.cpp dungeonbuilder.h
	$(CC) $(CFLAGS) $(DGEN)/procedurally_blind_db.cpp -o $@

build/corruptible_pblind_db.o : corruptible_pblind_db.h corruptible_pblind_db.cpp procedurally_blind_db.cpp
	$(CC) $(CFLAGS) $(DGEN)/corruptible_pblind_db.cpp -o $@

build/room.o : room.h room.cpp int_point.h
	$(CC) $(CFLAGS) $(DGEN)/room.cpp -o $@

build/character.o : dungeon.h ASCII_Lib.h ASCII_Lib.cpp def.h character.h character.cpp 
	$(CC) $(CFLAGS) $(CHAR)/character.cpp -o $@

build/main_character.o : def.h character.h main_character.cpp
	$(CC) $(CFLAGS) $(CHAR)/main_character.cpp -o $@

build/virtual_event.o : virtual_event.cpp virtual_event.h
	$(CC) $(CFLAGS) $(GUI)/virtual_event.cpp -o $@

build/gui_cleanup.o : gui_cleanup.cpp gui.h
	$(CC) $(CFLAGS) $(GUI)/gui_cleanup.cpp -o $@

build/gui_render.o : gui_render.cpp gui.h
	$(CC) $(CFLAGS) $(GUI)/gui_render.cpp -o $@

build/gui_loop.o : gui_loop.cpp gui.h
	$(CC) $(CFLAGS) $(GUI)/gui_loop.cpp -o $@

build/gui_init.o : gui_init.cpp gui.h
	$(CC) $(CFLAGS) $(GUI)/gui_init.cpp -o $@

build/gui_event.o : gui_event.cpp gui.h
	$(CC) $(CFLAGS) $(GUI)/gui_event.cpp -o $@

build/gui.o : gui.cpp gui.h virtual_event.h canvas.h terrain_defs.h
	$(CC) $(CFLAGS) $(GUI)/gui.cpp -o $@

build/canvas.o : canvas.h canvas.cpp character.h main_character.cpp chunk.cpp bresenham.h
	$(CC) $(CFLAGS) $(GUI)/canvas.cpp -o $@

build/dungeon.o :  dungeon.h dungeon.cpp int_point.h room.h terrain_defs.h
	$(CC) $(CFLAGS) $(DGEN)/dungeon.cpp -o $@

build/chunk.o : chunk.h chunk.cpp dungeon.h procedurally_blind_db.h overworld.h terrain_defs.h
	$(CC) $(CFLAGS) $(WORL)/chunk.cpp -o $@

build/overworld.o : overworld.h overworld.cpp terrain_defs.h
	$(CC) $(CFLAGS) $(WORL)/overworld.cpp -o $@

build/bresenham.o : bresenham.h bresenham.cpp
	$(CC) $(CFLAGS) $(MISC)/bresenham.cpp -o $@

clean :
	if [ -x roguelike ]; then rm roguelike; fi; if [ -d build ]; then rm -rf build; fi

what :
	echo $(OBJS)
