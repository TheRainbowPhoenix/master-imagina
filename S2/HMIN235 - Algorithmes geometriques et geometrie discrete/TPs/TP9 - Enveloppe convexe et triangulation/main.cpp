#include <iostream> 
#include <cmath>
#include <ctime>
#include <random>
#include <algorithm>
#include <stack>
#include "Image.h"

using namespace std;

struct Point
{
	int x, y;

	Point() : x(), y() {}
	Point(int x, int y) : x(x), y(y) {}

};

Point operator-(Point right, Point left)
{
	return Point(right.x - left.x, right.y - left.y);
}

long long lengthSquared(Point v)
{
	return (v.x * v.x) + (v.y * v.y);
}

long long dot(Point v1, Point v2)
{
	return (v1.x * v2.x) + (v1.y * v2.y);
}

long long cross(Point v1, Point v2)
{
	return (v1.x * v2.y) - (v1.y * v2.x);
}

std::ostream& operator<<(std::ostream& os, const Point& p)
{
	return os << "(" << p.x << ", " << p.y << ")";
}

std::istream& operator>>(std::istream& is, Point& p)
{
	return is >> p.x >> p.y; 
}

void draw_line(ImagePGM& image, int x1, int y1, int x2, int y2, int intensity = 255)
{
	int xdiff = std::max(x1, x2) - std::min(x1, x2);
	int ydiff = std::max(y1, y2) - std::min(y1, y2);
	int xdir = x1 <= x2 ? 1 : -1;
	int ydir = y1 <= y2 ? 1 : -1;

	int r = std::max(xdiff, ydiff);

	for (int i = 0; i < r + 1; ++i)
	{
		int x = x1;
		int y = y1;

		if (ydiff)
			y += (float(i) * ydiff) / r * ydir;
		if (xdiff)
			x += (float(i) * xdiff) / r * xdir;

		image(y, x) = intensity;
	}
}

void draw_line(ImagePGM& image, Point p1, Point p2, int intensity = 255)
{
	draw_line(image, p1.x, p1.y, p2.x, p2.y, intensity);
}

void to_lower(string& str)
{
	for (char& c : str)
		c = std::tolower(c);
}


void randomize_points(vector<Point>& points, Point center, int radius)
{
	default_random_engine generator(time(0));

	uniform_real_distribution<double> angle_distribution(0, 2 * M_PI);
	uniform_real_distribution<double> radius_distribution(0, 1);
	
	for (Point& p : points)
	{
		double random_angle = angle_distribution(generator);

		// sqrt() permet de rendre la distribution uniforme dans le rayon. 
		double random_radius = sqrt(radius_distribution(generator)) * radius;

		p.x = center.x + cos(random_angle) * random_radius;
		p.y = center.y + sin(random_angle) * random_radius; 
	} 
}

bool concave(Point r, Point s, Point t)
{
	Point vec_r_s = s - r;
	Point vec_r_t = t - r;

	return cross(vec_r_s, vec_r_t) <= 0;
}

Point find_pivot(vector<Point>& points)
{
	Point min = points[0];

	for (Point p : points)
	{
		if ((p.x < min.x) || (p.x <= min.x && p.y > min.y))
		{
			min = p;
		}
	}

	return min;
}

// le pivot est le centre de l'enveloppe
vector<Point> graham(vector<Point>& points)
{
	// recherche du pivot
	auto pivot_iter = min_element(points.begin(), points.end(), [](Point& p1, Point& p2) -> bool
	{
		return (p1.x < p2.x) || (p1.x <= p2.x && p1.y > p2.y);
	});

	Point pivot = *pivot_iter;
	iter_swap(points.begin(), pivot_iter);

	// trie des point selon l'angle du vecteur pivot/point
	sort(points.begin() + 1, points.end(), [pivot](Point& p1, Point& p2) -> bool
	{
		Point vec_pivot_p1 = (p1 - pivot);
		Point vec_pivot_p2 = (p2 - pivot);

		double angle_pivot_p1 = atan2(vec_pivot_p1.y, vec_pivot_p1.x); 
		double angle_pivot_p2 = atan2(vec_pivot_p2.y, vec_pivot_p2.x); 

		if (angle_pivot_p1 == angle_pivot_p2)
		{
			return lengthSquared(vec_pivot_p1) < lengthSquared(vec_pivot_p2); // on prend le point le plus loin si les angles sont egaux
		}

		return angle_pivot_p1 < angle_pivot_p2;
	});

	vector<Point> parcour = { points[0], points[1] };

	for (size_t i = 2 ; i < points.size() ; ++i)
	{
		while (parcour.size() > 1 && concave(points[i], parcour[parcour.size() - 2], parcour.back()))
		{
			parcour.pop_back();
		}

		parcour.push_back(points[i]);
	}

	// reverse(parcour.begin(), parcour.end());

	return std::move(parcour);
}

void draw_parcours(ImagePGM& image, vector<Point> parcours)
{
	for (size_t i = 0; i < parcours.size() - 1; ++i)
	{
		draw_line(image, parcours[i], parcours[i+1]);
	}

	if (parcours.size() >= 1)
	{
		draw_line(image, parcours[parcours.size() - 1], parcours[0]);
	}

	for (Point p : parcours)
	{
		image(p.y, p.x) = 128;
	}
}

int main(int argc, char const *argv[])
{
	if (argc < 4)
	{
		cerr << "Usage : " << argv[0] << " <x_max> <y_max> <nb_points>\n";
		exit(EXIT_FAILURE);
	}

	int x_max = atoi(argv[1]);
	int y_max = atoi(argv[2]);
	int nb_points = atoi(argv[3]);

	ImagePGM out(x_max + 1, y_max + 1);

	vector<Point> points(nb_points);

	int radius = min(x_max, y_max) / 2;
	Point center(x_max / 2, y_max / 2);

	randomize_points(points, center, radius);

	for (Point p : points)
	{
		// cerr << p << "\n";
		out(p.y, p.x) = 255;
	}

	vector<Point> parcours;

	Point pivot = find_pivot(points);
	parcours = graham(points);

	draw_parcours(out, parcours);
	
	out(pivot.y, pivot.x) = 64;

	cout << out;

	return 0;
}

