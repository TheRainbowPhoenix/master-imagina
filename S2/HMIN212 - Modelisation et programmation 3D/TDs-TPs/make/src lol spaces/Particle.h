#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "Canvas.h"
#include <cstdlib>
 
//#PREPROCESSING
#include <list>
//#PREPROCESSING_END

//#DECLARATION
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ CLASS PARTICLE

class Particle {

public:
	Vector2f position;
	Vector2f velocity;
	Vector2f acceleration;
	float birth;
	float lifetime;
	float mass;

	Particle();
	Particle(const Vector2f& position, float lifetime = 100);
	Particle(const Vector2f& pos, const Vector2f& v, float lifetime = 100);

	void apply_force(const Vector2f& force);

	void update();

	void display(Canvas& c);

	void run(Canvas& c);

	bool is_alive();

};
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ CLASS ATTRACTOR

class Attractor {

public:

	Vector2f position;
	float strength; //Force du champ d'attraction
	float mass;

	Attractor();
	Attractor(const Vector2f& position);

	Vector2f attract(const Particle& p) const;
};

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ CLASS REPELLER

class Repeller {

public:

	Vector2f position;
	float strength; //Force du champ de repulsion
	float mass;
	Repeller();
	Repeller(const Vector2f& position);

	Vector2f repel(const Particle& p) const;
};

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ CLASS PARTICLESYSTEM

class ParticleSystem {

public:
	std::list<Particle> particles;
	std::list<Vector2f> particles_forces;

	float duration;

	IntRect velocity_range;
	float velocity_over_time;
	//int max_particles;

	ParticleSystem();
	ParticleSystem(float duration, IntRect velocity_range = IntRect(0, 0, 0, 0), float velocity_over_time = 1);

	void apply_force(const Vector2f& force);
	
	void apply_repeller(const Repeller& rep);
	void apply_attractor(const Attractor& att);

	void add_particles(int n, Vector2f origin, float radius = 1);
	void add_particles(int n, IntRect zone);
	
	void run(Canvas& c);
};

////////////////////////////////////////////////// FONCTIONS DECLARATION

/* genere un nombre flotant aléatoire entre "start" et "end", precision permet de stypulé le nombre max de zero aprés la virgule */
float random(int start, int end, int precision = 100000);

/* Oblige le nombre floatant f à être entre "start" et "end" */
float constrain(float f, float start, float end);

//#DECLARATION_END

#endif
