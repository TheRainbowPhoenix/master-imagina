#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>
#include "ImageBase.h"

using namespace std;

int main(int argc, char **argv) {

  if (argc != 2) {
    cerr << "Usage : <STRING:image.pgm>\n";
    return 1;
  }

  ImageBase image_in;
  image_in.load(argv[1]);

  map<int, int> grey_occurrences;

  for (int x = 0; x < image_in.getHeight(); ++x){
    for (int y = 0; y < image_in.getWidth(); ++y){
      grey_occurrences[image_in[x][y]]++;
    }
  }

  for (auto it = grey_occurrences.begin() ; it != grey_occurrences.end() ; ++it){
    cout << it->first << " " << it->second / (double)(image_in.getHeight() * image_in.getWidth()) << "\n";
  }

  return 0;
}
