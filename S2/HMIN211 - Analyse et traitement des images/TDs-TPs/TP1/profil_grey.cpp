#include <stdio.h>
#include <iostream>
#include <vector>
#include "ImageBase.h"

using namespace std;

bool out_of_range_col(const ImageBase& image, int index){
  return index < 0 || index >= image.getWidth();  
}

bool out_of_range_raw(const ImageBase& image, int index) {
  return index < 0 || index >= image.getHeight();
}

int main(int argc, char **argv) {

  if (argc != 4) {
    cerr << "Usage : <STRING:image.pgm> <STRING:col/raw> <NOMBRE:indice>\n";
    return 1;
  }

  ImageBase image_in;
  image_in.load(argv[1]);
  string profil_type(argv[2]);
  int index = atoi(argv[3]);

  if (profil_type == "col") {

      if (!out_of_range_col(image_in, index))
        for (int x = 0; x < image_in.getHeight(); ++x)
          cout << x << " " << (int)image_in[x][index] << "\n";
      else
        cerr << "Erreur : Index out of range\n";

  } else if (profil_type == "raw") {

      if (!out_of_range_raw(image_in, index))
        for (int y = 0; y < image_in.getWidth(); ++y)
          cout << y << " " << (int)image_in[index][y] << "\n";
      else
        cerr << "Erreur : Index out of range\n";

  } else {
    cerr << "Erreur : veuillez choisir entre 'col' et 'raw'\n";
    return 1;
  }

  return 0;
}
