#include "Particle.h"
 
using namespace std;

#define DISPLAY_STATS \
		mvwprintw(canvas, 0, 0, "Particles : %d, Calculation : %f, Runtime : %f", PS.particles.size(), calc_time, run_time); \
		mvwprintw(canvas, 1, 0, "Frame time : %f, Clear : %f, Display : %f, Update : %f", frame_time, clear_time, display_time, update_time); \
		mvwprintw(canvas, 2, 0, "Input time : %f, Mouse pos : %d %d   ", input_time, Mouse::get_position().x, Mouse::get_position().y); \
		canvas.display(); \



int main(int argc, char const *argv[])
{
	srand(time(NULL));

	//program variables:

	int nbframe = 0;
	float update_time = 0.0;
	float frame_time = 0.0;

	//argv variables:

	//int border = 0;
	int n = 30000;
	float lifetime = 10000000;
	Vector2f gravity(0.0, 0.0);
	Vector2f wind(0.0, 0.0);

	if(argc > 1)
		n = atoi(argv[1]);

	if(argc > 2)
		lifetime = atof(argv[2]);

	if(argc > 3)
		gravity.y = atof(argv[3]);

	if(argc > 4)
		wind.x = atof(argv[4]);

	if(argc > 5)
		wind.y = atof(argv[5]);

	Term::init_curs();
	Term::set_timeout(0);

	init_color_scale(Color::Yellow, Color::Red);
	//Term::scr.set_background_color(Color::Green);
	//refresh();

	Canvas canvas;
	//canvas.set_background_color(Color::Magenta);
	//wclear(canvas);
	//canvas.set_attr(Attr::Normal);
	//canvas.set_color(Color::White);

	//canvas.set_background_color(Color::Yellow);

	IntRect zone(Vector2i::zero, Vector2i(50, 50));

	ParticleSystem PS(lifetime, IntRect(-1, 1, -1, 1), 1);
	PS.add_particles(n, Vector2f(canvas.get_size()) / 2.0f);
	//PS.add_particles(n, zone);
	
	/*
	bool print_left = false;
	bool print_right = false;
	bool print_middle = false;
	bool print_up = false;
	bool print_down = false;
	*/

	while(!Keyboard::is_pressed(Keyboard::Escape)){

		//canvas.set_color(Color(rand() % 256, rand() % 256, rand() % 256));

		nbframe++;
		//PS.add_particles(n, Vector2f(canvas.get_size()) / 2.0f);

		float total_time = clock();

		////////// READ AND USE INPUT

		Term::read_input();
		
		float input_time = (clock() - total_time) / (float)CLOCKS_PER_SEC;

		////////// FORCES CALCULATIONS

		float calc_time = clock();

		Vector2f total_force = gravity + wind;

		if(total_force != Vector2f::zero)
			PS.apply_force(gravity + wind);

		Vector2f origin(map_cell_to_pixel(Mouse::get_position()));
		/*
		if(Mouse::is_released(Mouse::Left)){
			print_left = true;
			print_right = false;
			print_middle = false;
			print_up = false;
			print_down = false;
		}
		else if(Mouse::is_released(Mouse::Right)){
			print_left = false;
			print_right = true;
			print_middle = false;
			print_up = false;
			print_down = false;
		}
		else if(Mouse::is_released(Mouse::Middle)){
			print_left = false;
			print_right = false;
			print_middle = true;
			print_up = false;
			print_down = false;
		}
		else if(Mouse::is_scrolling(Mouse::ScrollUp)){
			print_left = false;
			print_right = false;
			print_middle = false;
			print_up = true;
			print_down = false;
		}
		else if(Mouse::is_scrolling(Mouse::ScrollDown)){
			print_left = false;
			print_right = false;
			print_middle = false;
			print_up = false;
			print_down = true;
		}
		*/
			
		if(Mouse::is_released(Mouse::Left))
			PS.add_particles(n, origin);
	

		if(Keyboard::is_pressed(Keyboard::Z)){
			PS.add_particles(n, origin);
		}

		if(Mouse::is_pressed(Mouse::Left))
			PS.apply_repeller(Repeller(origin));
		else if(Mouse::is_pressed(Mouse::Right))
			PS.apply_attractor(Attractor(origin));
		else if(Mouse::is_pressed(Mouse::Middle))
			PS.add_particles(n, origin);
			//PS.apply_attractor(Attractor(origin));
		
		//else if(Keyboard::is_pressed(Keyboard::Z))
			//PS.add_particles(n, zone);


		calc_time = (clock() - calc_time) / (float)CLOCKS_PER_SEC;

		////////// CLEAR SCREEN

		float clear_time = clock();

		canvas.clear();
		//wclear(canvas);

		clear_time = (clock() - clear_time) / (float)CLOCKS_PER_SEC;

/*
		if(print_left)
			mvwprintw(canvas, 10, 10, "Left released");
		if(print_right)
			mvwprintw(canvas, 10, 10, "Right released");
		if(print_middle)
			mvwprintw(canvas, 10, 10, "Middle released");
		if(print_up)
			mvwprintw(canvas, 10, 10, "Mouse scrollUp");
		if(print_down)
			mvwprintw(canvas, 10, 10, "Mouse scrollDown");
*/
		mvwprintw(canvas, 10, 10, "Mouse bstate %x", Mouse::event.bstate);



		////////// RUN

		float run_time = clock();

		//canvas.set_border((Window::BorderType)border);
		//int intensity = rand() % 256;
		//canvas.set_on(ColorScale(intensity));
		PS.run(canvas);
		//canvas.set_off(ColorScale(intensity));
	
		run_time = (clock() - run_time) / (float)CLOCKS_PER_SEC;
		
		////////// DISPLAY CANVAS (PRINT TO VIRTUAL SCREEN)

		float display_time = clock();

		canvas.display();
		//Term::scr.display();

		display_time = (clock() - display_time) / (float)CLOCKS_PER_SEC;
		
		////////// DISPLAY STATS
 
		DISPLAY_STATS

		////////// UPDATE TERMINAL (PRINT TO REAL SCREEN)

		update_time = clock();

		Term::update();
		
		update_time = (clock() - update_time) / (float)CLOCKS_PER_SEC;

		frame_time = (clock() - total_time) / (float)CLOCKS_PER_SEC;

		//Term::wait(16);
	}

	Term::end_curs();

	return 0;
}