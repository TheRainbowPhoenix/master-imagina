#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <math.h>

using namespace std;

#include "Wave.hpp"

Wave::Wave() {            
  //Vérifi que tout est correct pour la taille des types
  checkTypesSize();
  is_data8_allocated  = false; // Tableau non encore alloué
  is_data16_allocated = false; // Tableau non encore alloué
}

Wave::Wave(short* data16,       // Tableau de données
           long int data_nb,    // Nombre de données
           short channels_nb,   // Nombre de canaux (1 pour mono ou 2 pour stéréo)
           int sampling_freq) { // Fréquence d'échantillonnage (en Hertz)(classique en musique : 44100 Hz)

  int i;

  // Vérifi que tout est correct pour la taille des types
  checkTypesSize();

  // Nombre de données
  (*this).data_nb = data_nb;
  // Tableau de donées lorsque l'on est sur des données 16 bits
  (*this).data16 = new short[data_nb];
  for (i=0; i<data_nb; i++) { 
    (*this).data16[i] = data16[i]; //Recopie en profondeur
  }
  is_data8_allocated  = false; // Tableau non alloué
  is_data16_allocated = true;  // Tableau alloué
  
  InitDescriptor(16, channels_nb, sampling_freq);
}

Wave::Wave(unsigned char* data8, // Tableau de donées lorsque l'on est sur des données 8 bits
           long int data_nb,     // Nombre de données
           short channels_nb,    // Nombre de canaux (1 pour mono ou 2 pour stéréo)
           int sampling_freq) {  // Fréquence d'échantillonnage (en Hertz)(classique en musique : 44100 Hz)

  int i;

  // Vérifi que tout est correct pour la taille des types
  checkTypesSize();

  // Nombre de données
  (*this).data_nb = data_nb;

  // Tableau de données lorsque l'on est sur des données 16 bits
  (*this).data8 = new unsigned char[data_nb];
  for (i=0; i<data_nb; i++) { 
    (*this).data8[i] = data8[i]; //Recopie en profondeur
  }
  is_data8_allocated  = true;  // Tableau alloué
  is_data16_allocated = false; // Tableau non alloué
  
  InitDescriptor(8, channels_nb, sampling_freq);
}

void Wave::InitDescriptor(int depth,           // Nombre de bits par donnée (8 ou 16)
                          short channels_nb,   // Nombre de canaux (1 pour mono ou 2 pour stéréo)
                          int sampling_freq) { // Fréquence d'échantillonnage (en Hertz)(classique en musique : 44100 Hz)
           
  // (2 octets) : Nombre de bits par donnée (8 ou 16)
  (*this).depth = depth;

  // (4 octets) : Constante "RIFF" i.e identification du format
  file_type[0] = 'R'; file_type[1] = 'I'; file_type[2] = 'F'; file_type[3] = 'F';
  // (4 octets) : Identifiant "WAVE"
  file_id[0] = 'W'; file_id[1] = 'A'; file_id[2] = 'V'; file_id[3] = 'E';
  // (4 octets) : Identifiant "fmt "
  chunk_id[0] = 'f'; chunk_id[1] = 'm'; chunk_id[2] = 't'; chunk_id[3] = (unsigned char)32;
  // (4 octets) : Constante "data"
  data_id[0] = 'd'; data_id[1] = 'a'; data_id[2] = 't'; data_id[3] = 'a';          

  // (4 octets) : file_size est le nombre d'octet restant à lire (i.e = taille du fichier moins 8 octets)
  file_size = 44 + (depth/8) * data_nb - 8;
  // (4 octets) : Nombre d'octets utilisés pour définir en détail le chunk
  chunk_size = 16;
  // (2 octets) : Format de fichier (1: PCM,  ...)
  format = 1;
  // (2 octets) : Nombre de canaux (1 pour mono ou 2 pour stéréo)
  (*this).channels_nb = channels_nb;
  // (4 octets) : Fréquence d'échantillonnage (en Hertz)
  (*this).sampling_freq = sampling_freq;
  // (4 octets) : Nombre d'octets par seconde de musique
  (*this).bytes_per_second = sampling_freq*channels_nb*depth/8;
  // (2 octets) : Nombre d'octets par échantillon
  (*this).bytes_per_sample = channels_nb*depth/8;
  // (4 octets) : nombre d'octet restant (i.e = taille du fichier moins 44 octets)
  data_size = (depth/8) * data_nb;
}

Wave::~Wave() {            
  if (is_data8_allocated)
    delete[] data8;
  if (is_data16_allocated)
    delete[] data16;
}

void Wave::getData8(unsigned char** data, // Tableau de donées lorsque l'on est sur des données 8 bits
                    int* size) {          // Taille du tableau
   
  int i;            
  if (!is_data8_allocated) { // Tableau non encore alloué
    cout<<"Wave::getData8: Erreur, les donnée ne sont pas présente en 8 bits \n";
    exit(-1);
  }

  // Nombre de données
  (*size) = data_nb;
  
  // Allocation du tableau de données 
  (*data) = new unsigned char[data_nb];
  for (i=0; i<data_nb; i++) { 
    (*data)[i] = data8[i]; //Recopie en profondeur
  }
}

void Wave::modifData8(unsigned char* data) { // Tableau de donées lorsque l'on est sur des données 8 bits
   
  int i;            
  if (!is_data8_allocated) { // Tableau non encore alloué
    cout<<"Wave::setData8: Erreur, le tableau n'est pas alloué\n";
    exit(-1);
  }

  // Remplissage du tableau
  for (i=0; i<data_nb; i++) { 
    data8[i] = data[i];       //Recopie en profondeur
  }
}

void Wave::read(char* fileName) {
  
  int pos = 0;
  int i;
  unsigned char c;
  unsigned char str_tmp[4];
  unsigned char header[44];

  FILE* fd = fopen(fileName, "rb");
  // TEST D'OUVERTURE
  if (fd == NULL) {
    cout<<"Wave::read: Erreur, le fichier "<<fileName<<" n'est pas ouvrable\n";
    exit(-1);
  }
  // TEST DE LECTURE
  if (fread (header, 1, 44, fd) != 44) {
    cout<<"Wave::read: Erreur, impossible de lire dans le fichier "<<fileName<<" les 44 octets d'entete\n";
    exit(-1);
  }
    
  //for (i=0; i<44; i++) {
  //cout<<(int)header[i]<<endl;
  //}

  // file_type
  for (i=0; i<4; i++, pos++) {
    file_type[i] = header[pos]; // Constante "RIFF" i.e identification du format
    cout<<file_type[i];
  }
  if (!((file_type[0] == 'R') && (file_type[1] == 'I') && (file_type[2] == 'F') && (file_type[3] == 'F'))) {
    cout<<"Wave::read: Erreur, le fichier "<<fileName<<" n'est pas un fichier RIFF\n";
    exit(-1);    
  }
  cout<<endl;

  // file_size
  file_size = 0;
  for (i=0; i<4; i++, pos++) {
    //Bit de poids faible en premier
    file_size +=(int)pow(256, i)*(int)header[pos]; //little-endian : increasing numeric significance with increasing memory addresses
  }
  cout<<"Wave::read: Le nombre d'octets restant à lire est : "<<file_size<<endl;
  
  // file_id
  for (i=0; i<4; i++, pos++) {
    file_id[i] = header[pos];         // Identifiant "WAVE"
    cout<<file_id[i];
  }
  cout<<endl;
  if (!((file_id[0] == 'W') && (file_id[1] == 'A') && (file_id[2] == 'V') && (file_id[3] == 'E'))) {
    cout<<"Wave::read: Erreur, le fichier "<<fileName<<" n'est pas un fichier WAVE\n";
    exit(-1);    
  }
  
  // chunk_id
  for (i=0; i<4; i++, pos++) {
    chunk_id[i] = header[pos];         // Identifiant "fmt "
    cout<<chunk_id[i];
  }
  cout<<endl;
  if (!((chunk_id[0] == 'f') && (chunk_id[1] == 'm') && (chunk_id[2] == 't') /*&& (chunk_id[3] == ' ')*/)) {
    cout<<"Wave::read: Erreur, le fichier "<<fileName<<" n'est pas un fichier 'fmt'\n";
    exit(-1);    
  }

  // chunk_size
  chunk_size = 0; // Nombre d'octets utilisés pour définir en détail le chunk
  for (i=0; i<4; i++, pos++) {
    //Bit de poids faible en premier
    chunk_size +=(int)pow(256, i)*(int)header[pos]; //little-endian : increasing numeric significance with increasing memory addresses
  }
  cout<<"Wave::read: chunk_size vaut : "<<chunk_size<<endl;

  // format
  format = 0;     // Format de fichier (1: PCM,  ...)
  for (i=0; i<2; i++, pos++) {
    //Bit de poids faible en premier
    format +=(int)pow(256, i)*(int)header[pos]; //little-endian : increasing numeric significance with increasing memory addresses
  }
  cout<<"Wave::read: Le format est (1=PCM) : "<<format<<endl;

  // format
  channels_nb = 0;     // Nombre de canaux (1 pour mono ou 2 pour stéréo)
  for (i=0; i<2; i++, pos++) {
    //Bit de poids faible en premier
    channels_nb +=(int)pow(256, i)*(int)header[pos]; //little-endian : increasing numeric significance with increasing memory addresses
  }
  cout<<"Wave::read: Le nombre de canaux est : "<<channels_nb<<endl;

  // sampling_freq
  sampling_freq = 0;     // Fréquence d'échantillonnage (en Hertz)
  for (i=0; i<4; i++, pos++) {
    //Bit de poids faible en premier
    sampling_freq +=(int)pow(256, i)*(int)header[pos]; //little-endian : increasing numeric significance with increasing memory addresses
  }
  cout<<"Wave::read: La fréquence d'échantillonge est : "<<sampling_freq<<endl;

  // bytes_per_second
  bytes_per_second = 0;     // Nombre d'octets par seconde de musique
  for (i=0; i<4; i++, pos++) {
    //Bit de poids faible en premier
    bytes_per_second +=(int)pow(256, i)*(int)header[pos]; //little-endian : increasing numeric significance with increasing memory addresses
  }
  cout<<"Wave::read: Le nombre d'octets par seconde est : "<<bytes_per_second<<endl;

  // bytes_per_sample
  bytes_per_sample = 0;     // Nombre d'octets par échantillon
  for (i=0; i<2; i++, pos++) {
    //Bit de poids faible en premier
    bytes_per_sample +=(int)pow(256, i)*(int)header[pos]; //little-endian : increasing numeric significance with increasing memory addresses
  }
  cout<<"Wave::read: Le nombre d'octets par échantillon est : "<<bytes_per_sample<<endl;
  if (bytes_per_sample != (bytes_per_second/sampling_freq)) {
    cout<<"Wave::read: bytes_per_sample != (bytes_per_second/sampling_freq)\n";
    exit(-1);
  }

  // depth
  depth = 0;     // Nombre de bits par donnée (8 ou 16)
  for (i=0; i<2; i++, pos++) {
    //Bit de poids faible en premier
    depth +=(int)pow(256, i)*(int)header[pos]; //little-endian : increasing numeric significance with increasing memory addresses
  }
  cout<<"Wave::read: Le nombre de bits par donnée est : "<<depth<<endl;
  if (depth != (8*bytes_per_sample/channels_nb)) {
    cout<<"Wave::read: depth != (8*bytes_per_sample/channels_nb)\n";
    exit(-1);
  }

  //chunk données
  for (i=0; i<4; i++, pos++) {
    data_id[i] = header[pos]; // Constante "data"
    cout<<data_id[i];
  }
  cout<<endl;
  if (!((data_id[0] == 'd') && (data_id[1] == 'a') && (data_id[2] == 't') && (data_id[3] == 'a'))) {
    cout<<"Wave::read: Erreur, le fichier "<<fileName<<" est corrompu (pas de chaine 'data' dans le header)\n";
    exit(-1);    
  }

  // data_size
  data_size = 0;     // nombre d'octet restant (i.e = taille du fichier moins 44 octets)
  for (i=0; i<4; i++, pos++) {
    //Bit de poids faible en premier
    data_size +=(int)pow(256, i)*(int)header[pos]; //little-endian : increasing numeric significance with increasing memory addresses
  }
  cout<<"Wave::read: Le nombre d'octets restant à lire est : "<<data_size<<endl;
  if (data_size != (file_size-36)) {
    cout<<"Wave::read: data_size != (file_size-36)\n";
    exit(-1);
  }

  //LECTURE DES DONNEES
  switch (depth) {

    case 8:
      data_nb = data_size;
      data8   = new unsigned char[data_nb]; 
      if (fread (data8, 1, data_nb, fd) != data_nb) { //Les donnees sont sur 8 bits
        cout<<"Wave::read: Erreur, impossible de lire dans le fichier "<<fileName<<" le bon nombre d'octet\n";
        exit(-1);
      }
      is_data8_allocated  = true;  // Tableau non alloué
      is_data16_allocated = false; // Tableau alloué
      break;

    case 16:
      data_nb = data_size/2;
      data16  = new short[data_nb];
      if (fread (data16, 2, data_nb, fd) != data_nb) { //Les donnees sont sur 16 bits
        cout<<"Wave::read: Erreur, impossible de lire dans le fichier "<<fileName<<" le bon nombre d'octet\n";
        exit(-1);
      }
      is_data8_allocated  = false; // Tableau non alloué
      is_data16_allocated = true;  // Tableau alloué
      break;

    default:
      cout<<"Wave::read: Erreur, la profondeur (depth = "<<depth<<") est inconnue\n";
      exit(-1);    
  }

  //FERMETURE DU FICHIER
  fclose(fd);      
}    

void Wave::write(char* fileName) {

  cout <<"Wave::write(char* fileName)\n";
  
  int pos = 0;
  int i;
  unsigned char header[44];

  FILE* fd = fopen(fileName, "wb");
  // TEST D'OUVERTURE
  if (fd == NULL) {
    cout<<"Wave::write: Erreur, le fichier "<<fileName<<" n'est pas ouvrable\n";
    exit(-1);
  }
    
  // file_type : constante "RIFF" i.e identification du format
  for (i=0; i<4; i++, pos++) {
    header[pos] = file_type[i]; // Constante "RIFF" i.e identification du format
  }

  // file_size
  cout<<"file_size = "<< file_size <<endl;
  for (i=0; i<4; i++, pos++) {
    //Bit de poids faible en premier
    header[pos] = (( 255 << (8*i) ) & file_size)>>(8*i); //little-endian : increasing numeric significance with increasing memory addresses
//    cout<<"header[pos] = "<<(int)header[pos];
  }
  
  // file_id : identifiant "WAVE"
  for (i=0; i<4; i++, pos++) {
    header[pos] = file_id[i];
  }

  // chunk_id : identifiant "fmt "
  for (i=0; i<4; i++, pos++) {
    header[pos] = chunk_id[i];
  }
  if (!((header[pos-4] == 'f') && (header[pos-3] == 'm') && (header[pos-2] == 't') /*&& (chunk_id[3] == ' ')*/)) {
    cout<<header[pos-4]<<header[pos-3]<<header[pos-2]<<header[pos-1]<<endl;
    cout<<"Wave::write: Erreur, le header est mal remplit pour le chunk_id 'fmt'\n";
    exit(-1);    
  }

  // chunk_size : nombre d'octets utilisés pour définir en détail le chunk
  for (i=0; i<4; i++, pos++) {
    //Bit de poids faible en premier
    header[pos] = (( 255 << (8*i) ) & chunk_size)>>(8*i); //little-endian : increasing numeric significance with increasing memory addresses
  }

  // format : format de fichier (1: PCM,  ...)
  for (i=0; i<2; i++, pos++) {
    //Bit de poids faible en premier
    header[pos] = (( 255 << (8*i) ) & format)>>(8*i); //little-endian : increasing numeric significance with increasing memory addresses
  }

  // channels_nb : Nombre de canaux (1 pour mono ou 2 pour stéréo)
  for (i=0; i<2; i++, pos++) {
    //Bit de poids faible en premier
    header[pos] = (( 255 << (8*i) ) & channels_nb)>>(8*i); //little-endian : increasing numeric significance with increasing memory addresses
  }

  // sampling_freq : fréquence d'échantillonge 
  for (i=0; i<4; i++, pos++) {
    //Bit de poids faible en premier
    header[pos] = (( 255 << (8*i) ) & sampling_freq)>>(8*i); //little-endian : increasing numeric significance with increasing memory addresses
  }

  // bytes_per_second : nombre d'octets par seconde de musique
  for (i=0; i<4; i++, pos++) {
    //Bit de poids faible en premier
    header[pos] = (( 255 << (8*i) ) & bytes_per_second)>>(8*i); //little-endian : increasing numeric significance with increasing memory addresses
  }

  // bytes_per_sample : nombre d'octets par échantillon
  for (i=0; i<2; i++, pos++) {
    //Bit de poids faible en premier
    header[pos] = (( 255 << (8*i) ) & bytes_per_sample)>>(8*i); //little-endian : increasing numeric significance with increasing memory addresses
  }

  // depth : nombre de bits par donnée (8 ou 16)
  for (i=0; i<2; i++, pos++) {
    //Bit de poids faible en premier
    header[pos] = (( 255 << (8*i) ) & depth)>>(8*i); //little-endian : increasing numeric significance with increasing memory addresses
  }

  //chunk données : constante "data"
  for (i=0; i<4; i++, pos++) {
    header[pos] = data_id[i]; // Constante "data"
  }

  // data_size : nombre d'octet restant (i.e = taille du fichier moins 44 octets)
  for (i=0; i<4; i++, pos++) {
    //Bit de poids faible en premier
    header[pos] = (( 255 << (8*i) ) & data_size)>>(8*i); //little-endian : increasing numeric significance with increasing memory addresses
  }

  // ECRITURE DU HEADER
  if (fwrite (header, 1, 44, fd) != 44) { 
    cout<<"Wave::write: Erreur, impossible d'écrire dans le fichier "<<fileName<<" le header\n";
    exit(-1);
  }

  cout<<"Fin écriture du header\n";
  //LECTURE DES DONNEES
  switch (depth) {
    case 8:
      cout<<"fwrite (data8, \n";
      if (fwrite (data8, 1, data_nb, fd) != data_nb) { //Les donnees sont sur 8 bits
        cout<<"Wave::write: Erreur, impossible d'écrire dans le fichier "<<fileName<<" le bon nombre d'octet\n";
        exit(-1);
      }
      break;
    case 16:
      if (fwrite (data16, 2, data_nb, fd) != data_nb) { //Les donnees sont sur 16 bits
        cout<<"Wave::write: Erreur, impossible d'écrire dans le fichier "<<fileName<<" le bon nombre d'octet\n";
        exit(-1);
      }
      break;
    default:
      cout<<"Wave::write: Erreur, la profondeur (depth = "<<depth<<") est inconnue\n";
      exit(-1);    
  }

  cout<<"Fin écriture des datas\n";

  //FERMETURE DU FICHIER
  fclose(fd);      
}    


//void Wave::read(char* fileName) {
//  
//  int i;
//  unsigned char c;
//  unsigned char str_tmp[4];
//  unsigned char header[44];
//
//  FILE* fd = fopen(fileName, "rb");
//  // TEST D'OUVERTURE
//  if (fd == NULL) {
//    cout<<"Wave::read: Erreur, le fichier "<<fileName<<" n'est pas ouvrable\n";
//    exit(-1);
//  }
//  // TEST DE LECTURE
//  if (fread (header, 1, 44, fd) != 44) {
//    cout<<"Wave::read: Erreur, impossible de lire dans le fichier "<<fileName<<" les 44 octets d'entete\n";
//    exit(-1);
//  }
//  
//  
//  for (i=0; i<44; i++) {
//    cout<<(int)header[i]<<endl;
//  }
//
//
//
//  exit(-1);
//  
//       
//  /*FLUX D'ENTREE POUR LE FICHIER*/	
//  ifstream ifs(fileName,ios::in|ios::binary);
//
//  /*TEST D'OUVERTURE*/
//  if (!ifs.is_open()) {
//    cout<<"Wave::read: Erreur, le fichier "<<fileName<<" n'est pas ouvrable\n";
//    exit(-1);
//  }
//  
//  //LECTURE DU HEADER
//  for (i=0; i<4; i++) {
//    ifs>>file_type[i];       // Constante "RIFF" i.e identification du format
//    cout<<file_type[i];
//  }
//  if (!((file_type[0] == 'R') && (file_type[1] == 'I') && (file_type[2] == 'F') && (file_type[3] == 'F'))) {
//    cout<<"Wave::read: Erreur, le fichier "<<fileName<<" n'est pas un fichier RIFF\n";
//    exit(-1);    
//  }
//  cout<<endl;
//  file_size = 0;
//  for (i=0; i<4; i++) {
//    ifs>>str_tmp[i];
//    //Bit de poids faible en premier
//    file_size +=(int)pow(256, i)*(int)str_tmp[i]; //little-endian : increasing numeric significance with increasing memory addresses
//  }
//  cout<<"Wave::read: Le nombre d'octets restant à lire est : "<<file_size<<endl;
//  
//  for (i=0; i<4; i++) {
//    ifs>>file_id[i];         // Identifiant "WAVE"
//    cout<<file_id[i];
//  }
//  cout<<endl;
//  if (!((file_id[0] == 'W') && (file_id[1] == 'A') && (file_id[2] == 'V') && (file_id[3] == 'E'))) {
//    cout<<"Wave::read: Erreur, le fichier "<<fileName<<" n'est pas un fichier WAVE\n";
//    exit(-1);    
//  }
//  for (i=0; i<4; i++) {
//    ifs>>chunk_id[i];         // Identifiant "fmt "
//    cout<<(int)chunk_id[i]<<endl;
//  }
//  cout<<endl;
//  if (!((chunk_id[0] == 'f') && (chunk_id[1] == 'm') && (chunk_id[2] == 't') /*&& (chunk_id[3] == ' ')*/)) {
//    cout<<"Wave::read: Erreur, le fichier "<<fileName<<" n'est pas un fichier 'fmt'\n";
//    exit(-1);    
//  }
//
//  // chunk_size
//  chunk_size = 0; // Nombre d'octets utilisés pour définir en détail le chunk
//  for (i=0; i<4; i++) {
//    ifs>>str_tmp[i];
//    cout<<(int)str_tmp[i]<<endl;
//    //Bit de poids faible en premier
//    chunk_size +=(int)pow(256, i)*(int)str_tmp[i]; //little-endian : increasing numeric significance with increasing memory addresses
//  }
//    cout<<"Wave::read: chunk_size vaut : "<<chunk_size<<endl;
//
//  // format
//  format = 0;     // Format de fichier (1: PCM,  ...)
//  for (i=0; i<2; i++) {
//    ifs>>str_tmp[i];
//    //Bit de poids faible en premier
//    format +=(int)pow(256, i)*(int)str_tmp[i]; //little-endian : increasing numeric significance with increasing memory addresses
//  }
//  cout<<"Wave::read: Le format est (1=PCM) : "<<format<<endl;
//
//
////  ifs>>channels_nb;          // Nombre de canaux (1 pour mono ou 2 pour stéréo)
////  cout<<"Wave::read: Le nombre de canaux est : "<<channels_nb<<endl;
////  ifs>>sampling_freq;        // Fréquence d'échantillonnage (en Hertz)
////  cout<<"Wave::read: La fréquence d'échantillonge est : "<<sampling_freq<<endl;
////  ifs>>bytes_per_second;     // Nombre d'octets par seconde de musique
////  cout<<"Wave::read: Le nombre d'octets par seconde est : "<<bytes_per_second<<endl;
////  ifs>>bytes_per_sample;     // Nombre d'octets par échantillon
////  cout<<"Wave::read: Le nombre d'octets par échantillon est : "<<bytes_per_sample<<endl;
////  ifs>>depth;                // Nombre de bits par donnée (8 ou 16)
////  cout<<"Wave::read: Le nombre de bits par donnée est : "<<depth<<endl;
////*/
////  //chunk données
////  for (i=0; i<4; i++) {
////    ifs>>data_id[i];       // Constante "data"
////    cout<<data_id[i];
////  }
////  if (!((data_id[0] == 'd') && (data_id[1] == 'a') && (data_id[2] == 't') && (data_id[3] == 'a'))) {
////    cout<<"Wave::read: Erreur, le fichier "<<fileName<<" est corrompu (pas de chaine 'data' dans le header)\n";
////    exit(-1);    
////  }
////  ifs>>data_size;       // nombre d'octet restant (i.e = taille du fichier moins 44 octets)
////
////  //LECTURE DES DONNEES
////  switch (depth) {
////    case 8:
////      samples_nb = data_size;
////      data       = new short[samples_nb];
////      char c;
////      for (i=0; i<samples_nb; i++) {
////        ifs>>c; //Lecture de 8 bits
////        data[i] = (short)c;
////      }  
////      break;
////    case 16:
////      samples_nb = data_size/2;
////      data       = new short[samples_nb];
////      for (i=0; i<samples_nb; i++) {
////        ifs>>data[i]; //Lecture de 16 bits
////      }  
////      break;
////    default:
////      cout<<"Wave::read: Erreur, la profondeur (depth = "<<depth<<") est inconnue\n";
////      exit(-1);    
////  }      
//
//  ifs.close();
//}    



//void Wave::read(char* fileName) {
//  
//  int i;
//  char c;
//  char str_tmp[5];
//  str_tmp[4] = '\0';
//       
//  /*FLUX D'ENTREE POUR LE FICHIER*/	
//  FILE* fd = fopen(fileName, "rb");
//
//  /*TEST D'OUVERTURE*/
//  if (fd == NULL) {
//    cout<<"Wave::read: Erreur, le fichier "<<fileName<<" n'est pas ouvrable\n";
//    exit(-1);
//  }
//  //LECTURE DU HEADER
//  for (i=0; i<4; i++) {
//    file_type[i] = (char) fgetc(fd);
//    cout<<file_type[i];
//  }
//  if (!((file_type[0] == 'R') && (file_type[1] == 'I') && (file_type[2] == 'F') && (file_type[3] == 'F'))) {
//    cout<<"Wave::read: Erreur, le fichier "<<fileName<<" n'est pas un fichier RIFF\n";
//    exit(-1);    
//  }
//  cout<<endl;
//  for (i=0; i<4; i++) {
//    file_type[i] = (char) fgetc(fd);
//    fscanf(fd, "%c", str_tmp[i]);
//  }
//  file_size = atoi(str_tmp);  // Nombre d'octet restant à lire (i.e = taille du fichier moins 8 octets)
//  cout<<"Wave::read: nombre d'octet restant à lire = "<<file_size<<endl;
//  for (i=0; i<4; i++) {
//    fscanf(fd, "%c", file_id[i]);// Identifiant "WAVE"
//    cout<<file_id[i];
//  }
//  cout<<endl;
//  if (!((file_id[0] == 'W') && (file_id[1] == 'A') && (file_id[2] == 'V') && (file_id[3] == 'E'))) {
//    cout<<"Wave::read: Erreur, le fichier "<<fileName<<" n'est pas un fichier WAVE\n";
//    exit(-1);    
//  }
//  for (i=0; i<4; i++) {
//    fscanf(fd, "%c", chunk_id[i]);
//    cout<<chunk_id[i];
//  }
//  cout<<endl;
//  if (!((chunk_id[0] == 'f') && (chunk_id[1] == 'm') && (chunk_id[2] == 't') /*&& (chunk_id[3] == ' ')*/)) {
//    cout<<"Wave::read: Erreur, le fichier "<<fileName<<" n'est pas un fichier 'fmt'\n";
//    exit(-1);    
//  }
////  /*TEST*/for (i=0; i<24; i++) {
////  /*TEST*/  ifs>>c;
////  /*TEST*/}
////
/////*  ifs>>chunk_size;           // Nombre d'octets utilisés pour définir en détail le chunk
////  ifs>>format;               // Format de fichier (1: PCM,  ...)
////  cout<<"Wave::read: Le format est (1=PCM) : "<<format<<endl;
////  ifs>>channels_nb;          // Nombre de canaux (1 pour mono ou 2 pour stéréo)
////  cout<<"Wave::read: Le nombre de canaux est : "<<channels_nb<<endl;
////  ifs>>sampling_freq;        // Fréquence d'échantillonnage (en Hertz)
////  cout<<"Wave::read: La fréquence d'échantillonge est : "<<sampling_freq<<endl;
////  ifs>>bytes_per_second;     // Nombre d'octets par seconde de musique
////  cout<<"Wave::read: Le nombre d'octets par seconde est : "<<bytes_per_second<<endl;
////  ifs>>bytes_per_sample;     // Nombre d'octets par échantillon
////  cout<<"Wave::read: Le nombre d'octets par échantillon est : "<<bytes_per_sample<<endl;
////  ifs>>depth;                // Nombre de bits par donnée (8 ou 16)
////  cout<<"Wave::read: Le nombre de bits par donnée est : "<<depth<<endl;
////*/
////  //chunk données
////  for (i=0; i<4; i++) {
////    ifs>>data_id[i];       // Constante "data"
////    cout<<data_id[i];
////  }
////  if (!((data_id[0] == 'd') && (data_id[1] == 'a') && (data_id[2] == 't') && (data_id[3] == 'a'))) {
////    cout<<"Wave::read: Erreur, le fichier "<<fileName<<" est corrompu (pas de chaine 'data' dans le header)\n";
////    exit(-1);    
////  }
////  ifs>>data_size;       // nombre d'octet restant (i.e = taille du fichier moins 44 octets)
////
////  //LECTURE DES DONNEES
////  switch (depth) {
////    case 8:
////      samples_nb = data_size;
////      data       = new short[samples_nb];
////      char c;
////      for (i=0; i<samples_nb; i++) {
////        ifs>>c; //Lecture de 8 bits
////        data[i] = (short)c;
////      }  
////      break;
////    case 16:
////      samples_nb = data_size/2;
////      data       = new short[samples_nb];
////      for (i=0; i<samples_nb; i++) {
////        ifs>>data[i]; //Lecture de 16 bits
////      }  
////      break;
////    default:
////      cout<<"Wave::read: Erreur, la profondeur (depth = "<<depth<<") est inconnue\n";
////      exit(-1);    
////  }      
////
////  ifs.close();
//  fclose(fd);
//}    


void Wave::checkTypesSize() {
  if (sizeof(char) != 1) {
    cout<<"Wave::checkTypesSize(): Erreur : le type char n'est pas de taille 8 bits\n";
    exit(-1);
  }
  if (sizeof(short) != 2) {
    cout<<"Wave::checkTypesSize(): Erreur : le type short n'est pas de taille 16 bits\n";
    cout<<"Wave::checkTypesSize(): Erreur : le type short est pas de taille "<< 8*sizeof(int)<<" bits\n";
    exit(-1);
  }
  //cout<<"Le type int est de taille "<<sizeof(int)<<endl;
  if (sizeof(int) != 4) {
    cout<<"Wave::checkTypesSize(): Erreur : le type int n'est pas de taille 32 bits\n";
    cout<<"Wave::checkTypesSize(): Erreur : le type int est pas de taille "<< 8*sizeof(long int)<<" bits\n";
    exit(-1);
  }
}
