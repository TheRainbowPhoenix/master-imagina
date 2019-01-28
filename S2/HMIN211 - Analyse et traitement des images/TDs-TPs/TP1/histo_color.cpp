#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>
#include "ImageBase.h"

using namespace std;

int main(int argc, char **argv) {

  if (argc != 2) {
    cerr << "Usage : <STRING:image.ppm>\n";
    return 1;
  }

  ImageBase image_in;
  image_in.load(argv[1]);

  map<int, array<int, 3> > RGB_occurrences;

  ImageBase* R = image_in.getPlan(ImageBase::PLAN_R);

  for (int x = 0; x < image_in.getHeight(); ++x){
    for (int y = 0; y < image_in.getWidth(); ++y){
      RGB_occurrences[(*R)[x][y]][0]++;
    }
  }

  ImageBase* G = image_in.getPlan(ImageBase::PLAN_G);

  for (int x = 0; x < image_in.getHeight(); ++x){
    for (int y = 0; y < image_in.getWidth(); ++y){
      RGB_occurrences[(*G)[x][y]][1]++;
    }
  }

  ImageBase* B = image_in.getPlan(ImageBase::PLAN_B);

  for (int x = 0; x < image_in.getHeight(); ++x){
    for (int y = 0; y < image_in.getWidth(); ++y){
      RGB_occurrences[(*B)[x][y]][2]++;
    }
  }

  double nb_pixels = image_in.getHeight() * image_in.getWidth();

  for (auto it = RGB_occurrences.begin() ; it != RGB_occurrences.end() ; ++it){
    cout << it->first << " " << it->second[0] / nb_pixels << " " << it->second[1] / nb_pixels << " " << it->second[2] / nb_pixels << "\n";
  }

  return 0;
}
