AM_CPPFLAGS = -I.\
			  -Isrc/world/dungeon_gen\
			  -Isrc/misc_classes\
			  -Isrc/character_classes\
			  -Isrc/world\
			  -Ilib/SDL-ASCII-Template\
			  -Isrc/gui\
			  -Isrc/items\
              $(BOOST_CPPFLAGS)

AM_LDFLAGS = $(BOOST_LDFLAGS)\
			 $(BOOST_SYSTEM_LDFLAGS)\
			 $(BOOST_FILESYSTEM_LDFLAGS)\
			 $(BOOST_DATE_TIME_LDFLAGS)\
			 $(SDL_LDFLAGS)

localdatadir=data

DEFS = -DDATADIR=\"$(localdatadir)\"

ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS = roguelike
roguelikedir = .

roguelike_SOURCES = \
	src/main.cpp\
	src/misc_classes/int_point.cpp\
	src/misc_classes/bresenham.cpp\
	src/misc_classes/helper.cpp\
    src/character_classes/main_character.cpp\
	src/character_classes/character.cpp\
	src/character_classes/enemy.cpp\
    src/character_classes/enemy_defs.cpp\
    src/character_classes/spawner.cpp\
    src/gui/gui_loop.cpp\
	src/gui/gui.cpp\
	src/gui/virtual_event.cpp\
	src/gui/gui_event.cpp\
	src/gui/gui_init.cpp\
	src/gui/game.cpp\
	src/gui/menu.cpp\
	src/gui/gui_cleanup.cpp\
	src/gui/gui_render.cpp\
	src/gui/world_map_gui.cpp\
	src/world/overworld.cpp\
	src/world/chunk.cpp\
	src/world/terrain_defs.cpp\
	src/world/world_map.cpp\
	src/world/dungeon_gen/dungeon.cpp\
	src/world/dungeon_gen/procedurally_blind_db.cpp\
	src/world/dungeon_gen/room.cpp\
	src/world/dungeon_gen/corruptible_pblind_db.cpp\
	src/world/dungeon_gen/dungeonbuilder.cpp\
	src/items/item.cpp\
    src/items/equipment.cpp\
    src/items/item_defs.cpp\
    lib/SDL-ASCII-Template/ASCII_Lib.cpp\
    \
	src/misc_classes/bresenham.h\
	src/misc_classes/int_point.h\
	src/misc_classes/helper.h\
    src/character_classes/character.h\
    src/character_classes/enemy_defs.h\
    src/character_classes/enemy.h\
    src/character_classes/spawner.h\
    src/gui/virtual_event.h\
	src/gui/gui.h\
	src/gui/game.h\
	src/gui/menu.h\
	src/gui/world_map_gui.h\
	src/world/terrain_defs.h\
	src/world/chunk.h\
	src/world/dungeon_gen/dungeonbuilder.h\
	src/world/dungeon_gen/room.h\
	src/world/dungeon_gen/dungeon.h\
	src/world/dungeon_gen/procedurally_blind_db.h\
	src/world/dungeon_gen/corruptible_pblind_db.h\
	src/world/overworld.h\
	src/world/world_map.h\
    src/items/item.h\
    src/items/item_defs.h\
	lib/SDL-ASCII-Template/def.h\
	lib/SDL-ASCII-Template/ASCII_Lib.h

dist_localdata_DATA = data/ascii.bmp data/chunk/
localdata_DATA = data/ascii.bmp data/chunk/
