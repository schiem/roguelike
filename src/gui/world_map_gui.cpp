#include <world_map_gui.h>

typedef std::vector<std::vector<MapTile> > MapTileMatrix;
WorldMapGUI::WorldMapGUI(int h, int w) {
    height = h;
    width = w;
    world_map = WorldMap(h, w);
    top_layer = std::vector<TopLayerData>();
    canvas = std::vector<std::vector<MapTile> >(height, 
            std::vector<MapTile>(width));

    TopLayerData cursor = {5, 5, map_tile::CURSOR};
    top_layer.push_back(cursor);
    refresh();
}

void WorldMapGUI::refresh() {
    MapTileMatrix raw_map = world_map.get_map();
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            canvas[i][j] = raw_map[i][j];
        }
    }
    add_layers();
}

void WorldMapGUI::add_layers() {
    for(int i = 0; i < top_layer.size(); i++) {
        canvas[top_layer[i].row][top_layer[i].col] = top_layer[i].tile;
    }
}

void WorldMapGUI::move_cursor(int col_change, int row_change) {
    int new_row, new_col;
    TopLayerData* cursor = &top_layer[0];

    new_row = cursor->row + row_change;
    new_col = cursor->col + col_change;

    if(new_row >= 1 && new_row < (height - 1) &&
       new_col >= 1 && new_col < (width - 1)) {
        cursor->row = new_row;
        cursor->col = new_col;
    }
    refresh();
}

const MapTileMatrix& WorldMapGUI::get_map() {
    return world_map.get_map();
}

const MapTileMatrix& WorldMapGUI::get_canvas() {
    return canvas;
}

IntPoint WorldMapGUI::get_selected_chunk() {
    return IntPoint(top_layer[0].row, top_layer[0].col);
}
