/*
 *  FirstProgram.cpp
 *  
 *
 *  Created by Olivier Strauss on 10/10/16.
 *  Copyright 2016 LIRMM. All rights reserved.
 *
 */

#include "CImg.h"
#include "string.h"

using namespace cimg_library;

// Calcul de la multiplication C = A.B au sens des matrices.
// C est NlinxNcol, A est NlinxNcom, B est NcomxNcol
// Le pointeurs doivent etre alloues correctement avant l'appel
int MatMult(double *A, double *B, double *C, int Nlin, int Ncom, int Ncol)
{
	int lin, col, k ;
	double *ptA, *ptB, *ptC ;
	
	ptC = C ;
	for(lin = 0 ; lin<Nlin ; lin++)
	{
		for(col = 0 ; col<Ncol ; col++, ptC++)
		{
			ptA = A + (lin*Ncom) ;
			ptB = B + col ;
			(*ptC) = 0.0 ;
			for(k = 0 ; k<Ncom ; k++)
			{
				(*ptC) += (*ptA) * (*ptB) ;
				ptA++ ; 
				ptB+= Ncol ; 
			}
		}
	}
	return 1 ;
}

// Donne des 2 points d'intersection entre la droite
// de parametre L et le carre de dimension Dx, Dy
int Intersection( double L[3], int Dx, int Dy, int x_inter[2], int y_inter[2] )
{
	double a=L[0], b=L[1], c=L[2] ;
	double x[4], y[4] ;
	int nb_points_ok = 0, n ;
	x[0] = 0 ;
	x[1] = Dx-1 ;
	y[2] = 0 ;
	y[3] = Dy-1 ;
	
	if(fabs(L[0])>1e-16)
	{ // droite de la forme x = b'y + c' ;
		b = -L[1]/L[0] ;
		c = -L[2]/L[0] ;
		x[2] = b * y[2] + c ;
		x[3] = b * y[3] + c ;
	}
	else 
	{
		x[2] = -Dx ;
		x[3] = -Dx ;
	}

	if(fabs(L[1])>1e-16)
	{ // droite de la forme x = b'y + c' ;
		a = -L[0]/L[1] ;
		c = -L[2]/L[1] ;
		y[0] = a * x[0] + c ;
		y[1] = a * x[1] + c ;
	}
	else 
	{
		y[0] = -Dy ;
		y[1] = -Dy ;
	}
	
	for(n=0 ; n<4 ; n++)
	{
		if( (x[n]>=0.0) && (y[n]>=0.0) && (x[n]<=Dx) && (y[n]<=Dy) && (nb_points_ok<2) )
		{
			x_inter[nb_points_ok] = (int)floor(x[n]+0.5) ;
			y_inter[nb_points_ok] = (int)floor(y[n]+0.5) ;
			nb_points_ok ++ ;
		}
	}
	
	if(nb_points_ok==2) return 1 ; 
	else return 0 ;	
}


int main(int argc, char *argv[])
{
	int nombre_de_points = 2, n=0 ;
	int xd[nombre_de_points], yd[nombre_de_points], xg[nombre_de_points], yg[nombre_de_points] ;
	char droite_gaughe = 'd' ;
	
	if(argc<2) 
	{
		printf("\nCe programme a deux arguments d'appel qui sont les deux images droite et gauche\n") ;
		printf("Il faut le lancer de la forme ./Stereo ImageDroite.tif ImageGauche.tif\n") ;
		return 0 ;
	}
	
	// Chargement des deux images dont les noms ont été passés au programme principal
	CImg<unsigned char> imageD(argv[1]),imageG(argv[2]) ;
	// Definition des couleurs de trace (rouge, vert, bleu)
	const unsigned char red[] = { 255,0,0 }, green[] = { 0,255,0 }, blue[] = { 0,0,255 };
	// Creation des objets d'affichage
	CImgDisplay Droite_disp(imageD,"Image droite"), Gauche_disp(imageG,"Image gauche");
	
	// Selection de nombre_de_points paires de points (en commencant par l'image droite
	while (!Droite_disp.is_closed() && !Gauche_disp.is_closed() && n<nombre_de_points) 
	{
		switch (droite_gaughe) 
		{
			case 'd' :
  		Gauche_disp.set_title("%s","Image gauche");
		  Droite_disp.set_title("%s","Cliquez ici") ;
				Droite_disp.wait();
				if (Droite_disp.button() && Droite_disp.mouse_y()>=0) 
				{
					yd[n] = Droite_disp.mouse_y();
					xd[n] = Droite_disp.mouse_x();
					imageD.draw_circle(xd[n],yd[n],3,red,1.0,1).display(Droite_disp);
					droite_gaughe = 'g' ;
				} break ;
			case 'g' :
		  Droite_disp.set_title("%s","Image droite") ;
				Gauche_disp.set_title("%s","Cliquez ici");
				Gauche_disp.wait();
				if (Gauche_disp.button() && Gauche_disp.mouse_y()>=0) 
				{
					yg[n] = Gauche_disp.mouse_y();
					xg[n] = Gauche_disp.mouse_x();
					imageG.draw_circle(xg[n],yg[n],3,blue,1.0,1).display(Gauche_disp);
					droite_gaughe = 'd' ; n++ ;
				} break ;
			default : break;
		}		
	}
	
	// Affichage de tous les points en vert
	for(n=0 ; n<nombre_de_points ; n++) 
	{
		imageD.draw_circle(xd[n],yd[n],3,green,1.0,1).display(Droite_disp);
		imageG.draw_circle(xg[n],yg[n],3,green,1.0,1).display(Gauche_disp);
	}
	
	// Selection de deux points dans l'image droite et affichage de la droite passant par ces deux points
	n=0 ;
	double delta, L[3] ;
	int x_inter[2], y_inter[2] ;
	while (!Droite_disp.is_closed() && !Gauche_disp.is_closed() && n<2) 
	{
		Droite_disp.set_title("%s","Cliquez deux points") ;
		Droite_disp.wait();
		if (Droite_disp.button() && Droite_disp.mouse_y()>=0) 
		{
			yd[n] = Droite_disp.mouse_y();
			xd[n] = Droite_disp.mouse_x();
			imageD.draw_circle(xd[n],yd[n],3,red,1.0,1).display(Droite_disp);
			n = n+1 ;
		}			
	}
	delta = (double)xd[0]*(double)yd[1] - (double)xd[1]*(double)yd[0] ;
	imageD.draw_line(xd[0],yd[0],xd[1],yd[1],green).display(Droite_disp);

	L[0] = (double)( yd[1] - yd[0] ) ;
	L[1] = (double)( xd[0] - xd[1] ) ;
	L[2] = (double)xd[1]*(double)yd[0] - (double)xd[0]*(double)yd[1] ;		
			
	n = Intersection(L, imageD.width(), imageD.height(), x_inter, y_inter ) ;
	if(n)	imageD.draw_line(x_inter[0],y_inter[0],x_inter[1],y_inter[1],red).display(Droite_disp);
	
 // Partie calcul sur les matrices
 
	// Definition d'une matrice de 5 lignes et 3 colonnes (et un plan) en double precision
	// remplie au depart de 0
	
	CImg <double> matrice_A(5,3,1,1,0) ; 
	CImg <double>::iterator it ; // defiition d'un iterateur (permet d'avoir le premier element de la matrice)
	int lin, col, NlinA, NcolA, NlinB, NcolB, NlinC, NcolC ;
	
	NlinA = matrice_A.height() ;
	NcolA = matrice_A.width() ;
	
	// Remplissage de la matrice A avec des valeurs aleatoires entre 0 et 10
	matrice_A.rand(0,10) ;
	
	// Affichage de la matrice A
	it = matrice_A.begin() ;
	for( lin=0 ; lin<NlinA ; lin++)
	{
		for( col=0 ; col<NcolA ; col++, it++)
		{
			printf("[%g]",(*it)) ;
		}
		printf("\n") ;
	}
	
 // Definition de la matrice B comme etant la pseudo-inverse de A 
	CImg <double> matrice_B =	matrice_A.pseudoinvert() ;
	printf("\n\n\n") ;
	
	NlinB = matrice_B.height() ;
	NcolB = matrice_B.width() ;
	
	// Affichage de la matrice B
	it = matrice_B.begin() ;
	for( lin=0 ; lin<NlinB ; lin++)
	{
		for( col=0 ; col<NcolB ; col++, it++)
		{
			printf("[%g]",(*it)) ;
		}
		printf("\n") ;
	}
	
	CImg <double> matrice_C(NlinA,NcolB,1,1,0) ;
	NlinC = matrice_C.height() ;
	NcolC = matrice_C.width() ;
	
	printf("\n\n\n") ;
	
	// Calcul de C = A * B au sens des matrices 
	MatMult((double *)matrice_A.begin(), (double *)matrice_B.begin(), (double *)matrice_C.begin(), NlinC, NcolA, NcolC) ;
	
	// Affichage de la matrice C
	it = matrice_C.begin() ;
	for( lin=0 ; lin<NlinC ; lin++)
	{
		for( col=0 ; col<NcolC ; col++, it++)
		{
			printf("[%g]",(*it)) ;
		}
		printf("\n") ;
	}
	
	// Attente de la fermeture d'une des images pour arrêter le programme
	while (!Droite_disp.is_closed() && !Gauche_disp.is_closed()) ;
	
	return 0;
}



