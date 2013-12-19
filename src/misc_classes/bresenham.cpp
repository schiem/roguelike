#include "bresenham.h"
/*
 * A Bresenham's line algorithm for plotting lines in discrete space.
 *
 * PRE: Will be given a starting point and an ending point.
 * POST: Will return an IntPoint array representing a line of points between the
 * starting and ending point.
 */
std::vector<IntPoint> bresenham_line(IntPoint& first, IntPoint& second) {
    std::vector<IntPoint> points = std::vector<IntPoint>();
    int x0 = first.col;
    int y0 = first.row;
    int x1 = second.col;
    int y1 = second.row;

    bool steep = (abs(y1 - y0) > abs(x1 - x0));

    if(steep) {
        std::swap(x0,y0);
        std::swap(x1, y1);
    }

    int deltax = abs(x1 - x0);
    int deltay = abs(y1 - y0);
    int error = deltax / 2;
    int y = y0;
    int inc;
    if (x0 < x1) {
        inc = 1;
    } else {
        inc = -1;
    }

    int ystep;
    if (y0 < y1) {
        ystep = 1;
    } else {
        ystep = -1;
    }

    for(int x = x0; x != x1; x += inc) {
        IntPoint current_point;
        if(steep) {
            //NOTE: This is like saying IntPoint(col, row)
            current_point = IntPoint(x, y);
        } else {
            current_point = IntPoint(y, x); 
        }

        points.push_back(current_point);

        error = error - deltay;
        if (error < 0) {
            y = y + ystep;
            error = error + deltax;
        }
    }

    return points;
}

std::vector<IntPoint> bresenham_circle(IntPoint& start, int radius) {
    std::vector<IntPoint> points = std::vector<IntPoint>();
    int x0 = start.row;
    int y0 = start.col;

    int x = radius;
    int y = 0;
    int radius_error = 1 - x;

    while(x >= y) {
        points.push_back(IntPoint(x + x0, y + y0));
        points.push_back(IntPoint(y + x0, x + y0));
        points.push_back(IntPoint(-x + x0, y + y0));
        points.push_back(IntPoint(-y + x0, x + y0));
        points.push_back(IntPoint(-x + x0, -y + y0));
        points.push_back(IntPoint(-y + x0, -x + y0));
        points.push_back(IntPoint(x + x0, -y + y0));
        points.push_back(IntPoint(y + x0, -x + y0));

        y++;
        if(radius_error < 0) {
            radius_error += (2*y + 1);
        } else {
            x--;
            radius_error += (2*(y-x + 1));
        }
    }

    return points;
}
