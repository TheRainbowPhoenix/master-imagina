#include <stdio.h>
#include <iostream>
#include <vector>
#include "../librairie/custom_c++/ImageBase.h"

using namespace std;

bool on_bound(const ImagePGM& image, int raw, int column) {
  return raw >= 0 && raw < image.height() && column >= 0 && column < image.width(); 
}

void assign_voisins_4(ImagePGM& image, int raw, int column, int value){
  if (on_bound(image, raw + 1, column))
    image[raw + 1][column] = value;

  if (on_bound(image, raw - 1, column))
    image[raw - 1][column] = value;

  if (on_bound(image, raw, column + 1))
    image[raw][column + 1] = value;

  if (on_bound(image, raw, column - 1))
    image[raw][column - 1] = value;
}

void assign_voisins_8(ImagePGM& image, int raw, int column, int value){
  assign_voisins_4(image, raw, column, value);

  if (on_bound(image, raw + 1, column + 1))
    image[raw + 1][column + 1] = value;

  if (on_bound(image, raw + 1, column - 1))
    image[raw + 1][column - 1] = value;

  if (on_bound(image, raw - 1, column + 1))
    image[raw - 1][column + 1] = value;

  if (on_bound(image, raw - 1, column - 1))
    image[raw - 1][column - 1] = value;  
}

void assign_voisins(ImagePGM& image, int raw, int column, int value, bool are_vois_8){
  if (are_vois_8)
    assign_voisins_8(image, raw, column, value);
  else
    assign_voisins_4(image, raw, column, value);
}

void fermeture(ImagePGM& image_in, ImagePGM& image_out, int value, bool are_vois_8) {
  for (int i = 0; i < image_in.height(); ++i) {
    for (int j = 0; j < image_in.width(); ++j) {
      if (image_in[i][j] == value)
        assign_voisins(image_out, i, j, value, are_vois_8);
    }
  }
}

int main(int argc, char** argv) {
  if (argc < 4) {
    printf("Usage: <ImageIn.pgm> <ImageOut.pgm> <String:Dilatation/Erosion> <NB_VOIS> <NB_EXEC> \n");
    printf("On peut appliquer plusieurs errosion à la suite avec EE...\n");
    printf("On peut appliquer plusieurs dilatation à la suite avec DD...\n");
    printf("On peut alterner EDEED...\n");
    return 1;
  }

  vector<int> seuils;

  for (int i = 3; i < argc; ++i) {
    seuils.push_back(atoi(argv[i]));
  }

  ImagePGM image_in;
  image_in.load(argv[1]);

  ImagePGM image_out = image_in;

  //E = errosiion et D = dilatation
  string arg_fermeture(argv[3]);

  bool are_vois_8 = false;

  int nb_exec = 1;

  if (argc >= 5)
    are_vois_8 = atoi(argv[4]) == 8 ? true : false;
  
  if (argc >= 6)
    nb_exec = atoi(argv[5]);

  for (int k = 0; k < nb_exec; ++k){
    for (size_t i = 0; i < arg_fermeture.size(); ++i){

      if (arg_fermeture[i] == 'E'){
        fermeture(image_in, image_out, 255, are_vois_8);
      } else if (arg_fermeture[i] == 'D') {
        fermeture(image_in, image_out, 0, are_vois_8);
      } else {
        cerr << "Erreur : l'argument doit etre E ou D\n";
      } 

      image_in = image_out;
    }
  }
  
  image_out.save(argv[2]);

  return 0;
}