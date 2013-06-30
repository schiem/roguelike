#include  "canvas.h"
#include <iostream>
using namespace std;
using namespace tiledef;

Canvas::Canvas()
{
    chunk_map = ChunkMatrix(10, vector<Chunk>(10)); 
    buffer = TileMatrix(150, vector<Tile>(300));
    chunk_map[5][8] = Chunk(5, 8, STARTING_WIDTH, STARTING_HEIGHT);
    main_char = Main_Character(100, 50, 25, 3, chunk_map[5][8], -1);
    canvas = TileMatrix(STARTING_HEIGHT, vector<Tile>(STARTING_WIDTH));
    update_buffer();
}


void Canvas::refresh()
{
    if(out_of_bounds())
    {
        update_chunk();
        update_buffer();
    }
    if(main_char.get_depth() >=0)
    {
        for(int i = 0; i < STARTING_HEIGHT; i++) {
            for(int j = 0; j < STARTING_WIDTH; j++) {
                canvas[i][j] = chunk_map[main_char.get_chunk_y()][main_char.get_chunk_x()].get_tile(main_char.get_depth(),i, j);
            }
        }
        canvas[main_char.get_y_loc()][main_char.get_x_loc()] = MAIN_CHAR;
    }
    else
    {
        for(int i = 0; i < STARTING_HEIGHT; i++) //STARTING_HEIGHT is actually screen height, same for STARTING_WIDTH
        {
            for (int j = 0; j < STARTING_WIDTH; j++) {
                canvas[i][j] = buffer[(STARTING_HEIGHT + main_char.get_y_loc()) - (STARTING_HEIGHT/2) + i][(STARTING_WIDTH + main_char.get_x_loc()) - (STARTING_WIDTH/2) + j];
            }
        }
        canvas[STARTING_HEIGHT/2][STARTING_WIDTH/2] = MAIN_CHAR;
        }
    }



bool Canvas::out_of_bounds()
{
    return (main_char.get_x_loc() < 0 || main_char.get_x_loc() >= STARTING_WIDTH || main_char.get_y_loc() < 0 || main_char.get_y_loc() >= STARTING_HEIGHT);
}

void Canvas::update_chunk()
{
    int x = main_char.get_chunk_x();
    int y = main_char.get_chunk_y();

    
    if (main_char.get_x_loc() < 0 )
    {
        x -= 1;
        main_char.set_x(STARTING_WIDTH-1);
    }
    else if (main_char.get_x_loc() >= STARTING_WIDTH)
    {
        x += 1;
        main_char.set_x(0);
    }
    if(main_char.get_y_loc() < 0)
    {
        y -= 1;
        main_char.set_y(STARTING_HEIGHT-1);
    }
    else if (main_char.get_y_loc()>= STARTING_HEIGHT)
    {
        y += 1;
        main_char.set_y(0);
    }
    main_char.update_dungeon(chunk_map[y][x]);
}


/*
This function is an abomination, a sin against code.

SACERDOS ab Ordinario delegatus, rite confessus, aut saltem corde peccata sua detestans, peracto, si commode fieri possit, Sanctissimo Missæ sacrificio, divinoque auxilio piis precibus implorato, superpelliceo et stola violacea indutus, et coram se habens obsessum ligatum, si sit periculum, eum, se et astantes communiat signo crucis, et aspergat aqua benedicta, et genibus flexis, aliis respondentibus, dicat Litanias ordinarias usque ad Preces exclusive.

*/
void Canvas::update_buffer()
{
    int x = 0;
    int y = 0;
    for(int col=main_char.get_chunk_y() - 1;col<=main_char.get_chunk_y()+1;col++) {
        if(chunk_map.size() < (size_t) col + 1) //Make sure this cast works TODO (this avoids -Wsign-compare warnings.
        {
            chunk_map.resize(col + 1);
        }
        for(int row=main_char.get_chunk_x()-1;row<=main_char.get_chunk_x()+1;row++) {
            if (chunk_map[col].size() < (size_t) row + 1) //Make sure this cast works TODO
            {
                Chunk temp_chunk;
                chunk_map[col].push_back(temp_chunk);
            }
            if (chunk_map[col][row].is_initialized() == false)
            {
                std::cout<<"initializing with "<<col<<" & "<<row<<std::endl;
                chunk_map[col][row] = Chunk(col, row, STARTING_WIDTH, STARTING_HEIGHT);
            }
            for (int a=0;a<STARTING_HEIGHT;a++) {
                for (int b=0;b<STARTING_WIDTH;b++) {
                    buffer[a + (x * STARTING_HEIGHT)][b + (y * STARTING_WIDTH)] = chunk_map[col][row].get_tile(-1, a, b);  //this is gross, i'm so sorry.
                }
            }
            y++;
        }
        y = 0;
        x++;
    }

}

const Chunk& Canvas::get_chunk()
{
    return chunk_map[main_char.get_chunk_x()][main_char.get_chunk_y()];
}

//Since this is a const reference, will we have to call it
//more than once? Maybe not...
const std::vector<std::vector<Tile> >& Canvas::get_matrix()
{
    return canvas;
}
