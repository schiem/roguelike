/**
 *  HELPER.H
 *
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


#ifndef HELPER_H
#define HELPER_H
#include <int_point.h>
#include <vector>
#include <boost/filesystem.hpp>
#include <string.h>
#include <constants.h>

using namespace boost::filesystem;


IntPoint get_abs(IntPoint, IntPoint);
std::vector<std::string> all_files(const string &);
std::vector<std::string> all_files_of_type(const string &, const string & extension);
std::string parse_settings(string &);
int get_max_width(std::vector<std::string>);
#endif
