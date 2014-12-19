AM_CPPFLAGS = -I.\
			  -Isrc/world/dungeon_gen\
			  -Isrc/world/civ\
              -Isrc/misc_classes\
			  -Isrc/character_classes\
			  -Isrc/world\
			  -Ilib/SDL-ASCII-Template\
			  -Isrc/gui\
			  -Isrc/items\
              -Isrc/menu\
              -Isrc/animation\
              -Isrc/plant\
              -Isrc/\
              -pg\
              $(BOOST_CPPFLAGS)

AM_LDFLAGS = $(BOOST_LDFLAGS)\
			 $(BOOST_SYSTEM_LDFLAGS)\
			 $(BOOST_FILESYSTEM_LDFLAGS)\
			 $(BOOST_DATE_TIME_LDFLAGS)\
			 $(SDL_LDFLAGS)\
             -pg

localdatadir=data

DEFS = -DDATADIR=\"$(localdatadir)\"

ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS = roguelike
roguelikedir = .

roguelike_SOURCES = \
	src/defs.cpp\
    src/main.cpp\
	src/misc_classes/int_point.cpp\
	src/misc_classes/bresenham.cpp\
	src/misc_classes/helper.cpp\
    src/misc_classes/debug.cpp\
    src/misc_classes/math_helper.cpp\
    src/misc_classes/binary_space.cpp\
    src/character_classes/main_character.cpp\
	src/character_classes/character.cpp\
	src/character_classes/enemy.cpp\
    src/character_classes/spawner.cpp\
    src/character_classes/den.cpp\
    src/gui/gui_loop.cpp\
	src/gui/gui.cpp\
	src/gui/virtual_event.cpp\
	src/gui/gui_event.cpp\
	src/gui/gui_init.cpp\
	src/gui/game.cpp\
    src/menu/menu.cpp\
	src/menu/start_menu.cpp\
    src/menu/main_menu.cpp\
    src/menu/equipment_menu.cpp\
    src/menu/inventory_menu.cpp\
    src/menu/item_menu.cpp\
    src/menu/equip_item.cpp\
    src/menu/font_menu.cpp\
    src/menu/escape_menu.cpp\
    src/menu/audio_menu.cpp\
    src/menu/info_menu.cpp\
    src/gui/gui_cleanup.cpp\
	src/gui/gui_render.cpp\
	src/gui/world_map_gui.cpp\
	src/world/chunk.cpp\
	src/world/chunk_layer.cpp\
	src/world/chunk_matrix.cpp\
	src/world/overworld_gen.cpp\
	src/world/world_map.cpp\
	src/world/dungeon_gen/procedurally_blind_db.cpp\
	src/world/dungeon_gen/room.cpp\
	src/world/dungeon_gen/corruptible_pblind_db.cpp\
	src/world/dungeon_gen/dungeonbuilder.cpp\
    src/world/civ/building.cpp\
	src/world/civ/settlement.cpp\
    src/world/civ/block.cpp\
	src/items/item.cpp\
    src/items/equipment.cpp\
    src/items/weapon.cpp\
    src/items/consumables.cpp\
    src/items/misc.cpp\
    src/animation/animation.cpp\
    src/animation/frame.cpp\
    src/animation/actor.cpp\
    src/animation/animation_defs.cpp\
    src/plant/plant.cpp\
    lib/SDL-ASCII-Template/ASCII_Lib.cpp\
    \
	src/defs.h\
    src/misc_classes/bresenham.h\
	src/misc_classes/int_point.h\
	src/misc_classes/helper.h\
    src/misc_classes/debug.h\
    src/misc_classes/math_helper.h\
    src/misc_classes/binary_space.h\
    src/character_classes/character.h\
    src/character_classes/enemy.h\
    src/character_classes/spawner.h\
    src/character_classes/den.h\
    src/gui/virtual_event.h\
	src/gui/gui.h\
	src/gui/game.h\
	src/gui/game_states.h\
    src/menu/menu.h\
	src/gui/world_map_gui.h\
	src/world/chunk.h\
	src/world/chunk_layer.h\
	src/world/chunk_matrix.h\
	src/world/dungeon_gen/dungeonbuilder.h\
	src/world/dungeon_gen/room.h\
	src/world/dungeon_gen/procedurally_blind_db.h\
	src/world/dungeon_gen/corruptible_pblind_db.h\
    src/world/civ/building.h\
	src/world/civ/settlement.h\
    src/world/civ/block.h\
    src/world/overworld_gen.h\
	src/world/world_map.h\
    src/items/item.h\
    src/animation/animation.h\
    src/animation/frame.h\
    src/animation/actor.h\
	src/animation/animation_defs.h\
    src/plant/plant.h\
    lib/SDL-ASCII-Template/color_def.h\
	lib/SDL-ASCII-Template/ASCII_Lib.h

dist_localdata_DATA = data/ data/chunk/
localdata_DATA = data/ data/chunk/
