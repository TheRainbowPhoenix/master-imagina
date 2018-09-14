# TD1

# EXERCICE 1

1. BONJOUR
VRC  LRC | VRC   R  | VRC   U  | VRC   O  | VRC   J  | VRC   N  | VRC   O  | VRC   B
 x xxxxxx   x xxxxxx   x xxxxxx   x xxxxxx   x xxxxxx   x xxxxxx   x xxxxxx   x xxxxxx 

2. B = 66, O = 79, N = 78, J = 74, U = 85, R = 82 

	B	O	N	J	O	U	R	LRC
64	1 	1	1	1	1	1	1	1
32	0 	0	0	0	0	0	0	0
16	0 	0	0	0	0	1	1	0
8	0 	1	1	1	1	0	0	0
4	0 	1	1	0	1	1	0	0
2	1 	1	1	1	1	0	1	0
1	0 	1	0	0	1	1	0	1
VRC	0 	1	0	1	1	0	0
		 R			   U			 O			   J			 N			  O				B
3. [0]101 0010 | [0]101 0010 | [1]100 1111 | [1]100 1010 | [0]100 1110 | [1]100 1111 | [0]100 0010

4. 1. VRC + LRC, On réussit à localiser et corriger l'erreur

	B	O	N	J	O	U	R	LRC
64	1 	1	1	1	1	1	1	1
32	0 	0	0	0	0	0	0	0
16	0 	0	0	0	0	1	1	0
8	0 	1	x	1	1	0	0	0 <
4	0 	1	1	0	1	1	0	0 
2	1 	1	1	1	1	0	1	0
1	0 	1	0	0	1	1	0	1
VRC	0 	1	0	1	1	0	0
			^
4. 2. LRC, On réussit à detecter 2 erreurs

	B	O	N	J	O	U	R	LRC
64	1 	1	1	1	1	1	1	1
32	0 	0	x	0	0	0	0	0 <
16	0 	0	0	0	0	1	1	0 
8	0 	1	x	1	1	0	0	0 <
4	0 	1	1	0	1	1	0	0
2	1 	1	1	1	1	0	1	0
1	0 	1	0	0	1	1	0	1
VRC	0 	1	0	1	1	0	0

4. 3. VRC, On réussit à detecter 2 erreurs

	B	O	N	J	O	U	R	LRC
64	1 	1	1	1	1	1	1	1
32	0 	0	0	0	0	0	0	0
16	0 	0	0	0	0	1	1	0
8	0 	1	x	1	1	x	0	0
4	0 	1	1	0	1	1	0	0 
2	1 	1	1	1	1	0	1	0
1	0 	1	0	0	1	1	0	1
VRC	0 	1	0	1	1	0	0
			^			^
4. 4. On ne voit pas l'erreur

	B	O	N	J	O	U	R	LRC
64	1 	1	1	1	1	1	1	1
32	0 	0	x	0	0	x	0	0
16	0 	0	0	0	0	1	1	0
8	0 	1	x	1	1	x	0	0
4	0 	1	1	0	1	1	0	0 
2	1 	1	1	1	1	0	1	0
1	0 	1	0	0	1	1	0	1
VRC	0 	1	0	1	1	0	0
					
# EXERCICE 2

1. Pas de LRC que des [VRC] 1001 1110 1010 0011 1100 1010

sens de lecture -> avec VRC à droite
	1 			  2				3
1001 111[0] | 1010 001[1] | 1100 101[0]

	1	2	3
64	1	1   1
32	0	0	1
16	0	1	0
8	1	0	0
2	1	0	1
4	1	0	0
1	1	1	1
VRC 0	1	0

(pas d'erreur dans ce sens de lecture)

sens de lecture <- avec VRC à droite
	1			  2			    3
1001 111[0] | 1010 001[1] | 1100 101[0]

	1	2	3
1	1	1   1
2	0	0	1
4	0	1	0
8	1	0	0
16	1	0	1
32	1	0	0
64	1	1	1
VRC 0	1	0
	^
(erreur detecter sur le le paquet de bit 1)

2. YES

# EXERCICE 3

1. 
11101100
101
 100
 101
   111
   101
    100
    101
      10

