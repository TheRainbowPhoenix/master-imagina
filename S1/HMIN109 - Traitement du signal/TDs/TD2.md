# TD2

# Exercice 1

```
S/B = 30 db
S/B = 10 log10 S/N <=> S/N = 10^1((S/B)/10)
S/B = 30 -> S/N = 1000
C = BP * log2(1 + 1000) = BP * log10(1001)/log10(2) = (3400 - 300) * 9,96 = 30800 b/s = 30,8 kb/s
```
# Exercice 2

```
n = 8 * 100
Te = 10^-4
Pe = (1 - Te)^n
Pe = (1 - 10^4)^800 = 0.92

Un message à une probabilité de 8% d'avoir une erreur
```

# Exercice 3

```
a) Liasion L = 56 000 bits/s
Temps de transmission sans erreur T = 8*1500/56000 = 0.214s
TTI = 7*1500/0.214 = 49065 bits/s
P = 49065/56000 = 88%

b) n = 8 * 1500
Te = 10^-5
P = (1 - 10^-5)^12000 = 89%
Temps transmission avec erreur tc = t/Pe = 0.214/0.89 = 0.240
TTIe = 10500/0.240 = 43750 bits/s
Pe = 43750/56000 = 78%
```

# Exercice 4

```
a) dpi = dot per inch

A4 = 21cm x 29,7cm 
A4 = 8,27 inch x 11,7 inch
A4 = 96,76 inch^2

300^2   = 1 inch^2
8708400 = 96,76 inch^2

T = 8708400/9600 
T = 907,125s 
T = 15 min

b) Le taux de compression doit être superieur à 15

c) D = 64 kbits/s

-> 15 Millions de pts 
-> 15 Millions de bits en 3s
-> 5 Millions bits/s

débit 64000 bits/s

Taux de compression = 5M/64k = 78
```

# Exercice 5

```
a) S/B = 10 log10(S/N) 
-> (S/B)/10 = log10(S/N)
-> S = log10(S/N)
-> S/N = 10^2

b) Nl = 5*100 = 500 lignes
Npl = 10*100 = 1000 pts/lignes
Np = 500 * 1000 = 500000

10 nuances dont 2^4 donc on encode sur 4 bits
Nb = 500000*4 = 2000000 bits

c) débit_tel : 2*BP_tel : 2000 bauds ~= 2000 bits/s
debit_ligne : BP_ligne * log2(1+100) = 2000 * log2(101) = 13316 bauds ~= 13316 bits/s

Tt : 2000000/2000 = 1000s = 16,6 min
```