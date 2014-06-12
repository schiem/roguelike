Roguelike
=========
<img src=http://i.imgur.com/zBKUMjB.png>
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
