#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include "Wave.hpp"

using namespace std;

void add_note(vector<double>& signal, double freq_note, double duration, double freq_ech, double amplitude = 1.0);

// signal in [-1,-1] -> [0, 255]
unsigned char normalize(double signal_ech);

vector<unsigned char> to_data8(vector<double>& signal);

void write_signal(string& filepath, vector<double>& signal, int nb_channels, int sampling_freq);

void DFT(vector<double>& signal, vector<double>& signal_imag, vector<double>& signal_reel);

void IDFT(vector<double>& signal, vector<double>& signal_imag, vector<double>& signal_reel);

/*
	This FFT has been proposed by Paul Bourke 
	http://paulbourke.net/miscellaneous/dft/
	This computes an in-place complex-to-complex FFT 
	x and y are the real and imaginary arrays of 2^m points.
	dir =  1 gives forward transform
	dir = -1 gives reverse transform 
	You MUST compute first the value m such that
	2^(m-1) < n (size of your signal) <= 2^m
	allocate a new signal of nm=2^m values
	then fill the n first values of this new signal 
	with your signal and fill the rest with 0
	WARNING : you must pass m, not nm !!!
*/
int FFT (int dir, int m, double* x, double* y);

vector<double> FFT_visualize(vector<double>& signal_real, vector<double>& signal_imaginary);

void normalisation(double* signal, int N);


/* Renvoie m tel que 2^m >= n  */
int next_pow2(int n) {

	int next_pow2 = 1;
	int m = 0;

	while (next_pow2 < n) {
		next_pow2 *= 2;
		m++;
	}

	return m;
}

int main(int argc, const char *argv[]) {
		
	cout << "Initialisation du signal...\n";

	double sample_time = 10; // en secondes
	string filepath = "sounds/la.wav";
	int sampling_freq = 44100;
	int nb_channels = 1;

	vector<double> signal;

	double la = 440.0;

	add_note(signal, la, sample_time, sampling_freq);

	/* les signaux doivent avoir une taille en puissance de deux pour que la FFT
	   puisse fonctionnée convenablement */


	int FFT_m = next_pow2(signal.size());
	long long FFT_size = pow(2, FFT_m);

	cout << FFT_m << " " << FFT_size << "\n";

	vector<double> signal_real(FFT_size);
	vector<double> signal_imaginary(FFT_size);

	copy(signal.begin(), signal.end(), signal_real.begin());

	cout << "Transformation de fourrier en cours..." << endl;

	FFT(1, FFT_m, signal_real.data(), signal_imaginary.data());

	/* Dans la transformation de fourrier chaque case du tableau represente une fréquence */ 

	FFT(-1, FFT_m, signal_real.data(), signal_imaginary.data());

	cout << "Ecriture du signal dans " << filepath << "\n"; 
	
	copy(signal_real.begin(), signal_real.begin() + signal.size(), signal.begin());

	normalisation(signal.data(), signal.size());

	write_signal(filepath, signal, nb_channels, sampling_freq);

	return 0;
}

/*
S(t) = sin(2 * PI * freq * t)
S(n) = S(n * tau) = sin(2 * Pi * freq * n * tau) = sin(2 * PI * freq * 1 / (freq * tau) * n
*/

void add_note(vector<double>& signal, double freq_note, double duration, double freq_ech, double amplitude){

	long long nb_ech = (long long)(duration * freq_ech);
		
	for (long long i = 0; i < nb_ech; ++i)
		signal.push_back(amplitude * sin(2 * M_PI * freq_note * (i / freq_ech)));
}

// signal in [-1,-1] -> [0, 255]
unsigned char normalize(double signal_ech){
	return (signal_ech + 1.0) * 127.5; 
}

vector<unsigned char> to_data8(vector<double>& signal) {

	vector<unsigned char> data8(signal.size());

	for (size_t i = 0; i < signal.size(); ++i)
		data8[i] = normalize(signal[i]);

	return move(data8);
}

void write_signal(string& filepath, vector<double>& signal, int nb_channels, int sampling_freq) {

	vector<unsigned char> data8(to_data8(signal));
	
	Wave wave(data8.data(), data8.size(), nb_channels, sampling_freq);

	// convertie en char* car la fonction write de modifie pas la constante de toute façon
	wave.write((char*)filepath.c_str());
}

void DFT(vector<double>& signal, vector<double>& signal_imag, vector<double>& signal_reel) {

}

void IDFT(vector<double>& signal, vector<double>& signal_imag, vector<double>& signal_reel) {

}

int FFT(int dir, int m, double *x, double *y) {

	int n,i,i1,j,k,i2,l,l1,l2;
	double c1,c2,tx,ty,t1,t2,u1,u2,z;
	
	/* Calculate the number of points */

	n = 1;

	for (i = 0 ; i < m ; i++) 
		n *= 2;
	
	/* Do the bit reversal */

	j = 0;
	i2 = n >> 1;
	
	for (i = 0 ; i < n - 1 ; i++) {
		
		if (i < j) {
			tx = x[i];
			ty = y[i];
			x[i] = x[j];
			y[i] = y[j];
			x[j] = tx;
			y[j] = ty;
		}

		k = i2;
		
		while (k <= j) {
			j -= k;
			k >>= 1;
		}
		
		j += k;
	}
	
	/* Compute the FFT */

	c1 = -1.0; 
	c2 = 0.0;
	l2 = 1;
	
	for (l = 0 ; l < m ; l++) {
		
		l1 = l2;
		l2 <<= 1;
		u1 = 1.0; 
		u2 = 0.0;
		
		for (j = 0 ; j < l1 ; j++) {
		
			for (i = j ; i < n ; i += l2) {
				i1 = i + l1;
				t1 = u1 * x[i1] - u2 * y[i1];
				t2 = u1 * y[i1] + u2 * x[i1];
				x[i1] = x[i] - t1; 
				y[i1] = y[i] - t2;
				x[i] += t1;
				y[i] += t2;
			}
		
			z =  u1 * c1 - u2 * c2;
			u2 = u1 * c2 + u2 * c1;
			u1 = z;
		}
		
		c2 = sqrt((1.0 - c1) / 2.0);
		
		if (dir == 1) 
			c2 = -c2;

		c1 = sqrt((1.0 + c1) / 2.0);
	}
	
	/* Scaling for forward transform */

	if (dir == 1) {
		for (i = 0 ; i < n ; i++) {
			x[i] /= n;
			y[i] /= n;
		}
	}

	return 1;
}

vector<double> FFT_visualize(vector<double>& signal_real, vector<double>& signal_imaginary) {
	
	double max = 0;
	vector<double> norme(signal_real.size());

	for (size_t i = 0; i < signal_real.size(); ++i) {
		norme[i] = sqrt(signal_real[i] * signal_real[i] + signal_imaginary[i] * signal_imaginary[i]);
		max = max < norme[i] ? norme[i] : max;
	}

	/* normalize val -> [-1, 1] */

	for (size_t i = 0; i < signal_real.size(); ++i) {
		norme[i] = (2.0 * norme[i] / max) - 1.0;
	}

	return move(norme);
}

void normalisation(double* signal, int N) {

	double max, min;
	double *pt, *fin;

	pt = signal;
	fin = pt + N;
	min = (*pt);
	max = (*pt);

	while (pt < fin) {
		min = (*pt) < min ? (*pt) : min;
		max = (*pt) > max ? (*pt) : max;
		pt++;
	}

	if(max > min + 1e-8) 
		max = 2.0 / (max - min);
	else
		max = 1e-8;

	pt = signal;

	while (pt < fin) {
		(*pt) = (((*pt) - min) * max) - 1.0;
		pt++;
	}
}

/* 
pas recopier sujet tp
pas donner de code
pas copier wikipedia

sitation bienvenue

vue outils

filtres
transformé
comment créé sons
filtrer sons

constater ce qui ce passe sur des sons créé

voir filtre sur sont créée
voir filtre sur vrai sont


rendre constation sur ce qu'on a fait
rendre difficulté a avoir

rendu après vacances en pdf.

minuit = deadline

pas plus de 15 pages (avec graphics et tout)
*/