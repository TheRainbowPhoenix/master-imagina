# TD3

# Exercice 1

**Question 1**

C1:

| x1 | x3 |
|----|----|
| 0  | 1  |

C2:

| x3 | x2 |
|----|----|
| 0  | 1  |
| 1  | 2  | 
| 0  | 2  |

```
Ce réseau n'est pas consistent, on peut trouver un exemple qui le prouve.

x1 ----- c1 ----- x3 ----- c2 ----- x2
0        0|1     ~0~       0|1     ~0~                  
~1~               1        1|2     ~1~
                           0|2      2       
```

**Question 2**

```
Arc-Consistant 
=/> Il existe au moins 1 solutions

Non arc-consistant 
=/> il n'existe aucune solution

Si on ne peut pas rendre un réseau arc consistent en calculant sa fermeture arc consistente 
=/> il n'existe pas de solutions au réseau

                     X1                              
                     R                             
           !=        B          !=                   
        R|B                       R|B                      
        B|R                       B|R
                                  B|G

           X2                   X3                     
           R                     R                   
           B          !=         B                    
                      R|B        G
                      B|R
                      B|G

```
# Exercice 2

**Question 1**

```
X = {LR1,CR1,LR2,CR2,LR3,CR3,LR4,CR4}
Pour tout x de X, D(x) = {1,2,3,4}

C = { 
	  C1 = <LR1,LR2,LR3,LR4> AllDiff,
	  C2 = <CR1,CR2,CR3,CR4> AllDiff,
	  C3 = <LR1,CR1,LR2,CR2> : |LR1 - LR2| != |CR1 != CR2| 
	}

```

# Exercice 3

```
Soit le réseau P = {X,D,C}

X = {U,T,V,W}
D(T) = {1,2}
D(V) = D(U) = {1,2,3}
D(W) = {1,2,3,4}

L = {L1,L2,L3,L4,L5}

avec L1... (sur poli)
```

**Question 1**

```
                           V
                         /   \                                  
                       /       \                               
                    L1           L4                              
                   /               \                           
                  /                 \                          
                 T---------L3--------W                        
                 | \                 |                        
                 |    \              |                        
                 |      \           L2                         
                 L5-------V---------/        
```

**Question 2**

```
{}              *
              /   \         
T           1       2                
          / | \     |               
U        1  2  3    1                  
          / | \    / \             
V        1  2  3  1   2         
       /| |\                             
W     1 2 3 4                          
```

**Question 3**

```



```                                                                                     
**Question 5**

```
Dac(T) = {2}
Dac(U) = {1,2}
Dac(V) = {2}
Dac(W) = {1,2,3}

la k-forte-consistence d'un réseau à k variables 
=> l'existence d'une solution
```