#include <stdio.h>
#include <iostream>
#include <vector>
#include <regex>
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

/* ajoute les seuils dans le vector seuils et renvoie l'index ou ce trouve la prochaine couleur sur laquel ajouter des seuils */
int add_seuils(int argc, const char* argv[], int index, vector<int>& seuils){
  
  while (index < argc && argv[index][0] != 'R' && argv[index][0] != 'G' && argv[index][0] != 'B') {
    seuils.push_back(atoi(argv[index]));
    index++;
  }

  return index;
}

void print_v(vector<int>& v){
  for (size_t i = 0; i < v.size(); ++i) {
    cout << v[i] << " ";
  }
  cout << "\n";
}

void set_plan_R(unsigned char *image_data, unsigned char *plan_R, int taille_plan){
  int i;
  for (i=0; i<taille_plan; i++){
    image_data[3*i] = plan_R[i];
  }
}
   
void set_plan_G(unsigned char *image_data, unsigned char *plan_G, int taille_plan){
  int i;
  for (i=0; i<taille_plan; i++){
    image_data[3*i+1] = plan_G[i];
  }
}

void set_plan_B(unsigned char *image_data, unsigned char *plan_B, int taille_plan){
  int i;
  for (i=0; i<taille_plan; i++){
    image_data[3*i+2] = plan_B[i];
  }
}

int main(int argc, const char *argv[]) {

  if (argc <= 3) {
    printf("Usage: <ImageIn.ppm> <ImageOut.ppm> [R <S1 S2 ... SN>] [G <S1 S2 ... SN>] [B <S1 S2 ... SN>]\n");
    printf("Attention a faire en sorte que SN < SN+1.\n");
    return 1;
  }

  ImageBase image_in; image_in.load(argv[1]);
  ImageBase image_out(image_in.getWidth(), image_in.getHeight(), image_in.getColor());

  vector<int> R_seuils, G_seuils, B_seuils;

  int i = 3;

  while (i < argc) {

      if (argv[i][0] == 'R') {
        i++;
        i = add_seuils(argc, argv, i, R_seuils);
      }
      else if (argv[i][0] == 'G') {
        i++;
        i = add_seuils(argc, argv, i, G_seuils);
      }
      else if (argv[i][0] == 'B') {
        i++;
        i = add_seuils(argc, argv, i, B_seuils);
      }
      else {
        cerr << "Erreur : sur le format d'entrée des données\n";
        return 1;
      }
  }

  cout << "R_seuils : "; print_v(R_seuils);
  cout << "G_seuils : "; print_v(G_seuils);
  cout << "B_seuils : "; print_v(B_seuils);

/* Seuillage des rouges */

  ImageBase* R_in = image_in.getPlan(ImageBase::PLAN_R);
  ImageBase* R_out = image_out.getPlan(ImageBase::PLAN_R);
  seuillage(R_in, R_out, R_seuils);

/* Seuillage des verts */

  ImageBase* G_in = image_in.getPlan(ImageBase::PLAN_G);
  ImageBase* G_out = image_out.getPlan(ImageBase::PLAN_G);
  seuillage(G_in, G_out, G_seuils);

/* Seuillage des bleues */  

  ImageBase* B_in = image_in.getPlan(ImageBase::PLAN_B);
  ImageBase* B_out = image_out.getPlan(ImageBase::PLAN_B);
  seuillage(B_in, B_out, B_seuils);

  set_plan_R(image_out.getData(), R_out->getData(), R_out->getWidth() * R_out->getHeight());
  set_plan_G(image_out.getData(), G_out->getData(), G_out->getWidth() * G_out->getHeight());
  set_plan_B(image_out.getData(), B_out->getData(), B_out->getWidth() * B_out->getHeight());

  image_out.save(argv[2]);

  return 0;
}
