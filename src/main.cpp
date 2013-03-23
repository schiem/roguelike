#include <DungeonBuilder.h>
#include <terrain_defs.h>
#include <Character.h>
#include <ncurses.h>
#include <stdio.h>


int main()
{
	//initialize curses (see http://www.tldp.org/HOWTO/NCURSES-Programming-HOWTO/ for more)
	initscr();
	noecho();
	keypad(stdscr, TRUE);
	raw();
	halfdelay(5);
    curs_set(0); 

	DungeonBuilder dungeon(50, 40);
    dungeon.build_pblind_dungeon(1, 1, 1);
   
    Main_Character myCharacter(100, 10, 10, 'c');
    
    //check to see if the program is running
	bool running = true;
    bool added = false;
	//the main loop of the program
	while (running == true){
		//everything takes action
		int c = getch();
		dungeon.print();		
		myCharacter.display_character();
		if (c == 'e'){
			running = false;
		}
		else{
            myCharacter.perform_action(c);
		}
		refresh();
	}
	//clean up ncurses
	endwin();

	return 0;	
}
