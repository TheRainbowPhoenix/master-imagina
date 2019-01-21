# TD3

# Exercice 1

**Question a)**
```
0 : 10
1 : 110
2 : 0110
3 : 0111
4 : 0101
5 : 000 
6 : 001
7 : 0100
8 : 1110
9 : 1111

2000 * (0.42 + 3 * (0.11 + 0,12 + 0.14) + 4 * (0.08 + 0.07 + 0.09 + 0.06 + 0.03)) = 6420 bits
```

**Question b)**
```
debit = (fmax - fmin) * 2
= (3500 - 500) * 2
= 6000 bit/s
t = 6420/6000 = 1.07s 
```

**Question c)**
```
h * 2000 = 8000 bit
tc = 8000 ~= 1.25
```

**Question d)**
```
On suppose qu'il y a du bruit

S/B = 10*log10(S/N)

6420 bit -> 2s
c = 3210 bit/s
c = BP * log2(1 + S/N)

S/(10 * B) = log10(S/N)

10^S/(10 * B) = S/N
c = BP * log2(1 + 10^S(10 * B))
c/BP = log2(1 + 10^S(10 * B))
2^c/BP = 1 + 10^S(10 * B)
2^c/BP -1 = 10^S(10 * B)
log10(2^c/BP -1) = S(10 * B)
10 * log10(2^c/BP -1) = S/B
S/B ~= 0.414 db
S/N ~= 1.1
```

# Exercice 2

**Question a)**
```
(Schema arbre)

0 : 1110
1 : 00
2 : 10
3 : 01
4 : 1111
5 : 110
```

**Question b)**
```
10 * 20 = 200 cm^2 = 3 inch^2
31 * 600^2 = 11 160 000 points

11.16 * 10^4 * (2 (0.2 + 0.31 + 0.19) + 3 * 0.12 + 4 * (0.1 + 0.08))
= 27 676 800 bit = 27,7676

11 160 000 * 3 / 27.7 * 10^6 = 33.5/27.7 * 10^6 = 1.20
```

**Question d)**
```
C = 600 000 * log2(1 + 10^2.5)
C = 600 000 * log(1 + 10^2.5) / log(2)
  =~ 5 Mbits/s

t = 27 676 800 / 5.10^6 = 5.53 sec
```

**Question e)**
```
Taux de compression = 5.53
```

# Exercice 6

**Question 1**

|     | S | N | O | W | LRC |
|-----|---|---|---|---|-----|
| 64  | 1 | 1 | 1 | 1 | 1   |
| 32  | 0 | 0 | 0 | 0 | 1   |
| 16  | 1 | 0 | 0 | 1 | 1   |
| 8   | 0 | 1 | 1 | 0 | 1   |
| 4   | 0 | 1 | 1 | 1 | 0   |
| 2   | 1 | 1 | 1 | 1 | 1   |
| 1   | 1 | 0 | 1 | 1 | 0   |
| VRC | 0 | 0 | 1 | 1 | 1   |

**Question 2**
```
10011100000|1011
1011
  1011 	
  1011
```

# Exercice 3



# Exercice 5

**Question a)**
```
128 niveau -> log2(128) -> 7 bits
&&	
signalisation -> 1 bits
-> 
IT de 8 bits
```

**Question b)**
```
Débit par voie de 64 kbits/s IT de 8 bits

64000 / 8 = 8000 trames

longueur d'une trame (taille) 8 bits/voie 

32 * 8 = 256 bits
```

**Question c)**
```
D = somme débit des voies = 32 * 64000 = 2048 kbits/s
```

**Question d)**
```
Dutile = 8000 * 7 * 31 = 1736 bits/s

Eff = 1736 / 2048 = 84.7 % =~ 7/8 * 31/32
```
