
#include <cmath>
#include "Canvas.h"
#include <vector>


using namespace std;


/*
Prototype couleurs

0 -> 80 : pair de couleur 8 couleurs + 1 couleurs par default;
si nb couleurs == 256
81 -> 255 : echelle de couleur modification du background possible et deux couleur min r g b max r g b

Colorscale{
	Color min 30 40 0
	Color max 700 90 30

670

r_l = max_r - min_r; 670
g_l = max_g - min_g; 50
b_l = max_b - min_b; 30

max - min

for(int i = 0 ; i < 175 ; ++i){
	init_color(81 + i, min_r + (i * r_l) / 175, min_g + (i * g_l) / 175, min_b + (i * b_l) / 175);
}

}



je veux : rentrer couleurs (rgb) et recuperer couleur (rgb)

set variation...
			    0   1000			
set_red_colors(min, max) default (0, 0)
set_green_colors(min, max) default (0, 0)
set_blue_colors(min, max) default (0, 0)

set_rgb_colors(r_min, r_max, g_min, g_max, b_min, b_max, backgrounds){
	
}


*/
/* Affiche toutes les Pairs de couleurs disponibles dans le terminal */
void print_colors(int mod){

	for (int i = 0; i < COLORS; ++i)
	{
		if(i % mod == 4 && i != 0)
			printw("\n");

		attron(COLOR_PAIR(i));
		printw("@");
		attroff(COLOR_PAIR(i));
	}

/*
	8 premier si (r == 0 || r == 128) && (b == 0 || b == 128) && (b == 0 || b == 128)
	&& (r == b == g == 192)

	16 premier si (r == b == g == 128) && (r == 0 || r == 255) && (b == 0 || b == 255) && (b == 0 || b == 255)

	r % 128 + 2 * (g % 128) + 4 * (b % 128) 
	0 --> 15 classic color 		
	16 --> 231 rgb color
	232 --> 255 grey color
	
	int RGB_MAX_INTENSITY = 0;
	int GREY_MAX_INTENSITY = 0;

	if(COLORS == 88){
		RGB_MAX_INTENSITY = 4;
		GREY_MAX_INTENSITY = 8;
	}
	else if(COLORS == 256){
		RGB_MAX_INTENSITY = 6;
		GREY_MAX_INTENSITY = 24;
	}

	grey = 8 + 10 * i
	
	(grey - 8) / 10 = i
*/
}

void print_scale(int n, int mod){
	//ROUGE
	for(int i = 0 ; i < n ; i++){
		if(i % mod == 0)
			mvprintw(0, i, "%d", i);
	}
	printw("\n");
}

void get_colors(vector<short*>& color_backup){

	short r;
	short g;
	short b;

	color_backup.clear();

	for (int i = 0; i < COLORS; ++i)
	{
		color_content(i, &r, &g, &b);
		short* col = new short[3];
		col[0] = r; col[1] = g; col[2] = b;
		color_backup.push_back(col);
	}
}

void reset_colors(const vector<short*>& color_backup){
	for (int i = 0; i < COLORS; ++i)
	{
		init_color(i, color_backup[i][0], color_backup[i][1], color_backup[i][2]);
	}
}

int main(int argc, char** argv)
{
	//int mod = 6;

	int n = 256;
	//int min = 255;
	//int max = 0;
	
	srand(time(NULL));

	if(argc > 1)
		n = atoi(argv[1]);
	//if(argc > 2)
	//	min = atoi(argv[2]);
	//if(argc > 3)
	//	max = atoi(argv[3]);

	Term::init_curs();
	//init_color_scale(Color(rand() % 256, rand() % 256, rand() % 256), Color(rand() % 256, rand() % 256, rand() % 256));
	init_color_scale(Color(255, 255, 0), Color(255, 0, 0));

	print_scale(COLORS, 5);
	
	for(int i = 0 ; i < 256 ; i++){
		Term::scr.set_on(ColorScale(i));
		printw("@");
		Term::scr.set_off(ColorScale(i));
	}
	printw("\n");

	Term::scr.set_color_pair(ColorPair::Default);

	short f, b;
	pair_content(n, &f, &b);
	
	printw("front (%d) rgb : %s\n", f, to_color_rgb(f).to_string().c_str());
	printw("back (%d) rgb : %s\n", b, to_color_rgb(b).to_string().c_str());

	printw("color_pairs : %d\n", COLOR_PAIRS);
	printw("color_number : %d\n", COLORS);

	getch();

	Term::end_curs();
	
	return 0;
}
