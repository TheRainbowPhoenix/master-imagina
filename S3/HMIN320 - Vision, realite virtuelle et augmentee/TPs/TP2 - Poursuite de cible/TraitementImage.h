#include "math.h"
#include "stdio.h"

#define ALLOCATION_KO 0
#define ALLOCATION_OK 1
#define TERMINE 2

#define ALLOCATION(N,type) new type [N]   // pour C++ windows
#define DESALLOCATION(pointeur) delete pointeur   // pour C++ windows

#define EPSILON 1e-32


#define Fabs(a) ( ( (a) > 0.0 ) ? (a) : (-(a)) )
#define Min(a,b) ( ( (a) < (b) ) ? (a) : (b) )
#define Max(a,b) ( ( (a) > (b) ) ? (a) : (b) )


 int Derive_image(double * Image,
																		double * Imagex,
																		double * Imagey, 
																		int ncol,
																		int nlin,
																		double alpha) //typiquement 0.4
 {

  double ao,oa,ko,klo;
  double *B1,*B2, *dx, *dy, *dxy ;
  double *debut, *fin ;
  int dim,ncol1,nlin1,nbre_d_element;
  int col,lin;
  double *pixel_suivant ;
  double *B_courant, *B_suivant,*B_precedent ;
 

  ncol1 = ncol - 1;
  nlin1 = nlin - 1;
  nbre_d_element = ncol * nlin;
  dim = (nlin>ncol) ? nlin : ncol ;
  ao = ( (alpha>0.0) && (alpha<1.0) ) ? alpha : 0.135 ;
  oa = 1.0 - ao ;
  ko = oa/(2.0*ao);
  klo=1.0/(2.0-ao);
  /* allocation memoire*/

  B1  = ALLOCATION(dim ,double ) ;
  B2  = ALLOCATION(dim ,double ) ;

  if((B1==NULL)||(B2==NULL))
  {
   printf( "Impossible d'allouer B1 ou B2 dans la procedure de derivation !!");
   return(0) ;
  }


/**********************************/
/* Calcul de la derivee suivant y */
/**********************************/

  /* lissage en x */
 for( debut=Image, lin=0 ; lin<nlin ; lin++, debut+=ncol )
  {
    /* Lissage Causal : B1 */

   B_courant = B1 ;
   B_suivant = B1 ;
   pixel_suivant = debut ;

   (*B_suivant) =  (*pixel_suivant) ;
   B_suivant++  ; pixel_suivant++;
  
   fin = B1 + ncol ;

   while(B_suivant<fin)
   {	
    (*B_suivant) = ao * (*B_courant) +
		   oa * (*pixel_suivant) ;
    B_suivant++ ; B_courant++ ;
    pixel_suivant++ ;
   }
    
    /* Lissage Anti-Causal : B2 */
   
   B_courant = B2 + ncol1 ;
   B_suivant = B2 + ncol1 ;
   pixel_suivant = debut + ncol1 ;

   (*B_suivant) =  (*pixel_suivant) ;
   B_suivant-- ; pixel_suivant--;
  
   fin = B2 ;

   while(B_suivant>=fin)
   {	
    (*B_suivant) = ao * (*B_courant) +
                   oa * (*pixel_suivant);		 

    B_suivant-- ; B_courant-- ;
    pixel_suivant-- ; 
   }

   
   B_precedent = B1 ; B_suivant = B2 ;
   dy = Imagey + ( lin * ncol ) ; fin = dy + ncol ;
   pixel_suivant = Image + (lin * ncol);
   while(dy<fin)
   {
    (*dy) = klo*((*B_precedent) + (*B_suivant)-ao * (*pixel_suivant))  ;
    dy++ ; B_precedent++ ; B_suivant++ ;pixel_suivant++;
   }


  } /* fin du lissage */


/****************************************************************************/

  /* derivateur en y */


  for( debut=Imagey, col=0 ; col<ncol ; col++, debut++ )
  { 

    // derivateur Causal : B1 
   
   B_courant = B1 ;
   B_suivant = B1 ;
   pixel_suivant = debut ;

   (*B_suivant) =  (*pixel_suivant) ;
 
   B_suivant++  ; pixel_suivant+=ncol ; 
   fin = B1 + nlin ;

   while(B_suivant<fin)
   {	
    (*B_suivant) = ao * (*B_courant) +
                   oa * (*pixel_suivant);
    B_suivant++ ; B_courant++ ;
    pixel_suivant+=ncol ;
   }

    // Lissage Anti-Causal : B2 
   
   B_courant = B2 + nlin1 ;
   B_suivant = B2 + nlin1 ;
   pixel_suivant = debut + nlin1*ncol ;

   (*B_suivant) =  (*pixel_suivant) ;
 
   B_suivant-- ;
   pixel_suivant-=ncol ; 
   fin = B2 ;

   
   while(B_suivant>=fin)
   {	
    (*B_suivant) = ao * (*B_courant) +
                   oa * (*pixel_suivant);
    B_courant-- ;
    B_suivant--;
    pixel_suivant-=ncol ;
   }

  
   B_precedent = B1 ; B_suivant = B2 ;
   dy = Imagey + col ; fin = dy + nlin*ncol;
   while(dy<fin)
   {
    (*dy) = ko*(-(*B_precedent) + (*B_suivant))  ;
    dy+=ncol ; B_precedent++ ; B_suivant++ ;
   }
   
  } // fin du derivateur 



/**********************************/
/* Calcul de la derivee suivant x */
/**********************************/

 
  /* lissage en y */


  for( debut=Image, col=0 ; col<ncol ; col++, debut++ )
  {
    // lissage Causal : B1 

   B_courant = B1 ;
   B_suivant = B1 ;
   pixel_suivant = debut ;

   (*B_suivant) =  (*pixel_suivant) ;
   B_suivant++  ; pixel_suivant+=ncol;
   
   fin = B1 + nlin ;

   while(B_suivant<fin)
   {	
    (*B_suivant) = ao * (*B_courant) + 
                   oa * (*pixel_suivant);
    B_suivant++ ; B_courant++ ; 
    pixel_suivant+=ncol ;
   }



    // Lissage Anti-Causal : B2 
   
   B_courant = B2 + nlin1 ;
   B_suivant = B2 + nlin1 ;
   pixel_suivant = debut + nlin1 *ncol;

   (*B_suivant) =  (*pixel_suivant) ;
   B_suivant-- ; pixel_suivant-=ncol;
  
   fin = B2 ;

   while(B_suivant>=fin)
   {	
    (*B_suivant) = ao * (*B_courant) + 
                   oa * (*pixel_suivant);

    B_suivant-- ; B_courant-- ; 
    pixel_suivant-=ncol ;
   }

  
   B_precedent = B1 ; B_suivant = B2 ;
   dx = Imagex + col ; fin = dx + nlin *ncol;
   pixel_suivant = Image + col;
   while(dx<fin)
   {
    (*dx) =klo*( (*B_precedent) + (*B_suivant)-ao * (*pixel_suivant))  ;
    dx+=ncol ; B_precedent++ ; B_suivant++ ;pixel_suivant+=ncol;
   }

  } // fin du lissage 




  // derivee en x 


  for( debut=Imagex, lin=0 ; lin<nlin ; lin++, debut+=ncol )
  {

    // derivee Causal : B1 
   

   B_courant = B1 ;
   B_suivant = B1 ;
   pixel_suivant = debut ;

   (*B_suivant) =  (*pixel_suivant) ;
 
   B_suivant++ ; pixel_suivant++; 
   fin = B1 + ncol ;

   while(B_suivant<fin)
   {	
    (*B_suivant) = ao * (*B_courant) + 
                   oa * (*pixel_suivant);
    B_suivant++ ; B_courant++ ;
    pixel_suivant++ ;
   }

  
   // derivee Anti-Causal : B2 
   B_courant = B2 + ncol1 ;
   B_suivant = B2 + ncol1 ;
   pixel_suivant = debut + ncol1 ;

   (*B_suivant) =  (*pixel_suivant) ;
  
   B_suivant-- ;
   pixel_suivant-- ;
   
   fin = B2 ;

   while(B_suivant>=fin)
   {	
    (*B_suivant) = ao * (*B_courant) +
                   oa * (*pixel_suivant) ;
    B_courant-- ;
    B_suivant--;
    pixel_suivant-- ;
   }

  
 
   B_precedent = B1 ; B_suivant = B2 ;
   dx = Imagex + ( ncol * lin ) ; fin = dx + ncol;

   while(dx<fin)
   {
    (*dx) =ko*(- (*B_precedent) + (*B_suivant))  ;
    dx++ ; B_precedent++ ; B_suivant++ ;
   }


  } // fin du derivateur

 DESALLOCATION(B1);
 DESALLOCATION(B2);
 return 0;
 
}

int Lisse_image(double * Image,
																int nlin,
																int ncol,
																double alpha) //typiquement 0.4
{
	
	double ao,oa,ko,klo;
	double *B1,*B2, *pt ;
	double *debut, *fin ;
	int dim,ncol1,nlin1,nbre_d_element;
	int col,lin;
	double *pixel_suivant ;
	double *B_courant, *B_suivant,*B_precedent ;
 
	
	ncol1 = ncol - 1;
	nlin1 = nlin - 1;
	nbre_d_element = ncol * nlin;
	dim = (nlin>ncol) ? nlin : ncol ;
	ao = ( (alpha>0.0) && (alpha<1.0) ) ? alpha : 0.135 ;
	oa = 1.0 - ao ;
	ko = oa/(2.0*ao);
	klo=1.0/(2.0-ao);
	/* allocation memoire*/
	
	B1  = ALLOCATION(dim ,double ) ;
	B2  = ALLOCATION(dim ,double ) ;
	
	if((B1==NULL)||(B2==NULL))
	{
		printf( "Impossible d'allouer B1 ou B2 dans la procedure Deriche!!");
		return(0) ;
	}
	
	
	/* lissage en x */
	
 for( debut=Image, lin=0 ; lin<nlin ; lin++, debut+=ncol )
	{
		/* Lissage Causal : B1 */
		
		B_courant = B1 ;
		B_suivant = B1 ;
		pixel_suivant = debut ;
		
		(*B_suivant) =  (*pixel_suivant) ;
		B_suivant++  ; pixel_suivant++;
  
		fin = B1 + ncol ;
		
		while(B_suivant<fin)
		{	
			(*B_suivant) = ao * (*B_courant) +
			oa * (*pixel_suivant) ;
			B_suivant++ ; B_courant++ ;
			pixel_suivant++ ;
		}
		
		/* Lissage Anti-Causal : B2 */
		
		B_courant = B2 + ncol1 ;
		B_suivant = B2 + ncol1 ;
		pixel_suivant = debut + ncol1 ;
		
		(*B_suivant) =  (*pixel_suivant) ;
		B_suivant-- ; pixel_suivant--;
  
		fin = B2 ;
		
		while(B_suivant>=fin)
		{	
			(*B_suivant) = ao * (*B_courant) +
			oa * (*pixel_suivant);		 
			
			B_suivant-- ; B_courant-- ;
			pixel_suivant-- ; 
		}
		
		
		B_precedent = B1 ; B_suivant = B2 ;
		pt = Image + ( lin * ncol ) ; fin = pt + ncol ;
		pixel_suivant = Image + (lin * ncol);
		while(pt<fin)
		{
			(*pt) = klo*((*B_precedent) + (*B_suivant)-ao * (*pixel_suivant))  ;
			pt++ ; B_precedent++ ; B_suivant++ ;pixel_suivant++;
		}
		
		
	} /* fin du lissage */
	
	
	/****************************************************************************/
	
	/* lissage en y */
	
	
	for( debut=Image, col=0 ; col<ncol ; col++, debut++ )
	{ 
		
		// lissage Causal : B1 
		
		B_courant = B1 ;
		B_suivant = B1 ;
		pixel_suivant = debut ;
		
		(*B_suivant) =  (*pixel_suivant) ;
		
		B_suivant++  ; pixel_suivant+=ncol ; 
		fin = B1 + nlin ;
		
		while(B_suivant<fin)
		{	
			(*B_suivant) = ao * (*B_courant) +
			oa * (*pixel_suivant);
			B_suivant++ ; B_courant++ ;
			pixel_suivant+=ncol ;
		}
		
		// Lissage Anti-Causal : B2 
		
		B_courant = B2 + nlin1 ;
		B_suivant = B2 + nlin1 ;
		pixel_suivant = debut + nlin1*ncol ;
		
		(*B_suivant) =  (*pixel_suivant) ;
		
		B_suivant-- ;
		pixel_suivant-=ncol ; 
		fin = B2 ;
		
		
		while(B_suivant>=fin)
		{	
			(*B_suivant) = ao * (*B_courant) +
			oa * (*pixel_suivant);
			B_courant-- ;
			B_suivant--;
			pixel_suivant-=ncol ;
		}
		
  
		B_precedent = B1 ; B_suivant = B2 ;
		pt = Image + col ; fin = pt + nlin*ncol;
		pixel_suivant = Image + col;
		while(pt<fin)
		{
			(*pt) = klo*((*B_precedent) + (*B_suivant)-ao * (*pixel_suivant))  ;
			pt+=ncol ; B_precedent++ ; B_suivant++ ;pixel_suivant+=ncol;
		}
				
	} // fin du lissage 
	
	
 DESALLOCATION(B1);
 DESALLOCATION(B2);
 return 0;
 
}

int Derive_temporelle_image(double * Image,
																												double * ImageFt, 
																												double * Imaget, 
																												int nlin,
																												int ncol,
																												double alpha) //typiquement 0.4
{
	double ao,oa ;
	double *pt, *ptf, *pti ;
	double *debut, *fin ;
	int col,lin, N ;
	
	ao = ( (alpha>0.0) && (alpha<1.0) ) ? alpha : 0.135 ;
	oa = 1.0 - ao ;
	N = nlin * ncol ;
	
 ptf = ImageFt ;	
 pti = Image ;
	fin = pti + N ;
	
	// filtrage en t
	while(pti<fin)
	{
		(*ptf) = ao*(*ptf) + oa*(*pti) ;
		ptf++ ; pti++ ;
	}
	
	ptf = ImageFt ;	
	pti = Image ;
	pt = Imaget ;
	fin = pti + N ;

	while(pti<fin)
	{
		(*pt) = (*ptf) - (*pti) ;
		ptf++ ; pt++ ; pti++ ;
	}
	
	
	Lisse_image(Imaget, nlin, ncol, alpha) ;
	return 1 ;
	
}

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

double Inverse2x2(double *A)
{
	double det, tmp ;
	
	det = A[0]*A[3] - A[1]*A[2] ;
	if(fabs(det)<1e-16) return det ;
	tmp = A[0]/det ; A[0] = A[3]/det ; A[3] = tmp ; 
	A[1] = -A[1]/det ;
	A[2] = -A[2]/det ;	
}


// Creation d'une Image_transformee qui est la transformee de l'Image_originale par une rotation de theta et une translation de (tx,ty) 
int Transformation(double *Image_originale, double *Image_transformee, int Nx, int Ny, double tx, double ty, double theta, char interpolation) 
{
	int x, y, n ;
	int xc, yc ;
	double dx, dy, *pt ;
	double c_theta, s_theta ;
	
	pt = Image_transformee ;
	
	c_theta = cos(theta) ;
	s_theta = sin(theta) ;
	
	for(y=0 ; y<Ny ; y++)
	{
		for(x=0 ; x<Nx ; x++, pt++)
		{
			(*pt) = 0.0 ;
			dx = c_theta*(double)x - s_theta*(double)y + tx ;
			dy = s_theta*(double)x + c_theta*(double)y + ty ;
			if(interpolation)
			{
				xc = (int)floor(dx) ; dx -= (double)xc ;
				yc = (int)floor(dy) ; dy -= (double)yc ;
				if( (xc>=0) && (xc<Nx) && (yc>=0) && (yc<Ny) )
				{
					n=yc*Nx+xc ;
					(*pt) = (1.0-dx)*(1.0-dy)*Image_originale[n] ;
				}				
				xc+=1 ;
				if( (xc>=0) && (xc<Nx) && (yc>=0) && (yc<Ny) )
				{
					n=yc*Nx+xc ;
					(*pt) += (dx)*(1.0-dy)*Image_originale[n] ;
				}
				yc++ ;
				if( (xc>=0) && (xc<Nx) && (yc>=0) && (yc<Ny) )
				{
					n=yc*Nx+xc ;
					(*pt) += (dx)*(dy)*Image_originale[n] ;
				}
				xc-- ;
				if( (xc>=0) && (xc<Nx) && (yc>=0) && (yc<Ny) )
				{
					n=yc*Nx+xc ;
					(*pt) += (1.0-dx)*(dy)*Image_originale[n] ;
				}
			}
			else 
			{
				xc = (int)floor(dx + 0.5) ;
				yc = (int)floor(dy + 0.5) ;
				if( (x>=0) && (x<Nx) && (yc>=0) && (yc<Ny) )
				{
					n=yc*Nx+xc ;
					(*pt) = Image_originale[n] ;
				}								
			}
		}
	}
	return 1 ;	
}


double CorrelationPearson(double *Image_1, double *Image_2, int Nx, int Ny) 
{
	int N ;
 double	*pt1, *pt2, *fin ;
	double moy1, moy2 ;
	double var1, var2 ;
	double resultat = 0.0 ;
	
	N = Nx * Ny ;
	moy1 = 0.0 ;	moy2 = 0.0 ;
	var1 = 0.0 ;	var2 = 0.0 ;

	fin = Image_1 + N ;
	pt1 = Image_1 ;	pt2 = Image_2 ;
	
	// calcul des moyennes
	while(pt1<fin)
	{
		moy1 += (*pt1++) ;
		moy2 += (*pt2++) ;		
	}
	moy1 /= (double)N  ;
	moy2 /= (double)N  ;

	pt1 = Image_1 ;	pt2 = Image_2 ;
	
	// calcul des variances
	while(pt1<fin)
	{
		resultat = (*pt1++) - moy1 ; 
		var1 += resultat*resultat  ;
		resultat = (*pt2++) - moy2 ; 
		var2 += resultat*resultat  ;
	}
	
	// calcul de la correlation
	while(pt1<fin) resultat += ( (*pt1++) - moy1 ) * ( (*pt2++) - moy2 ) ;
	
	return ( resultat / sqrt(var1*var2) ) ;	
}
