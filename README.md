Roguelike
=========
<img src=http://imgur.com/iEr9v8W.png>
There is a currently working title...
(Someday this will have a real name)

<img src=http://i.imgur.com/SPzG4GZ.png>
Pick where you spawn in the generated world.


<img src=http://i.imgur.com/6tP7JN8.png>
You can now explore an overworld and fight a limited
number of monsters.

<img src=http://i.imgur.com/gMueBa0.png>
Dungeons can be explored as well.

<img src=http://i.imgur.com/GGVw45n.png>
There's now a menu system!

<img src=http://i.imgur.com/bWpsWoL.png>
With items and item descriptions!


<img src=http://i.imgur.com/6p2pMR3.png>
And a death screen!

What is it?
-----------
We're currently trying to build a roguelike game to improve our C++ skills.
Eventually it may be in a state to play, at the moment we really only have
the very basic systems in place.

Goals and Aspirations
---------------------
* Finite, random world generation
* Interactive and semi-persistent population
* Many different types of dungeons and enemies
* Some sort of combat system or something
* Sea travel
* Large cities

Dependencies
------------
* Linux (for now)
* SDL

Installation
------------

To try it for yourself, clone the repository and generate your own configure script:

    git clone https://github.com/schiem/roguelike.git
    cd roguelike
    autoreconf --install
    ./configure
    make

Documentation
-------------
Now with documentation!  Check it out over at <a href="https://schiem.github.io/roguelike">github pages</a>.
Many thanks to doxygen.


Update
-------------
It's been quite a while since we've contributed to this project.  Let's get the ball rolling again!

We want to come up with a more finite list of things that this game sets out to accomplish so
we at least have a direction we can work towards for an MVP.

For the basics of a playable game:

1. Character creation screen
2. Adding a start state (pick where you want to go, start with X/Y equipment)
3. Progression system
4. Way to obtain/buy/sell goods

In addition, we've also discussed and are revamping the purpose--since we need a narrower focus to be able
to continue to work on it.  The new focus is:

This is a homesteading esque game.  You will pick a location to embark, with limited supplies, and will try 
to find a way to build a shelter and survive for as long as you can.  The most basic systems for this will be:
* Protection from the elements; being outdoors in harsh weather conditions without proper equipment will 
be very bad for your character
* Getting a food supply; being able to create your own food will be necessary to begin exploring
* Deal with random encounters; NPCs or enemies may find your dwelling, and you may need to fight them off
* Trade goods in town; towns and cities will have a plethora of shops where you can exchange your goods


Although this scope seems limited, once we have those in place we should have a playable game that we 
can start expanding other systems into.
