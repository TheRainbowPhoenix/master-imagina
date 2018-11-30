#include <iostream>
#include <cmath>
#include "Wave.hpp"

using namespace std;

// signal in [-1,-1] -> [0, 255]
unsigned char normalize(double signal){
	return (signal + 1.0) * 127.5; 
}

int create_note(unsigned char* data, long int data_size, double freq_note, double freq_ech){

	for (int i = 0; i < data_size; ++i){
		double Si = sin(2 * M_PI * freq_note * ((i * 1.0) / freq_ech));
		data[i] = normalize(Si);
	}

	return 1;    
}

int write_note(unsigned char* data, long int data_size, double time_start, double duration, double freq_note, double freq_ech){
	
	int k = 0;
	float offset = time_start * freq_ech;
	float max = offset + (duration * freq_ech);
		
	for (int i = offset; i < max; ++i){

		if(i >= data_size){
			cout << "Impossible d'ecrire la note en entire, data trop petit" << endl;
			return -1;
		}

		double Si = sin(2 * M_PI * freq_note * ((k * 1.0) / freq_ech));
		data[i] = normalize(Si);
	
		k++;
	}

	return 1;
}

int main(int argc, const char *argv[]) {
	
	char* output = (char*)"la";
	
	cout << "Creation des données...\n";

	double sample_time = 10;
	int sampling_freq = 44100;
	int channels_nb = 1;

	long int data_size = sampling_freq * sample_time;
	unsigned char* data = new unsigned char[data_size];

	for (int i = 0; i < sample_time; ++i){
		write_note(data, data_size, i, 1, i * 200, sampling_freq);
	}

	cout << "Creation du fichier wav...\n";
	
	Wave wave(data, data_size, channels_nb, sampling_freq);

	cout << "Ecriture du fichier wav dans " << output << "\n";

	wave.write(output);

	return 0;
}
/*
S(t) = sin(2 * PI * freq * t)
S(n) = S(n * tau) = sin(2 * Pi * freq * n * tau) = sin(2 * PI * freq * 1 / (freq * tau) * n
*/