/**
 *  HELPER.CPP
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


#include <helper.h>

/*
 * PRE: Takes in an IntPoint representing chunk coordinates, and an InPoint
 * representing coordinates within that chunk.
 * POST: Returns and IntPoint containing the absolute coordinates, i.e. the
 * coordinates there were no chunks.
 */

IntPoint get_abs(IntPoint chunk, IntPoint coords) {
    return IntPoint(chunk.row * STARTING_HEIGHT + coords.row, 
            chunk.col * STARTING_WIDTH + coords.col);
}

/* Returns all the files in a directory
*/
std::vector<std::string> all_files(const string & dir_string)
{
    path dir_path(dir_string);
    std::vector<std::string> files;
    if(exists(dir_path))
    {
        directory_iterator end_path;
        for(directory_iterator iter(dir_path); iter!=end_path; iter++)
        {
            files.push_back(iter->path().leaf().string());
        }
    return files;
    }
}

/* Returns all the files in a directory of a given type
*/
std::vector<std::string> all_files_of_type(const string & dir_string, const string & extension)
{
    path dir_path(dir_string);
    std::vector<std::string> files;
    if(exists(dir_path))
    {
        directory_iterator end_path;
        for(directory_iterator iter(dir_path); iter!=end_path; iter++)
        {
            if(iter->path().extension() == extension)
            {
                files.push_back(iter->path().leaf().stem().string());
            }
        }
    return files;
    }
}
