AM_CPPFLAGS = -I.\
			  -Isrc/world/dungeon_gen\
			  -Isrc/misc_classes\
			  -Isrc/character_classes\
			  -Isrc/world\
			  -Ilib/SDL-ASCII-Template\
			  -Isrc/gui

AM_LDFLAGS =  -lSDLmain\
			  -lSDL

bin_PROGRAMS = roguelike
roguelikedir = .

roguelike_SOURCES = \
	src/main.cpp\
	src/misc_classes/int_point.cpp\
	src/misc_classes/bresenham.cpp\
	src/character_classes/main_character.cpp\
	src/character_classes/character.cpp\
	src/gui/gui_loop.cpp\
	src/gui/gui.cpp\
	src/gui/virtual_event.cpp\
	src/gui/gui_event.cpp\
	src/gui/gui_init.cpp\
	src/gui/canvas.cpp\
	src/gui/gui_cleanup.cpp\
	src/gui/gui_render.cpp\
	src/world/overworld.cpp\
	src/world/chunk.cpp\
	src/world/terrain_defs.cpp\
	src/world/dungeon_gen/dungeon.cpp\
	src/world/dungeon_gen/procedurally_blind_db.cpp\
	src/world/dungeon_gen/room.cpp\
	src/world/dungeon_gen/corruptible_pblind_db.cpp\
	src/world/dungeon_gen/dungeonbuilder.cpp\
	lib/SDL-ASCII-Template/ASCII_Lib.cpp

roguelike_HEADERS = \
	src/misc_classes/bresenham.h\
	src/misc_classes/int_point.h\
	src/character_classes/character.h\
	src/gui/virtual_event.h\
	src/gui/gui.h\
	src/gui/canvas.h\
	src/world/terrain_defs.h\
	src/world/chunk.h\
	src/world/dungeon_gen/dungeonbuilder.h\
	src/world/dungeon_gen/room.h\
	src/world/dungeon_gen/dungeon.h\
	src/world/dungeon_gen/procedurally_blind_db.h\
	src/world/dungeon_gen/corruptible_pblind_db.h\
	src/world/overworld.h\
	lib/SDL-ASCII-Template/def.h\
	lib/SDL-ASCII-Template/ASCII_Lib.h
