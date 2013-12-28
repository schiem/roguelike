#include <vector>
#include <stdlib.h>
#include <iostream>

#include <world_map.h>

using namespace std;

struct TopLayerData {
    int row;
    int col;
    MapTile tile;
};


class WorldMapGUI {
    typedef std::vector<std::vector<MapTile> > MapTileMatrix;
    public:
        WorldMapGUI(int, int);
        const MapTileMatrix& get_canvas();
        void move_cursor(int, int);

    private:
        std::vector<TopLayerData> top_layer;
        WorldMap world_map;
        MapTileMatrix canvas;
        int width;
        int height;

        void refresh();
        void add_layers();
};
