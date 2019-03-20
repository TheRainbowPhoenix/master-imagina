#include <iostream> 
#include "Image.h"

using namespace std;


void set_neighbors(ImagePGM& image, int i, int j, int value, int offset) {
	image(i + offset, j) = value;
	image(i - offset, j) = value;
	image(i, j + offset) = value;
	image(i, j - offset) = value;
}

void set_neighbors_diag(ImagePGM& image, int i, int j, int value, int offset) {
	image(i + offset, j + offset) = value;
	image(i - offset, j + offset) = value;
	image(i - offset, j - offset) = value;
	image(i + offset, j - offset) = value;
}

ImagePGM init_chamfrein(vector<int>& param, size_t radius) {
	ImagePGM out(radius * 2 + 1, radius * 2 + 1);

	for (size_t i = 0 ; i < param.size() ; ++i) {
		if ((i + 1) % 2 != 0) {
			set_neighbors(out, 0, 0, param[i], i);
		} else {
			set_neighbors_diag(out, 0, 0, param[i], i);
		}
	}

	return std::move(out);
}

int main(int argc, char const *argv[])
{
	if (argc < 2) {
		cerr << "Usage : " << argv[0] << " <filename>\n";
		return 1;
	}

	ImagePGM in(argv[1]);

	cout << in;

	return 0;
}

