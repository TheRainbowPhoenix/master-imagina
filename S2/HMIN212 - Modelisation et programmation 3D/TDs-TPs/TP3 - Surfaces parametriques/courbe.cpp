#include "courbe.h" 

void draw_point(const Point3& point){
  glPointSize(4);
  glBegin(GL_POINTS);
    glVertex3dv(point);
  glEnd();
}

void draw_line(const Point3& start, const Point3& end){
  glBegin(GL_LINES);
    glVertex3dv(start);
    glVertex3dv(end);
  glEnd();
}

void draw_curve(const std::vector<Point3>& points) {
	glBegin(GL_LINE_STRIP);
	for (size_t i = 0; i < points.size(); ++i) {
		glVertex3dv(points[i]);		
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

std::vector<Point3> bezier_surface_bernstein(const std::vector<std::vector<Point3> >& control_grid, long nb_u, long nb_v) {

	std::vector<Point3> surface;
	std::vector<std::vector<Point3> > courbes;
	
	for (size_t k = 0; k < control_grid.size(); ++k) {
		courbes.push_back(bezier_curve_bernstein(control_grid[k], nb_u));
		draw_curve(courbes[k]);
	}

	std::vector<Point3> surface_control(courbes.size());

	for (int u = 0; u <= nb_u; ++u){

		for (size_t l = 0; l < courbes.size(); ++l){
			surface_control[l] = courbes[l][u];
		}

		std::vector<Point3> res;
		res = bezier_curve_bernstein(surface_control, nb_v);
		draw_curve(res);

		surface.insert(surface.end(), res.begin(), res.end());
	}

	return std::move(surface);
}

void draw_surface(const std::vector<Point3>& points){
	glBegin(GL_POINTS); //	GL_QUAD_STRIP;
	for (size_t i = 0; i < points.size(); ++i) {
		glVertex3dv(points[i]);		
	}
	glEnd();
}
