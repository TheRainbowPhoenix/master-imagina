#include "ImageBase.h"

////////////////////////////////// ImageBase ////////////////////////////////////////

ImageBase::ImageBase() : m_data(), m_height(0), m_width(0), m_is_valid(false) {}

ImageBase::ImageBase(int width, int height) : ImageBase(width, height, width * height)  {}

ImageBase::ImageBase(int width, int height, size_t data_size) : m_data(data_size), m_height(height), m_width(width) {
	
	if(m_data.size() == 0) {
		std::cout << "Attention : la taille de l'image est nulle\n";
		m_is_valid = false;
	} else {
		m_is_valid = true;
	}
}

OCTET* ImageBase::operator[](int raw) {
	
	if(!is_valid()) {
		printf("L'image courante n'est pas valide");
		exit(0);
	}
	
	if(raw >= height()) {
		printf("L'indice se trouve en dehors des limites de l'image");
		exit(0);
	}
	
	return this->data() + raw * this->width();
}

const OCTET* ImageBase::operator[](int raw) const{
	return (*this)[raw];
}

////////////////////////////////// ImagePGM ////////////////////////////////////////

bool ImagePGM::load(const std::string& filename){

	this->m_data.resize(0);
	this->m_height = 0;
	this->m_width = 0;
	this->m_is_valid = false;

	size_t l = filename.size();

	// Le fichier ne peut pas etre que ".pgm" ou ".ppm"

	if(l <= 4) {
		printf("Chargement de l'image impossible : Le nom de fichier n'est pas conforme, il doit comporter l'extension, et celle ci ne peut être que '.pgm'");
		return true;
	}

	if(strcmp(filename.c_str() + l - 3, "pgm") == 0) {

		lire_nb_lignes_colonnes_image_pgm(filename.c_str(), &this->m_height, &this->m_width);
		
		this->m_data.resize(this->m_height * this->m_width);
		
		lire_image_pgm(filename.c_str(), this->m_data.data(), this->m_data.size());

	} else {
		printf("Chargement de l'image impossible : Le nom de fichier n'est pas conforme, il doit comporter l'extension, et celle ci ne peut être que .pgm");
		return false;
	}
	
	m_is_valid = true;

	return true;
}

bool ImagePGM::save(const std::string& filename){
	
	if(!this->is_valid())
		return false;

	ecrire_image_pgm(filename.c_str(), this->data(),  this->height(), this->width());

	return true;
}

////////////////////////////////// ImagePPM ////////////////////////////////////////

ImagePPM::ImagePPM(int width, int height) : ImageBase(width, height, width * height * 3) {}

ImagePPM::ImagePPM(const ImagePGM& red, const ImagePGM& green, const ImagePGM& blue) {
	
	if ( (red.width() == green.width() && red.width() == blue.width()) && (red.height() == green.height() && red.height() == blue.height()) ) {
		ImagePPM(red.width(), red.height());
		set_plan(PLAN_R, red);
		set_plan(PLAN_G, green);
		set_plan(PLAN_B, blue);
	} else {
		std::cout << "Attention : les images ne sont pas au meme format\n"; 
	}
}

bool ImagePPM::load(const std::string& filename){

	this->m_data.resize(0);
	this->m_height = 0;
	this->m_width = 0;
	this->m_is_valid = false;
	
	size_t l = filename.size();

	// Le fichier ne peut pas etre que ".ppm"

	if(l <= 4) {
		printf("Chargement de l'image impossible : Le nom de fichier n'est pas conforme, il doit comporter l'extension, et celle ci ne peut être que '.pgm' ou '.ppm'");
		return false;
	}

	size_t nb_pixel = 0;

	if(strcmp(filename.c_str() + l - 3, "ppm") == 0) {

		lire_nb_lignes_colonnes_image_ppm(filename.c_str(), &this->m_height, &this->m_width);
		
		nb_pixel = this->m_height * this->m_width;
  		this->m_data.resize(nb_pixel * 3); 

		lire_image_ppm(filename.c_str(), this->m_data.data(), nb_pixel);

	} else {
		printf("Chargement de l'image impossible : Le nom de fichier n'est pas conforme, il doit comporter l'extension, et celle ci ne peut être que .ppm");
		return false;
	}
	
	m_is_valid = true;

	return true;
}

bool ImagePPM::save(const std::string& filename){
	
	if(!is_valid())
		return false;

	ecrire_image_ppm(filename.c_str(), this->data(),  this->height(), this->width());

	return true;
}

ImagePGM ImagePPM::get_plan(PLAN plan) {
	
	ImagePGM grey(width(), height());
	
	switch(plan) {

	case PLAN_R:
		planR(grey.data(), this->data(), this->height() * this->width());
		break;
	case PLAN_G:
		planV(grey.data(), this->data(), this->height() * this->width());
		break;
	case PLAN_B:
		planB(grey.data(), this->data(), this->height() * this->width());
		break;
	default:
		printf("Il n'y a que 3 plans, les valeurs possibles ne sont donc que 'PLAN_R', 'PLAN_G', et 'PLAN_B'");
		exit(0);
		break;
	}

	return std::move(grey);
}

void ImagePPM::set_plan(PLAN plan, const ImagePGM& grey_image) {

	if (grey_image.width() != this->width() || grey_image.height() != this->height()) {
		std::cout << "Attention : les images ne sont pas au meme format\n";
		return;
	}

	switch(plan) {

	case PLAN_R:
		set_plan_R(this->data(), grey_image.data(), this->height() * this->width());
		break;
	case PLAN_G:
		set_plan_G(this->data(), grey_image.data(), this->height() * this->width());
		break;
	case PLAN_B:
		set_plan_B(this->data(), grey_image.data(), this->height() * this->width());
		break;
	default:
		printf("Il n'y a que 3 plans, les valeurs possibles ne sont donc que 'PLAN_R', 'PLAN_G', et 'PLAN_B'");
		exit(0);
		break;
	}
}

////////////////////////////////// FONCTIONS ////////////////////////////////////////

void set_plan_R(OCTET* image_data, const OCTET* plan_R, int taille_plan){
  int i;
  for (i=0; i<taille_plan; i++){
    image_data[3*i] = plan_R[i];
  }
}

void set_plan_G(OCTET* image_data, const OCTET* plan_G, int taille_plan){
  int i;
  for (i=0; i<taille_plan; i++){
    image_data[3*i+1] = plan_G[i];
  }
}

void set_plan_B(OCTET* image_data, const OCTET* plan_B, int taille_plan){
  int i;
  for (i=0; i<taille_plan; i++){
    image_data[3*i+2] = plan_B[i];
  }
}

void seuillage(ImagePGM& image, const std::vector<int>& seuils){
  
  bool assigned = false;
  
  if(seuils.size() > 0) {

    for (int i = 0; i < image.height(); ++i){
      for (int j = 0; j < image.width(); ++j){
        
        assigned = false;

        for (size_t k = 0; k < seuils.size(); ++k){
          if (image[i][j] < seuils[k]){
            image[i][j] = (255 / seuils.size()) * k ;
            assigned = true;
            break;
          }
        }

        if (!assigned)
          image[i][j] = 255;

      }
    }

  }
}

std::vector<int> histo_grey(const ImagePGM& image) {

	std::vector<int> grey_occurrences(256);

	for (int i = 0; i < image.height(); ++i){
		for (int j = 0; j < image.width(); ++j){
			grey_occurrences[image[i][j]]++;
		}
	}

	return std::move(grey_occurrences);
/*
	for (auto it = grey_occurrences.begin() ; it != grey_occurrences.end() ; ++it){
		std::cout << it->first << " " << it->second / (double)(image.height() * image.width()) << "\n";
	}
*/
}

std::vector<std::array<int, 3> > histo_color(const ImagePPM& image) {

  std::vector<std::array<int, 3> > RGB_occurrences(256);

  ImagePGM R = image.get_plan(ImagePPM::PLAN_R);
  ImagePGM G = image.get_plan(ImagePPM::PLAN_G);
  ImagePGM B = image.get_plan(ImagePPM::PLAN_B);

  for (int i = 0; i < image.height(); ++i){
    for (int j = 0; j < image.width(); ++j){
      RGB_occurrences[R[i][j]][0]++;
      RGB_occurrences[G[i][j]][1]++;
      RGB_occurrences[B[i][j]][2]++;
    }
  }

  return std::move(RGB_occurrences);

  //double nb_pixels = image.height() * image.width();
/*
  for (auto it = RGB_occurrences.begin() ; it != RGB_occurrences.end() ; ++it){
    std::cout << it->first << " " << it->second[0] / nb_pixels << " " << it->second[1] / nb_pixels << " " << it->second[2] / nb_pixels << "\n";
  }
*/
}

void print_histo_grey(std::vector<int> histo_grey) {

	//double nb_pixels = image.height() * image.width();

	for (size_t i = 0 ; i < histo_grey.size() ; ++i){
		std::cout << i << " " << histo_grey[i] << "\n";
	}
}

void print_histo_color(std::vector<std::array<int, 3> > histo_color) {

	//double nb_pixels = image.height() * image.width();

	for (size_t i = 0 ; i < histo_color.size() ; ++i){
		std::cout << i << " " << histo_color[i][0] << " " << histo_color[i][1] << " " << histo_color[i][2] << "\n";
	}
}

