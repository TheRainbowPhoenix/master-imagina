#include <iostream>
#include "Image.h"

using namespace std;
/*
#define FOND 255
#define OBJET 0

bool on_bound(const ImagePGM& image, int raw, int column) {
  return raw >= 0 && raw < image.height() && column >= 0 && column < image.width(); 
}

bool is_fond(int value){
  return value == FOND;
}

bool is_objet(int value){
  return value == OBJET;
}

void difference(ImagePGM& image_in, ImagePGM& image_dilatee, ImagePGM& image_out) {
  for (int i = 0; i < image_in.height(); ++i) {
    for (int j = 0; j < image_in.width(); ++j) {
      if (is_fond(image_in[i][j]) && is_fond(image_dilatee[i][j]))
        image_out[i][j] = FOND;
      else if (is_objet(image_in[i][j]) && is_objet(image_dilatee[i][j]))
        image_out[i][j] = FOND;
      else if (is_objet(image_dilatee[i][j] && is_fond(image_in[i][j])))
        image_out[i][j] = OBJET;
    }
  }
}

*/

int main(int argc, char** argv) {
  if (argc < 2) {
    printf("Usage: <ImageIn.pgm>\n");
    return 1;
  }

  vector<int> seuils;

  for (int i = 3; i < argc; ++i) {
    seuils.push_back(atoi(argv[i]));
  }

  std::cerr << "Creation objet...\n";

  //ImagePPM image_in(ImagePGM("baboonG.pgm"), ImagePGM("baboonB.pgm"), ImagePGM("baboonR.pgm"));

  ImagePPM image_in(argv[1]);

  //red.map([](OCTET& o){cout << o << "\n";});

  //ImagePGM image_in;
  //cin >> image_in;

  if (!image_in.valid()) {
    std::cerr << "Warning : invalid image\n";
  }

  std::cerr << "Ecriture objet...\n";

  cout << image_in;

  std::cerr << "fin\n";

  /*
  ImagePGM image_out(image_in);

  cout << "Separatorooooo \n\n\n";
  //image_dilatee.load(argv[2]);

  cout << image_out;
*/
  //difference(image_in, image_dilatee, image_out);

  //image_out.save(argv[2]);

  return 0;
}