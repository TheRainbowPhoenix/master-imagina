#include "Particle.h"

//#DEFINITION
////////////////////////////////////////////////// CONSTRUCTEURS
 
Particle::Particle() : position(), velocity(random(-1, 1), random(-1, -1)), acceleration(), birth(), lifetime(), mass(1) {}

Particle::Particle(const Vector2f& position, float lifetime) : position(position), velocity(random(-1, 1), random(-1, -1)), acceleration(), birth(lifetime), lifetime(lifetime), mass(1){}

Particle::Particle(const Vector2f& pos, const Vector2f& v, float lifetime) : position(pos), velocity(v), acceleration(), birth(lifetime), lifetime(lifetime), mass(1) {} 

////////////////////////////////////////////////// METHODES

void Particle::apply_force(const Vector2f& force){
	acceleration += (force / mass);
}

void Particle::update() { 
	velocity += acceleration;
	position += velocity;
	acceleration = Vector2f::zero;
	lifetime -= 1.0;
}

void Particle::display(Canvas& c) {
	
	int intensity = ((int)lifetime * 255) / (int)birth;

	chtype CS = ColorScale(intensity);

	c.set_on(CS);
	c.set(position.x, position.y);
	c.set_off(CS);
	//c.toggle(position.rounded());
}

void Particle::run(Canvas& c) {
	update();
	display(c);
}

bool Particle::is_alive() {
	return lifetime > 0;
}

////////////////////////////////////////////////// CONSTRUCTEURS

Attractor::Attractor() : position(), strength(100), mass(1) {}

Attractor::Attractor(const Vector2f& position) : position(position), strength(100), mass(1) {}

////////////////////////////////////////////////// METHODES

Vector2f Attractor::attract(const Particle& p) const {

	Vector2f dir = position - p.position;
	
	float dist_squared = dir.length_squared();

	//constrain dist pour ne pas perdre le controle des particules trop rapides
	dist_squared = constrain(dist_squared, 100, 1000000000000);

	dir.normalize();

	float force = (strength * mass * p.mass) / dist_squared;
	
	return dir * force;
}

////////////////////////////////////////////////// CONSTRUCTEURS

Repeller::Repeller() : position(), strength(1), mass(1) {}

Repeller::Repeller(const Vector2f& position) : position(position), strength(100), mass(1) {}

////////////////////////////////////////////////// METHODES

Vector2f Repeller::repel(const Particle& p) const {

	Vector2f dir = position - p.position;
	
	float dist_squared = dir.length_squared();

	//constrain dist pour ne pas perdre le controle des particules trop rapides
	dist_squared = constrain(dist_squared, 100, 10000000000000);

	dir.normalize();

	float force = (strength * mass * p.mass) / dist_squared; 

	return -dir * force;
}

////////////////////////////////////////////////// CONSTRUCTEURS

ParticleSystem::ParticleSystem() : particles(), particles_forces(), duration(1000), velocity_range(), velocity_over_time() {}

ParticleSystem::ParticleSystem(float duration, IntRect velocity_range, float velocity_over_time) : particles(), particles_forces(),  duration(duration), velocity_range(velocity_range), velocity_over_time(velocity_over_time) {}
 
////////////////////////////////////////////////// METHODES

void ParticleSystem::apply_force(const Vector2f& force){
	
	for(std::list<Particle>::iterator it = particles.begin() ; it != particles.end() ; ++it){
		it->apply_force(force);
	}
	
/*
	for(std::list<Vector2f>::iterator it = particles_forces.begin() ; it != particles_forces.end() ; ++it){
		*it += force;
	}
*/
}

void ParticleSystem::apply_repeller(const Repeller& rep){
	for(std::list<Particle>::iterator it = particles.begin() ; it != particles.end() ; ++it){
		Vector2f force = rep.repel(*it);
		it->apply_force(force);
	}
}

void ParticleSystem::apply_attractor(const Attractor& att){
	for(std::list<Particle>::iterator it = particles.begin() ; it != particles.end() ; ++it){
		Vector2f force = att.attract(*it);
		it->apply_force(force);
	}
}

void ParticleSystem::add_particles(int n, Vector2f origin, float radius){

		for(int i = 0 ; i < n ; i++){
			
			float angle = random(0, 1) * M_PI * 2;
			float rand_num = random(0, 1) + random(0, 1);
			float r = rand_num > 1 ? (2 - rand_num) * radius : rand_num * radius;

			particles.push_back(Particle(origin, 
										 Vector2f(r * cos(angle), r * sin(angle)), 
										 duration));
		//particles_forces.push_back(Vector2f::zero);
		}
}

void ParticleSystem::add_particles(int n, IntRect zone){
	for(int i = 0 ; i < n ; i++){
		particles.push_back(Particle(Vector2f(random(zone.x, zone.width), random(zone.y, zone.height)), 
									 Vector2f(random(velocity_range.x, velocity_range.y), random(velocity_range.width, velocity_range.height)), 
									 duration));
		//particles_forces.push_back(Vector2f::zero);
	}
}

void ParticleSystem::run(Canvas& c){
/*
	std::list<Vector2f>::iterator itf = particles_forces.begin();

	for(std::list<Particle>::iterator it = particles.begin(); it != particles.end() ; ++it){
		it->apply_force(*itf);
		itf++;
	}

	itf = particles_forces.begin();
*/
	for(std::list<Particle>::iterator it = particles.begin() ; it != particles.end() ; ++it){
		
		if(it->is_alive()){
			it->run(c);
			it->velocity *= velocity_over_time;
		}
		else{
			it = particles.erase(it); //met a jour l'iterateur pour pointer vers l'iterateur suivant
			//itf = particles_forces.erase(itf);
		}
		//itf++;
	}
}

////////////////////////////////////////////////// FONCTIONS DEFINITION

float random(int start, int end, int precision){

	int longueur = abs(abs(end) - start);

	int randlong = (rand() % ((longueur * precision) + 1));

	return (float) start + ((float) randlong / (float) precision);
}

float constrain(float f, float start, float end){
	return f < start ? start : f > end ? end : f;  
}

//#DEFINITION_END