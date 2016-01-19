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

namespace fs=boost::filesystem;

namespace tile_load {

    bool conf_exists() {
        fs::path tileconf(TILECONF);
        if (fs::exists(tileconf)) {
            std::cout<<"Found conf file!"<<std::endl;
            return true;
        } else {
            std::cout<<"Could not find conf file. It should be located at "<<TILECONF<<" ..."<<std::endl;
            return false;
        }
    }

    void print_conf() {
        fs::path tileconf(TILECONF);
        std::ifstream input(tileconf.string().c_str());
        std::string current;
        while(!input.eof()) {
            std::getline(input, current);
            std::cout<<current<<std::endl;
        }
    }

    void load_conf() {
        if (conf_exists()) {
            print_conf();
        } else {
            exit(EXIT_FAILURE);
        }
    }

}
