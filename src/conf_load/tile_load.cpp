/**
 *  @file CONF_LOAD.CPP
 *  @author Michael Yoder, Seth Yoder
 *
 *
 *  @section LICENSE
 *  This file is part of ROGUELIKETHING.
 *
 *  ROGUELIKETHING is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  ROGUELIKETHING is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with ROGUELIKETHING.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <fstream>
#include <iostream>
#include <boost/filesystem.hpp>

#include <utility.h>
#include <tile_load.h>
#include <string.h>
#include <ini.h>

#include <unordered_map>

namespace fs=boost::filesystem;

namespace tile_load {

    bool conf_exists() {
        if (utility::file_exists(TILECONF)) {
            cout<<"Found conf file!"<<std::endl;
            return true;
        } else {
            cout<<"Could not find conf file. It should be located at "<<TILECONF<<" ..."<<std::endl;
            return false;
        }
    }

    static int handle_ini_entry(void* tiledefs, const char* section, const char* name, const char* value) {
        #define MATCH(s, n) strcmp(s, n)==0

        std::unordered_map<string, tile>* tilemap = 
                (std::unordered_map<string, tile>*) tiledefs;

        (*tilemap)[section].tilename=section;
        if(MATCH(name,"char_count")) {
            (*tilemap)[section].char_count=stoi(value);
        } else if (MATCH(name, "tile_id")) {
            (*tilemap)[section].tile_id=stoi(value);
        } else if (MATCH(name, "color")) {
            (*tilemap)[section].color=value;
        } else if (MATCH(name, "corporeal")) {
            (*tilemap)[section].corporeal=stoi(value);
        } else if (MATCH(name, "visible")) {
            (*tilemap)[section].visible=stoi(value);
        } else if (MATCH(name, "opaque")) {
            (*tilemap)[section].opaque=stoi(value);
        } else if (MATCH(name, "seen")) {
            (*tilemap)[section].seen=stoi(value);
        } else if (MATCH(name, "can_build_overtop")) {
            (*tilemap)[section].can_build_overtop=stoi(value);
        } else {
            cout<<"Could not load configuration: name="<<name<<"; value="<<value<<"."<<endl;
        }

        return 0;
    }

    void print_tile(tile& theTile) {
        cout<<"Tile name: "<<theTile.tilename<<endl;
        cout<<"Font character (char_count): "<<theTile.char_count<<endl;
        cout<<"Tile ID: "<<theTile.tile_id<<endl;
        cout<<"Color: "<<theTile.color<<endl;
        cout<<"Corporeal: "<<theTile.corporeal<<endl;
        cout<<"Visible: "<<theTile.visible<<endl;
        cout<<"Opaque: "<<theTile.opaque<<endl;
        cout<<"Seen: "<<theTile.seen<<endl;
        cout<<"Can build overtop: "<<theTile.can_build_overtop<<endl;
    }

    void print_conf() {
        fs::path tileconf(TILECONF);
        /*
        std::ifstream input(tileconf.string().c_str());
        std::string current;
        while(!input.eof()) {
            std::getline(input, current);
            std::cout<<current<<std::endl;
        }
        */

        std::unordered_map<string, tile> tilemap;

        if (ini_parse(tileconf.string().c_str(), handle_ini_entry, &tilemap) < 0) {
            printf("Can't load test.ini\n");
            exit(EXIT_FAILURE);
        }
        
        print_tile(tilemap["KOBOLD_CORPSE"]);
    }

    void load_conf() {
        if (conf_exists()) {
            print_conf();
        } else {
            exit(EXIT_FAILURE);
        }
    }

}
