#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int get_rand(int, int);

void print_drop(WINDOW *win, int i, int j, int boldness, char *text);

static int get_boldness(int, int);

struct Droplet  {
	int len;
	int head;
	int speed;
};

int main(int arc, char ** argv) {
	srand(time(NULL));
	initscr();
	noecho();
	start_color();
	curs_set(FALSE);
	WINDOW *win = newwin(0,0,0,0);
	nodelay(win, TRUE);
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	init_pair(2, COLOR_WHITE, COLOR_BLACK);
	int x_max, y_max;
	getmaxyx(win, y_max, x_max);
	struct Droplet droplets[x_max]; 
	for(int d = 0; d < x_max; d++) {
		struct Droplet droplet = {
			.len = get_rand(3, y_max), 
			.head = get_rand(-y_max<<2, 0), 
			.speed = get_rand(1, 5)
		};
		droplets[d] = droplet;
	}
	char *drop_type[2] = {"0", "1"};
	for(;;) {
		for (int x = 0; x < x_max; x++) {	
			struct Droplet drp = droplets[x];
			if (drp.head - drp.len == y_max) {
				drp.head = get_rand(-y_max<<2, 0);
				drp.len = get_rand(3, y_max);
				drp.speed = get_rand(1, 5);
			}
			for (int l = 0; l < drp.len; l++) {
				int boldness = get_boldness(l, drp.len);
				int row = (drp.head * drp.speed) - l;
				print_drop(win, row, x, boldness, drop_type[get_rand(0, 1)]);						
			}
			drp.head += 1;
			droplets[x] = drp;
		}
		wrefresh(win);
		werase(win);
		napms(80);
	}
	return 0;
}

static int get_boldness(int index, int len) {
	return !index ? 2 : index < 3 ? 1 : 0;
}

int get_rand(int min, int max) {
	return rand() % (max - min + 1) + min;
}

void print_drop(WINDOW *win, int i, int j, int boldness, char *text) {
	if (boldness == 2) {
		wattrset(win, COLOR_PAIR(2) | A_BOLD);
	} else if (boldness == 1) {
		wattrset(win, COLOR_PAIR(1) | A_BOLD);
	} else {
		wattrset(win, COLOR_PAIR(1));
	}
	if (i >= 0) {
		mvwprintw(win, i, j, text);	
	}
	return;		
}

