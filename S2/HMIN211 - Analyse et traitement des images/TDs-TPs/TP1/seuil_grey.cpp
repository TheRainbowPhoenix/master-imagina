#include <stdio.h>
#include <iostream>
#include <vector>
#include "ImageBase.h"

using namespace std;

void seuillage(ImageBase* image_in, ImageBase* image_out, const vector<int>& seuils){
  
  bool assigned = false;
  
  if(seuils.size() > 0) {

    for (int x = 0; x < image_in->getHeight(); ++x){
      for (int y = 0; y < image_in->getWidth(); ++y){
        
        assigned = false;

        for (size_t i = 0; i < seuils.size(); ++i){
          if ((*image_in)[x][y] < seuils[i]){
            (*image_out)[x][y] = (255 / seuils.size()) * i ;
            assigned = true;
            break;
          }
        }

        if (!assigned)
          (*image_out)[x][y] = 255;

      }
    }

  }
}

int main(int argc, char **argv) {

  if (argc <= 3) {
    printf("Usage: <ImageIn.pgm> <ImageOut.pgm> <S1 S2 ... SN>\n");
    printf("Attention a faire en sorte que SN < SN+1.\n");
    return 1;
  }

  vector<int> seuils;
  
  for (int i = 3; i < argc; ++i) {
      seuils.push_back(atoi(argv[i]));
  }

  ImageBase image_in; image_in.load(argv[1]);
  ImageBase image_out(image_in.getWidth(), image_in.getHeight(), image_in.getColor());

  seuillage(&image_in, &image_out, seuils);

  image_out.save(argv[2]);

  return 0;
}
