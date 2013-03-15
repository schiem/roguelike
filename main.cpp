#include <DungeonBuilder.h>
#include <Character.h>
#include <ncurses.h>
#include <stdio.h>

int main()
{
	//initialize curses (see http://www.tldp.org/HOWTO/NCURSES-Programming-HOWTO/ for more)
    /*
	initscr();
	noecho();
	keypad(stdscr, TRUE);
	raw();
	halfdelay(5);
    */
	
	DungeonBuilder dungeon(50, 40);
    dungeon.build_pblind_dungeon(1, 1, 1);

    /*
	//check to see if the program is running
	bool running = true;

	//the main loop of the program
	while (running == true){
		//everything takes action

		printw("Things\n");
		
		int c = getch();
		
		if (c == 'e'){
			running = false;
		}
		else{
			printw("%c\n", c);
		}

		refresh();
	}
	//clean up ncurses
	endwin();
    */

	return 0;	
}
