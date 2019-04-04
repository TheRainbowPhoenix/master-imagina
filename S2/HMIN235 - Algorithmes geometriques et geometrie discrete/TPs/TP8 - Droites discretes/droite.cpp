#include <iostream> 
#include <cmath>
#include "Image.h"
#include "Vector3.h"
#include "Point3.h"

using namespace std;

void bresenham_line(ImagePGM& image, int x0, int y0, int x1, int y1, OCTET color = 255)
{
	int dx = x1 - x0;
	int dy = y1 - y0;

	int innerHor = 2 * dy;
	int innerDiag = 2 * (dx - dy);

	int e = 2 * dy - dx;

	for (int x = x0, y = y0 ; x <= x1 ; ++x)
	{
		image(y, x) = color;
		
		if (e >= 0)
		{
			y++;
			e -= innerDiag;
		}
		else
		{
			e += innerHor;
		}
	}
}

void bresenham_line_v2(ImagePGM& image, int x0, int y0, int x1, int y1, OCTET color = 255)
{
	int dx =  abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1; 
	int err = dx + dy, e2; /* error value e_xy */

	for ( ; ; )
	{
		if (x0==x1 && y0==y1) 
			break;
		
		image(y0, x0) = color;
		
		e2 = 2*err;

		if (e2 >= dy)
		{ 
			err += dy; 
			x0 += sx;
		}

		if (e2 <= dx)
		{
			err += dx;
			y0 += sy;
		}
	}
}

void reveilles_line(ImagePGM& image, int x0, int y0, int x1, int y1, OCTET color = 255)
{
	int dx = x1 - x0;
	int dy = y1 - y0;

	int u = dy * x0 - dx * y0;
	int r = dy * x0 - dx * y0 - u;
	
	int x = x0;
	int y = y0;

	while (x <= x1)
	{
		image(y, x) = color;

		x++;
		r += dy;

		if (r < 0 || r >= dx)
		{
			y++;
			r -= dx;
		}
	}
}

void thick_line(ImagePGM& image, int x0, int y0, int x1, int y1, int width, OCTET color = 255)
{
	int dx = x1 - x0;
	int dy = y1 - y0;

	double length = sqrt(dx * dx + dy * dy);

	int perp_dx = round(dy / length);
	int perp_dy = round(-dx / length);

	int i = 1;

	while (i <= width)
	{
		reveilles_line(image, x0 - perp_dx * i, y0 - perp_dy * i, x1 - perp_dx * i, y1 - perp_dy * i);
		i++;
	}
}

void thick_line_v2(ImagePGM& image, int x0, int y0, int x1, int y1, int width, OCTET color = 255)
{
	int dx = abs(x1-x0), sx = x0 < x1 ? 1 : -1;
	int dy = abs(y1-y0), sy = y0 < y1 ? 1 : -1;

	int err = dx - dy, e2, x2, y2; /* error value e_xy */
	float ed = dx + dy == 0 ? 1 : sqrt((float)dx * dx + (float) dy * dy);

	for (width = (width + 1) / 2 ; ; )
	{
		image(y0, x0) = color;

		e2 = err; x2 = x0;

		if (2*e2 >= -dx) /* x step */
		{
			for (e2 += dy, y2 = y0; e2 < ed*width && (y1 != y2 || dx > dy); e2 += dx)
				image(y2 += sy, x0) = color;

			if (x0 == x1)
				break;

			e2 = err;
			err -= dy;
			x0 += sx;
		}

		if (2*e2 <= dy) /* y step */
		{
			for (e2 = dx-e2 ; e2 < ed*width && (x1 != x2 || dx < dy) ; e2 += dy)
				image(y0, x2 += sx) = color;
			
			if (y0 == y1)
				break;
			
			err += dx;
			y0 += sy;
		}
	}
}

void to_lower(string& str)
{
	for (char& c : str)
		c = std::tolower(c);
}

int main(int argc, char const *argv[])
{
	if (argc < 5)
	{
		cerr << "Usage : " << argv[0] << " <X0 Y0 X1 Y1> [BRESENHAM|REVEILLES|THICK] <WIDTH>\n";
		exit(EXIT_FAILURE);
	}

	int x0 = atoi(argv[1]);
	int y0 = atoi(argv[2]);
	int x1 = atoi(argv[3]);
	int y1 = atoi(argv[4]);

	int width = 1;

	string algo = "bresenham";

	if (argc > 5)
		algo = argv[5];

	to_lower(algo);

	if (argc > 6)
		width  = atoi(argv[6]);
	
	ImagePGM out(1000, 1000);

	if (algo == "reveilles")
		reveilles_line(out, x0, y0, x1, y1);
	else if (algo == "thick")
		thick_line_v2(out, x0, y0, x1, y1, width);
	else
		bresenham_line(out, x0, y0, x1, y1);

	cout << out;

	return 0;
}

