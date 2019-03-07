#include "courbe.h" 

void draw_point(const Point3& point){
  glPointSize(4);
  glBegin(GL_POINTS);
    glVertex3d(point.x, point.y, point.z);
  glEnd();
}

void draw_line(const Point3& start, const Point3& end){
  glBegin(GL_LINES);
    glVertex3d(start.x, start.y, start.z);
    glVertex3d(end.x, end.y, end.z);
  glEnd();
}

void draw_curve(const std::vector<Point3>& points) {
	glBegin(GL_LINE_STRIP);
	for (size_t i = 0; i < points.size(); ++i) {
		glVertex3d(points[i].x, points[i].y, points[i].z);		
	}
	glEnd();
}

#define F1(U) (2 * pow(U, 3) - 3 * pow(U, 2) + 1) 
#define F2(U) (-2 * pow(U, 3) + 3 * pow(U, 2)) 
#define F3(U) (pow(U, 3) - 2 * pow(U, 2) + U) 
#define F4(U) (pow(U, 3) - pow(U, 2)) 

std::vector<Point3> hermite_cubic_curve(Point3 P0, Point3 P1, Vector3 V0, Vector3 V1, long nb_u){
	
	//std::vector<Point3> points(nb_u); bug de openGl quand on optimise l'allocation mémoire
	std::vector<Point3> points;

	for (int i = 0; i <= nb_u; ++i){
		double u = 1 / (double)nb_u * i;
		points.push_back(Point3(F1(u) * Vector3(P0) + F2(u) * Vector3(P1) + F3(u) * V0 + F4(u) * V1));
	}

	return std::move(points);
}

double fact(double k) {
	return k == 0 ? 1 : k * fact(k - 1);
}

double bernstein(double n, double i, double u) {
	return fact(n) / (fact(i) * fact(n - i)) * pow(u, i) * pow(1 - u, n - i);
}

std::vector<Point3> bezier_curve_bernstein(const std::vector<Point3>& control_points, long nb_u) {

	//std::vector<Point3> points(nb_u); bug de openGl quand on optimise l'allocation mémoire
	std::vector<Point3> points;
	
	for (long i = 0; i <= nb_u; ++i){
		
		double u = 1 / (double)nb_u * i;

		Vector3 point(0, 0, 0);

		for (size_t j = 0; j < control_points.size(); ++j){
			double b = bernstein(control_points.size() - 1, j, u);
			point += b * Vector3(control_points[j]);
		}

		points.push_back(Point3(point));
	}

	return std::move(points);
}

Vector3 casteljau_point(const std::vector<Point3>& control_points, double k, double i, double u){
	
	if(k == 0) 
		return Vector3(control_points[i]);

	Vector3 p1 = casteljau_point(control_points, k - 1, i, u);
	Vector3 p2 = casteljau_point(control_points, k - 1, i + 1, u);

	return ((1 - u) * p1) + (u * p2);
}


std::vector<Point3> bezier_curve_casteljau(const std::vector<Point3>& control_points, long nb_u) {
	
	//std::vector<Point3> points(nb_u); bug de openGl quand on optimise l'allocation mémoire.
	std::vector<Point3> points; 

	for (long i = 0; i <= nb_u; ++i){

		double u = 1 / (double)nb_u * i;

		Vector3 point = casteljau_point(control_points, control_points.size() - 1, 0, u);
		
		points.push_back(Point3(point));
	}

	return std::move(points);
}

void draw_surface_cylindrique(const std::vector<Point3>& courbe_bezier, const Point3& line_start, const Point3& line_end, long nb_v) {

	Vector3 direction = line_end - line_start;

	for (long i = 0; i <= nb_v; ++i){

		double v = 1 / (double)nb_v * i;

		std::vector<Point3> courbe_v;

		for (size_t j = 0; j < courbe_bezier.size(); ++j){
			Point3 point = courbe_bezier[j] + (1 - v) * direction; 
			courbe_v.push_back(point);
		}
		draw_curve(courbe_v);
	}

	return;
}

void draw_surface_reglee(const std::vector<Point3>& courbe_bezier1, const std::vector<Point3> courbe_bezier2, long nb_v) {

	for (long i = 0; i <= nb_v; ++i){

		double v = 1 / (double)nb_v * i;

		std::vector<Point3> courbe_v;

		for (size_t j = 0; j < courbe_bezier1.size(); ++j){

			Point3 point = Point3((1 - v) * Vector3(courbe_bezier1[j]) + v * Vector3(courbe_bezier2[j])); 

			courbe_v.push_back(point);
		}

		draw_curve(courbe_v);
	}

	return;
}

std::vector<std::vector<Point3> > bezier_surface_bernstein(const std::vector<std::vector<Point3> >& control_grid, long nb_u, long nb_v) {

	std::vector<std::vector<Point3> > surface;
	
	std::vector<std::vector<Point3> > control_u; // taille control_grid.size()
	
	glColor3f(1, 0, 0);

	// Dessin des courbes de controle en rouge

	for (size_t k = 0; k < control_grid.size(); ++k) {
		control_u.push_back(bezier_curve_bernstein(control_grid[k], nb_u));
		//draw_curve(control_u[k]);
	}

	std::vector<std::vector<Point3> > control_v; // taille nbu + 1

	// Dessin de la surface de bezier en jaune

	glColor3f(1, 1, 0);

	for (int u = 0; u <= nb_u; ++u){

		control_v.push_back(std::vector<Point3>());

		for (size_t k = 0; k < control_u.size(); ++k)
			control_v[u].push_back(control_u[k][u]);
		
		surface.push_back(bezier_curve_bernstein(control_v[u], nb_v));
	}

	return std::move(surface);
}

void draw_surface(const std::vector<std::vector<Point3> >& surface) {
	//glBegin(GL_QUADS); //	GL_QUAD_STRIP;
	for (size_t u = 0; u < surface.size() - 1; ++u) {
		for (size_t v = 0; v < surface[u].size() - 1; ++v) {
			glBegin(GL_QUAD_STRIP); //	GL_QUAD_STRIP;
				glVertex3d(surface[u][v].x, surface[u][v].y, surface[u][v].z);		
				glVertex3d(surface[u][v+1].x, surface[u][v+1].y, surface[u][v+1].z);		
				glVertex3d(surface[u+1][v].x, surface[u+1][v].y, surface[u+1][v].z);		
				glVertex3d(surface[u+1][v+1].x, surface[u+1][v+1].y, surface[u+1][v+1].z);
			glEnd();
		}
	}
	//glEnd();
}

void draw_surface_grid(const std::vector<std::vector<Point3> >& surface) {

	if (surface.empty())
		return;

	for (size_t u = 0; u < surface.size(); ++u) {
		draw_curve(surface[u]);
	}

	std::vector<std::vector<Point3> > reverse;

	for (size_t u = 0; u < surface[0].size(); ++u) {
		
		reverse.push_back(std::vector<Point3>());

		for (size_t v = 0; v < surface.size(); ++v) {
			reverse[u].push_back(surface[v][u]);
		}

		draw_curve(reverse[u]);
	}
}

Vector3 rotate_around_z(const Vector3& v, double angle) {
	Vector3 tmp;
	tmp.x = v.x * cos(angle) - v.y * sin(angle);
	tmp.y = v.x * sin(angle) + v.y * cos(angle);
	tmp.z = v.z;
	return tmp;
}
/*
Vector3 rotate_around_z(const Vector3& v, double angle) {
	Vector3 tmp = v;
	rotate_around_z(tmp, angle);
	return tmp;
}
*/
Vector3 rotate_around_y(const Vector3& v, double angle) {
	Vector3 tmp;
	tmp.x = v.x * cos(angle) + v.z * sin(angle);
	tmp.y = v.y;
	tmp.z = -v.x * sin(angle) + v.z * cos(angle);
	return tmp;
}
/*
Vector3 rotate_around_around_y(const Vector3& v, double angle) {
	Vector3 tmp = v;
	rotate_around_y(tmp, angle);
	return tmp;
}
*/
Vector3 rotate_around_x(const Vector3& v, double angle) {
	Vector3 tmp;
	tmp.x = v.x;
	tmp.y = v.y * cos(angle) - v.z * sin(angle);
	tmp.z = v.y * sin(angle) + v.z * cos(angle);
	return tmp;
}
/*
Vector3 rotate_around_x(const Vector3& v, double angle) {
	Vector3 tmp = v;
	rotate_around_x(tmp, angle);
	return tmp;
}
*/
Vector3 rotate_around(const Vector3& axis, double angle) {
	Vector3 tmp;

	tmp.x = cos(angle) + (axis.x * axis.x) * (1 - cos(angle)) +
		  (axis.x * axis.y) * (1 - cos(angle)) - axis.z * sin(angle) +
		  (axis.x * axis.z) * (1 - cos(angle)) + axis.y * sin(angle);

	tmp.y = (axis.y * axis.x) * (1 - cos(angle)) + axis.z * sin(angle) +
		  cos(angle) + (axis.y * axis.y) * (1 - cos(angle)) +
		  (axis.y * axis.z) * (1 - cos(angle)) - axis.x * sin(angle);

	tmp.z = (axis.z * axis.x) * (1 - cos(angle)) - axis.y * sin(angle) +
		  (axis.z * axis.y) * (1 - cos(angle)) + axis.x * sin(angle) +
		  cos(angle) + (axis.z * axis.z) * (1 - cos(angle));

	return tmp;
}
/*
Vector3 rotate_around(const Vector3& v, const Vector3& axis, double angle) {
	Vector3 tmp = v;
	rotate_around(v, axis, angle);
	return tmp;
}
*/
std::vector<std::vector<Point3> > facettes_cylindre(const Point3& line_start, const Point3& line_end, double rayon, size_t nb_meridiens) {

	std::vector<std::vector<Point3> > facettes;

	double angle = (M_PI * 2) / (double)nb_meridiens;
	double length = (line_end - line_start).length();

	//Vector3 axis = (line_end - line_start).normalized();
	
	for (size_t i = 0; i <= nb_meridiens; ++i) {
		
		facettes.push_back(std::vector<Point3>());
		
		double alpha = angle * i;
		
		//Vector3 transform = rotate_around(axis, alpha);
		//Point3 p1 = line_start + transform;
		//Point3 p2 = line_end + transform;
		
		Point3 p1 = line_start + Vector3(rayon * cos(alpha), rayon * sin(alpha), -length/2);
		Point3 p2 = line_end + Vector3(rayon* cos(alpha), rayon * sin(alpha), length/2);
		
		//std::swap(p1.x, p1.z);
		//std::swap(p2.x, p2.z);

		draw_point(p1);	
		draw_point(p2);
		
		facettes[i].push_back(p1);
		facettes[i].push_back(p2);
	}

	return std::move(facettes);
}

void draw_facettes(const std::vector<std::vector<Point3> >& facettes) {
	glBegin(GL_QUAD_STRIP); //	GL_QUAD_STRIP;
	
	for (size_t i = 0; i < facettes.size() - 1; ++i) {
		for (size_t j = 0; j < facettes[i].size(); ++j) {
			glVertex3d(facettes[i][j].x, facettes[i][j].y, facettes[i][j].z);		
			glVertex3d(facettes[i+1][j].x, facettes[i+1][j].y, facettes[i+1][j].z);
		}
	}

	if (facettes.size() > 1){
		for (size_t j = 0; j < facettes[facettes.size() - 1].size(); ++j) {
			glVertex3d(facettes[facettes.size() - 1][j].x, facettes[facettes.size() - 1][j].y, facettes[facettes.size() - 1][j].z);		
			glVertex3d(facettes[0][j].x, facettes[0][j].y, facettes[0][j].z);
		}
	}

	glEnd();
}

void draw_facettes_grid(const std::vector<std::vector<Point3> >& facettes) {
	for (size_t i = 0; i < facettes.size() - 1; ++i) {
		for (size_t j = 0; j < facettes[i].size() - 1; ++j) {
			draw_line(facettes[i][j], facettes[i+1][j]);
			draw_line(facettes[i][j], facettes[i][j+1]);

			draw_line(facettes[i][j+1], facettes[i+1][j+1]);
			draw_line(facettes[i+1][j], facettes[i+1][j+1]);
		}
	}

	if (facettes.size() > 1){
		for (size_t j = 0; j < facettes[facettes.size() - 1].size() - 1; ++j) {
			draw_line(facettes[facettes.size() - 1][j], facettes[0][j]);
			draw_line(facettes[facettes.size() - 1][j], facettes[facettes.size() - 1][j+1]);

			draw_line(facettes[facettes.size() - 1][j+1], facettes[0][j+1]);
			draw_line(facettes[0][j], facettes[0][j+1]);
		}
	}
}

void draw_vect(const Point3& p, const Vector3& v) {

	// Sauvegarde de la couleur initiale

	GLfloat current_color[4] = {0};
	glGetFloatv(GL_CURRENT_COLOR, current_color);

	// Affiche les composantes du vecteur

	Vector3 x = Vector3(v.x, 0, 0);
	Vector3 y = Vector3(0, v.y, 0);
	Vector3 z = Vector3(0, 0, v.z);

	glColor3f(1, 0, 0);
	draw_line(p, p + x);
	
	glColor3f(0, 1, 0);
	draw_line(p, p + y);

	glColor3f(0, 0, 1);
	draw_line(p, p + z);

	// Affiche le vecteur avec ça pointe
	
	glColor3f(1, 0, 1);
	draw_line(p, p + v);
	draw_point(p + v);

	// Affiche le point de depart

	glColor3f(1, 1, 1);
	draw_point(p);

	// Reset color
	
	glColor3f(current_color[0], current_color[1], current_color[2]);
}

std::vector<std::vector<Point3> > facettes_cone(const Point3& line_start, const Point3& line_end, double rayon, size_t nb_meridiens) {

	std::vector<std::vector<Point3> > facettes;

	double angle = (360 / (double)nb_meridiens) * (M_PI / 180.0);
	double length = (line_end - line_start).length();

	//Vector3 axis = (line_end - line_start).normalized();
	
	for (size_t i = 0; i < nb_meridiens; ++i) {
		
		facettes.push_back(std::vector<Point3>());
		
		double alpha = angle * i;
		
		//Vector3 transform = rotate_around(axis, alpha);
		//Point3 p1 = line_start + transform;
		//Point3 p2 = line_end + transform;
		
		Point3 p1 = line_start + Vector3(rayon * cos(alpha), rayon * sin(alpha), -length/2);
		Point3 p2 = line_end;

		//std::swap(p1.x, p1.z);
		//std::swap(p2.x, p2.z);

		draw_point(p1);	
		draw_point(p2);
		
		facettes[i].push_back(p1);
		facettes[i].push_back(p2);
	}

	return std::move(facettes);
}

std::vector<std::vector<Point3> > facettes_sphere(const Point3& center, double radius, size_t meridiens, size_t paralleles)  {

	std::vector<std::vector<Point3> > facettes;

	double angle_parallele = (M_PI * 2) / (double)paralleles;
	double angle_meridien = M_PI / (double)meridiens;

	for (size_t i = 0; i <= paralleles; ++i) {
		
		facettes.push_back(std::vector<Point3>());

		double beta = angle_parallele * i;

		for (size_t j = 0; j <= meridiens; ++j) {
			
			double alpha = (M_PI/2) - (angle_meridien * j);

			//Point3 p = center + ( Vector3(cos(alpha), sin(alpha), 1) + Vector3(cos(beta), sin(beta)) ) * radius;
			//Point3 p = center + Vector3(cos(beta)*cos(alpha), cos(beta) * sin(alpha), sin(beta)) * radius;
			
			Point3 p = center + Vector3(cos(alpha) * cos(beta), cos(alpha) * sin(beta), sin(alpha)) * radius;
			//std::swap(p.x, p.z);
			//Point3 p = center + Vector3(radius, cos(beta), sin(alpha)) * radius;

			//draw_point(p);
			//draw_point(p2);

			facettes[i].push_back(p);
		}
	}

	return std::move(facettes);
}



void draw_facettes_sphere(const std::vector<std::vector<Point3> >& facettes) {
	
	glBegin(GL_QUAD_STRIP);
	
	for (size_t i = 0; i < facettes.size(); ++i) {
		for (size_t j = 0; j < facettes[i].size()-1; ++j) {
			glVertex3d(facettes[i][j].x, facettes[i][j].y, facettes[i][j].z);		
			glVertex3d(facettes[i][j+1].x, facettes[i][j+1].y, facettes[i][j+1].z);
		}
	}

	if (facettes.size() > 1){
		for (size_t j = 0; j < facettes[facettes.size()].size(); ++j) {
			glVertex3d(facettes[facettes.size()-1][j].x, facettes[facettes.size()-1][j].y, facettes[facettes.size()-1][j].z);		
			glVertex3d(facettes[0][j].x, facettes[0][j].y, facettes[0][j].z);
		}
	}

	glEnd();
}

/*
std::vector<std::vector<Point3> > facettes_polygone(const Point3& center, double radius, size_t sides) {
	std::vector<std::vector<Point3> > facettes;

	double angle = (360 / (double)nb_meridiens) * (M_PI / 180.0);
	double length = (line_end - line_start).length();

	//Vector3 axis = (line_end - line_start).normalized();
	
	for (size_t i = 0; i < nb_meridiens; ++i) {
		
		facettes.push_back(std::vector<Point3>());
		
		double alpha = angle * i;
		
		//Vector3 transform = rotate_around(axis, alpha);
		//Point3 p1 = line_start + transform;
		//Point3 p2 = line_end + transform;
		
		Point3 p1 = line_start + Vector3(rayon * cos(alpha), rayon * sin(alpha), -length/2);
		Point3 p2 = line_end;

		std::swap(p1.x, p1.z);
		std::swap(p2.x, p2.z);

		draw_point(p1);	
		draw_point(p2);
		
		facettes[i].push_back(p1);
		facettes[i].push_back(p2);
	}
	return std::move(facettes);	
}
*/