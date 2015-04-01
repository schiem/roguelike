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

IntPoint get_abs(IntPoint chunk, IntPoint coords) {
    return IntPoint(chunk.row * CHUNK_HEIGHT + coords.row,
            chunk.col * CHUNK_WIDTH + coords.col);
}

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

std::string parse_settings(string &setting)
{
    unsigned int position = setting.find("=");
    if(position == setting.npos)
    {
        return "";
    }
    else
    {
        string value = setting.substr(position+1);
        setting = setting.substr(0, position);
        return value;
    }
}

int get_max_width(vector<string> string_list)
{
    int length = 0;
    for(int i=0; i<string_list.size(); i++)
    {
        if(string_list[i].size() > length)
        {
            length = string_list[i].size();
        }
    }
    return length;
}

std::vector<std::string> split_string(std::string input, char character)
{
    cout<<input<<" "<<character<<endl;
    std::vector<std::string> strings;
    int last_position = 0;
    for(int i = 0; i < input.length();i++) {
        if(input[i] == character) {
            strings.push_back(input.substr(last_position, i - last_position));
            last_position = i+1;
        }
    }
    strings.push_back(input.substr(last_position, input.length()));

    /*
    cout<<"Strings: ";
    for(int i = 0; i < strings.size(); i++) {
        cout<<strings[i]<<endl;
    }
    cout<<endl;
    */

    return strings;
}

std::vector<int> strings_to_ints(std::vector<std::string> strings)
{
    std::vector<int> int_vec;
    stringstream ss;
    int temp;
    for(int i = 0; i < strings.size();i++)
    {
        ss << strings[i];
        ss >> temp;
        int_vec.push_back(temp);
        ss.str(std::string());
        ss.clear();
    }
    return int_vec;
}

bool in_range(IntPoint chunk, IntPoint coords, IntPoint range_chunk, IntPoint center, IntPoint radius) {
    IntPoint abs = get_abs(chunk, coords);
    IntPoint tl_abs = get_abs(range_chunk,
            IntPoint(center.row - radius.row,
                center.col - radius.col));

    IntPoint br_abs = get_abs(range_chunk,
            IntPoint(center.row + radius.row,
                center.col + radius.col));

    bool is_x = (abs.col>=tl_abs.col && abs.col<br_abs.col);
    bool is_y = (abs.row>=tl_abs.row && abs.row<br_abs.row);
    return (is_x && is_y);
}

IntPoint normalize_coords(IntPoint coords)
{
    int row = coords.row + (CHUNK_HEIGHT * (coords.row < 0)) - (CHUNK_HEIGHT * (coords.row >= CHUNK_HEIGHT));
    int col = coords.col + (CHUNK_WIDTH * (coords.col < 0)) - (CHUNK_WIDTH * (coords.col >= CHUNK_WIDTH));
    std::cout<<"Original coords: "<<coords<<", and new coords"<<IntPoint(row, col)<<std::endl;
    return IntPoint(row, col);
}

IntPoint normalize_chunk(IntPoint coords)
{
    int c_row = 0 + (coords.row >= CHUNK_HEIGHT) - (coords.row < 0);
    int c_col = 0 + (coords.col >= CHUNK_WIDTH) - (coords.col < 0);
    std::cout<<"New chunk: "<<IntPoint(c_row, c_col)<<std::endl; 
    return IntPoint(c_row, c_col);
}

std::vector<IntPoint> sort_ip_by_row(std::vector<IntPoint> points)
{
    int lowest;
    for(int i=0;i<points.size();i++)
    {
        lowest = points[i].row;
        for(int j=i;j<points.size();j++)
        {
            if(points[j].row < lowest)
            {
                IntPoint temp = points[i];
                points[i] = points[j];
                points[j] = temp;
            }
        }
    }
    return points;
}

std::vector<IntPoint> sort_ip_by_col(std::vector<IntPoint> points)
{
    int lowest;
    for(int i=0;i<points.size();i++)
    {
        lowest = points[i].col;
        for(int j=i;j<points.size();j++)
        {
            if(points[j].col < lowest)
            {
                IntPoint temp = points[i];
                points[i] = points[j];
                points[j] = temp;
            }
        }
    }
    return points;
}

std::vector<std::vector<IntPoint> > sort_intpoints(std::vector<IntPoint> points)
{
    points = sort_ip_by_row(points);
    std::vector<std::vector<IntPoint> > sorted_points;
    
    //split it into lists based on y-values
    //assumes list is not empty
    int y_val = points[0].row;
    std::vector<IntPoint> temp;
    for(int i=0;i<points.size();i++)
    {
        if(points[i].row != y_val)
        {
            sorted_points.push_back(temp);
            temp = std::vector<IntPoint>();
            y_val = points[i].row;
        }
        else
        {
            temp.push_back(points[i]);
        }
    }
    
    //now sort them all by x
    for(int i=0;i<sorted_points.size();i++)
    {
        sorted_points[i] = sort_ip_by_col(sorted_points[i]);
    }
    
    //get the largest list in the vector.
    int largest_vector = find_largest(sorted_points);
    int smallest_y = sorted_points[0][0].row;
    
    //find the smallest x coordinate
    int smallest_x = sorted_points[0][0].col;
    for(int i=0;i<sorted_points.size();i++)
    {
        if(sorted_points[i][0].col < smallest_x)
        {
            smallest_x = sorted_points[i][0].col;
        }
    }

    //and finally, actually pad them out
    for(int i=0;i<sorted_points.size();i++)
    {
        //pad the beginning
        for(int j=0;j<(sorted_points[i][0].row - smallest_y);j++)
        {
            sorted_points[i].insert(sorted_points[i].begin(), IntPoint(-1, -1));
        }
        //pad the end
        for(int j=0;j<(largest_vector - sorted_points[i][sorted_points[i].size() - 1].row);j++)
        {
            sorted_points[i].push_back(IntPoint(-1, -1));
        }
    }
    return sorted_points;
}






std::string concatenate_vectors(std::vector<std::string> first, std::vector<std::string> second)
{
    std::string temp = "";
    for(int i=0;i<first.size();i++)
    {
        temp = temp + first[i];
        if(i < second.size())
        {
            temp = temp + second[i];
        }
    }
    return temp;
}

