#include "WindowsTemplate/window_stuff.h"
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "animation.h"
#include "frame.h"
#include "actor.h"
#include "misc/int_point.h"
#include "misc/bresenham.h"
#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace pt=boost::posix_time;

void init()
{
    //do the window things
    initialize_window_stuff();
}

std::vector<Animation> construct_animations()
{
    //make a list of animation objects
    std::vector<Animation> anims;

    //make the first animation with 50 ms/frame, and 10 frames
    Animation first = Animation(50, 10);
    
    //iterate through the frames.
    for(int i=0;i<10;i++)
    {
        //start point for the circle
        IntPoint start = IntPoint(20, 25);
        
        //make a circle with the same radius as the frame number (frame 3 has a radius of 3)
        std::vector<IntPoint> points = bresenham_circle(start, i);
        for(int j=0;j<points.size();j++)
        {
            //for each point in the circle, make an actor and add it to the frame
            Actor a = Actor(points[j].col, points[j].row, '*', YELLOW);
            first.add_actor_to_frame(a, i);
        }
    }
    
    //add the animation we just made to the list of animations
    anims.push_back(first);
    return anims;
}

bool get_input()
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
        {
            return false;
        }
    }
    return true;
}

void run_things(std::vector<Animation> &animations, long ms)
{
    for(int i=0;i<animations.size();i++)
    {
        animations[i].step(ms);
    }
}

void render(Window& window, std::vector<Animation> &animations)
{
        window.clearWindow();
        //iterate through the animations
        for(int i=0; i<animations.size();i++)
        {
            //get the current frame for the animation
            Frame f = animations[i].get_frame();

            //iterate through the actors in the frame
            for(int j=0;j<f.get_actors().size();j++)
            {
                //draw each actor
                Actor a = f.actor(j);
                window.drawChar(a.get_x(), a.get_y(), a.get_char(), a.get_color());
            }
        }

        window.updateWindow();
}

void cleanup()
{
    cleanup_window_stuff();
}

int main()
{
    init();
    bool running = true;
    std::vector<Animation> animations = construct_animations();
    
    /** handle framerate stuff **/

    //make a timer using the boost timing system
    pt::ptime timer;

    //get the current time
    timer = pt::microsec_clock::local_time();
    long ms = 0;
    
    //make a window using my window wrapper
    Window window = Window(50, 40);
    
    while(running == true)
    {
        //render everything
        render(window, animations);
        
        //check if we should quit
        running = get_input();
        
        //get the time difference between the last frame and this frame
        long ms = (pt::microsec_clock::local_time() - timer).total_milliseconds();
        
        //update the timer
        timer = pt::microsec_clock::local_time();
        
        //tick the animations forward
        run_things(animations, ms);
    }
    cleanup();
    return 0;
}
