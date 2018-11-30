#ifndef WAVE_HPP
#define WAVE_HPP

class Wave {


private :
  //chunk type
  char file_type[4];         // (4 octets) : Constante "RIFF" i.e identification du format
  int file_size;             // (4 octets) : file_size est le nombre d'octet restant à lire (i.e = taille du fichier moins 8 octets)
  char file_id[4];           // (4 octets) : Identifiant "WAVE"
  //chunk format
  char chunk_id[4];          // (4 octets) : Identifiant "fmt "
  int chunk_size;            // (4 octets) : Nombre d'octets utilisés pour définir en détail le chunk
  short format;              // (2 octets) : Format de fichier (1: PCM,  ...)
  short channels_nb;         // (2 octets) : Nombre de canaux (1 pour mono ou 2 pour stéréo)
  int sampling_freq;         // (4 octets) : Fréquence d'échantillonnage (en Hertz)
  int bytes_per_second;      // (4 octets) : Nombre d'octets par seconde de musique
  short bytes_per_sample;    // (2 octets) : Nombre d'octets par échantillon
  short depth;               // (2 octets) : Nombre de bits par donnée (8 ou 16)
  //chunk données
  char data_id[4];          // (4 octets) : Constante "data"
  int data_size;            // (4 octets) : nombre d'octet restant (i.e = taille du fichier moins 44 octets)
  //data
  unsigned char* data8;     // Tableau de donées lorsque l'on est sur des données 8 bits
  short* data16;            // Tableau de donées lorsque l'on est sur des données 16 bits
  long int data_nb;         // Nombre de données

  bool is_data8_allocated;  // Indique si le tableau à été alloué
  bool is_data16_allocated; // Indique si le tableau à été alloué
  
  static void checkTypesSize(); // Test la taille des types

  void InitDescriptor(int depth,          // Nombre de bits par donnée (8 ou 16)
                      short channels_nb,  // Nombre de canaux (1 pour mono ou 2 pour stéréo)
                      int sampling_freq); // Fréquence d'échantillonnage (en Hertz)(classique en musique : 44100 Hz)

public:
       
  //Méthode permettant de récupérer les données (cad le signal sur 8 bits)     
  void getData8(unsigned char** data, // pointeur sur le tableau de donées lorsque l'on est sur des données 8 bits
                int* size);           // pointeur sur la taille du tableau 
  //Méthode permettant de modifier les données
  void modifData8(unsigned char* data); // Tableau de donées lorsque l'on est sur des données 8 bits

  void read(char* fileName);   //Lecture d'un fichier wave
  void write(char* fileName);
       
  Wave();
  ~Wave();
  Wave(short* data16,      // Tableau de données
       long int data_nb,   // Nombre de données
       short channels_nb,  // Nombre de canaux (1 pour mono ou 2 pour stéréo)
       int sampling_freq); // Fréquence d'échantillonnage (en Hertz)(classique en musique : 44100 Hz)
  Wave(unsigned char* data8, // Tableau de donées lorsque l'on est sur des données 8 bits
       long int data_nb,     // Nombre de données
       short channels_nb,    // Nombre de canaux (1 pour mono ou 2 pour stéréo)
       int sampling_freq);   // Fréquence d'échantillonnage (en Hertz)(classique en musique : 44100 Hz)
};
#endif //IO_WAVE_HPP
