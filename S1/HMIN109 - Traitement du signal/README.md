# HMIN109 - Traitement du signal

## Sommaire [↺](#sommaire-)

* [Informations](#informations-)
  + [Examens](#examens-)
  + [Ressources](#ressources-)

## Informations [↺](#sommaire-)

### Examens [↺](#sommaire-)

- Note final : 67% Exam + 33% TP

### Ressources [↺](#sommaire-)

- [Cours](http://www.lirmm.fr/~wpuech/enseignement/master_informatique/index.html)
- [TPs](http://www.lirmm.fr/~strauss/MasterInfo/TravauxPratiquesHMIN109.html)

## I) LES RÉSEAUX

- Applications : telnet, ftp, nfs
- Ordinateurs  : PC, stations de travail, périphériques, terminaux
- Coupleurs    : asynchrone, synchrone, ethernet
- Adaptateurs  : modem, transceiver
- Liens        : paire trosadée, câble coaxial, fibre optique, ondes hertziennes.
- Connexion et interconnection : noeuds, routeurs, commutateurs, répéteurs.
- Langages : signaux électriques, bytes, trames, fonctions dans les applications.
- Normes et standards : 

**Buts**

- Echanges entre personnes
- Partage déquipements
- Terme réseau très vague

**Supports : des caractèristiques au choix**

- Coût
- Bande passante

**Codage de l'information**

- Alphabet
- ASCII (1 lettre = 1 octet)
- Paquets
- 8 bits ou 7 bits + parité ou 4B/5B 
- Signaux sur le support
- Niveaux et changements de niveaux

### **Exemple : Bit de parité**
```
On résonne sur les bits à 0

0 1 2 3 4 5 6 7
0 0 1 1 0 1 0|1 (bit de parité)

Ici on compte les bits à 0 et on ne rajoute pas de 0 sur le bit de parité pour avoir un nombre de 0 pair.
```
**Modes de transmission**

- Bits : singaux de support
- Bande de base : représentation directe des bits
- Analogique : modem et porteuse

**Synchronisation entre émetteur et récepteur**

- Synchrone : horloge transmise avec les données
- Asynchrone : devant chaque éléments de données on ajoute un groupe de bits pour l'échantillonnge

**Les erreurs**

l'information reçue doit être identique à l'information émise.
Le signale peut être modifié, des bits ou octets perdus durant le transfert de l'information : erreurs
Il faut les détecter et les corriger.

- Detection d'une modification
- l'émetteur rajoute des bits, fonction des données qu'il transmet
- Le récepteur recalcule la fonction et vérifie

- Détection d'une perte (paquet) : besoin de numérotation, ajoutée par l'émetteur et vérifiée par le récepteur
- Détection d'un mauvais ordre d'arrivée : réseaux maillés

**Fenêtrage**

- Un Ack accusé réception de plusieurs éléments d'information.
- Primordial dans les transferts de fichiers

**Contrôle de flux**

- Flot d'arrivée trop rapide pour le récepteur ou poru les noeuds intermédiares
- Quand fenêtrage : résolu par l'émetteur
- Asynchrone : XON - XOFF
- ICMP : Source Quench

**Mode connecté (un paquet fait le train, les autres suivent)**

- En début de chaque session : création d'un chemin virtuel CV entre les deux protagonistes
- Chaque noeud réserve les ressources nécessaires à la session
- Dans chaque élément d'information : numéro du CV
- Fin de session : chaque noeud est averti
- Exemple : téléphone, X25, 

**Mode non connecté (chaque paquet est un train qui trouve sont chemin)**

- Chaque élément d'information (datagramme) qui circule contient l'adresse du destinataire et de l'émetteur.
- Les noeuds (routeurs) dispatchent à la volée : il faut trouve le bon chemin rapidement (but du routage)

**Les couches**

- Chaque couche ajoute un entête et un identificateur de la couche supérieur

**Adressage et nommage**

- But : identifier un objet réseau
- Adrese liée à la géographie
- Nom lié à la fonction ou l'indetité (personne)
- Problème : unicité et gestion

## II) CONCEPTS DES TÉLÉCOMMUNCATIONS

**Forme**

- texte
- musique
- images fixes

**Codage**

- Adaptation au canal de communication
- Capteurs et transducteurs

### Codage en bande de base

- Binaire
```
	1 0 1 1 0 0 0 0 0 1 0 0 0 0 0
	- _ - - _ _ _ _ _ - _ _ _ _ _
```

- Bipolaire (sequences de O <= n+1)
```
	1 0 1 1 0 0 0 0 0 1 0 0 0 0 0
	- _   - _ _ _ _ _   _ _ _ _ _
		-             -
```

- HDB3n (Haute densité bourrage) 
```
	1 0 1 1 0 0 0 0 0 1 0 0 0 0 0
	- _   -   _ _ - _   _ _ _   _
		-   -         -       -
```

**Téléinformatique**

- Echantillonage: Shannon Fe >= 2 Fmax
- Quantification/codage: Si D est la dynamique du signal: n >= log2 D
- Debit du signal numérisé: C >= n * Fe (bits/s)

### Sécurisation de la transmission 

- Taux d'erreur binaire (TEB) = (nb bits erronés) / (nb bits transmis)
- Soit n le nombre de bits du message, la probabilité de transmission sans erreur P = (1 - TEB)^n

**Clés de contrôle**

- Bit de parité : VRC, efficacité 50 - 60%
- Caractère de parité : LRC, efficacité 95%
- Combinaison VRC/LRC
- Envoi de la même trame en plusieurs exemplaires
- Clés de contrôle de 2 à 4 octets, CRC (Cyclic Redundancy Check), efficacité 100%

### Eficacité

- Taux de transfert des informations, TTI = (nb de bits utiles) / (durée de transmission)
- Rendement du support = TTI / Débit nominal du support

### Exemple : Taux de transfert des informations

```
Message 1000 caractère aCSII avec 1 bit VRC/caractère, liaison 9600 bits/s, TEB = 10^-4
P = (1 - 10^-4)^8*1000 = 44%

Templs de transmission sans tenir compte des erreurs t = 8000/9600 = 0.83s
TTI = 7000/0.83 = 8403 bits/s

Temps de transmission avec erreurs : tc = t/P = 0.83/0.44 = 1.89s
TTIe = 7000/1.89 = 3696  bits/s

Rendement = 3696/9600 = 38%
```

**Compression**

- Image, JPEG, MPEG1, MPEG2
- Texte, codage de la longueur de ligne, codage de huffman

### Exemple : Codage par plage RLE, RLC (Run Length Encoding/Coding)

```
223 223 223 214 214 214 214 223 223 223 217 214 214 214 = 14 * 8 bits = 112 bits
  {3,223}       {4,214}       {3,223} {1,217} {3,214}   = 5*(8+2) = 50 bits

Est interressant quand il y a de la redondance d'affiler
```

**Codage de Huffman**

- Codage à longueur variable
- Symboles très frequent -> codes courts
- Symboles rares -> codes longs
- Un code ne doit jamais être le début d'un autre code

1. Trier les symboles par probabilité décroissante
2. Regrouper les 2 symboles qui ont les probabilités les plus faibles
3. Remplacer ces symboles par un symbole : somme des probabilité des caractères
4. Tant que tout n'est pas regroupé : rentour en 2
5. En partant du bas et remontant jusqu'au bout
- à gauche : ajouer un 0
- à droite : ajouter un 1

## III) Le réseau de télécommunication

**Trafic téléphonique**

- Soit N(t) le nombre de circuits occupés à l'instant t, le volume de trafic pendant un temps T: V(t) = integrale[0,T] N(t) dt en secondes
- Intensité du trafic (en erlang ou %): I(T) = 1/T V(t)

```
1 erlang = 1 E -> 1 ligne pendant 1 heure
               -> 2 ligne pendant 1/2 heure
```

Circuit:
```
 🢑
1┃━━━ ━ ━━━
2┃ ━━━ ━━  ━━━
3┃━━━ ━━   ━━
4┃ ━━━  ━━━
5┃   ━━━━━
 ┗━━━━━━━━━━━━━━🢒

1) 5 circuits : inutile
2) 4 circuits : les 12 communication sont établies
3) 3 circuits : 2 sessions sont perdues
```

Compromis : 3 paramètres

- Trafic
- Capacité du faisceau : n
- Probabilité de perte

Probabilité de pertes:
```
p = (E^n / n!) / (somme[h = 0, n] E^h / h!)
```

Ex: 2 faisceaux de 10 circuits
-> trafic de 5 erlangs
-> p = 2%

Rendement = 0,5 E/circuit

Regroupe les é faisceaux
-> 1 faisceau de 20 circuits
-> p = 2%
-> trafic de 13 Erlangs

Rendement = 0.65 E/circuit

## IV) Réseau

**Exemple**

Débit de 9600 bits/s message de 300 octets 3 liaisons : L = 3
liaison = 0.25s
transit = 0.75s

message en 3 paquets
T un paquet-liaison = 0.83s
100 octets
Transit = 0.417s


# ICI

## Traitement Linéaire Des Signaux

### I. Signal

**1. Qu'est-ce ?**

- Transfert d'information
- Support :
	- Courant electrique
	- Champs electro-magnetiques
	- Pression acoustique
	- Variation du Support

**Définition** : Une information qui est codé sous la forme d'une variation d'une grandeur intensive (mesurable) en fonction d'une grandeur extensive (que l'on peut indexer).

**2. Signaux élémentaires**

- **Continu** : un signal pour lequel on possède toutes les données (physique).
- **Analogique** : La seule différence avec le signal continu est la manière dont on traite ce signal.
- **Echantilloné** : Transformation des signaux en valeures entières (signaux numeriques).

**3. Modèles mathématiques**

(schema)

**4. Fourier**

![equation][1]
[1]: https://latex.codecogs.com/svg.latex?\Large&space;s(t)=\sum_{i}^nAi.sin(w_it+q_i)

![equation][2]
[2]: https://latex.codecogs.com/svg.latex?\Large&space;\int&space;s(t).sin(w_0t)dt=B\int&space;sin^2(w_0t)dt

![equation][3]
[3]: https://latex.codecogs.com/svg.latex?\Large&space;\int_0^{\infty}s(t)e^{-iwt}dt

**5. Transformation de Fourier**

- **Linéarité** : TF { f(t) + g(t) } = TF { f(t)} + TF{g(t) }
- Soit λ ∈ ℝ : TF { λ.f(t) } = λ.TF { f(t) }

### II. Systèmes Linéaires

**1. Qu'est ce ?**

- Un systeme est lineaire quand on peut utiliser le principe de superposition :
	- e₁(t) → s₁(t)
	- e₂(t) → s₂(t)
	- e₃(t) = e₁(t) + e₂(t) → s₃(t) = s₁(t) + s₂(t)

**2. Systèmes**

![equation][4]
[4]: https://latex.codecogs.com/svg.latex?\Large&space;\sum_{i=1}^{n}i^3=\frac{n(n+1)}{2}^2

A(w).e^(jϕ(w)) = F(jw) { (A(w)), (ϕ(w)) :} 

#### 3. Réponse impulsionnelle

```math
e(t) → ▩▩ → s(t)

L_p{S_λ(t)} = L_p{λ(j)}*F(p) = F(p)


F(p) → f(t) = L_p^-1{F(p)}

S(p) = F(p) * E(p) = L_p{(f*e)(t)} = L_p{S(t)}

S(t) = (f * e)(t)

f(t) = sortie du système linéaire quand il a en entré une impulsion de direct?
**réponse impulsionnelle du système**

```

#### 4. Fonctions de transfert

```math
F(p) = (b_0 + b_1 * p + ... + b_m * p^m) / (a_0 + a_1 * p + ... + a_m * p^n) * (e^(λp))
```

si le système est  **n ≥ m**.

#### 5. Rapport entre fonction de transfert et equations différencielle

```math
F(p) = (3 + 2p - 3p²) / (1 - p²)

S(p) = F(p) * E(p) = (3 + 2p - 3p²) / (1 - p²) * E(p) 

(1 - p²)*S(p) = (3 + 2p - 3p²)*E(p)
(1 - p²)*L_p{S(t)} = (3 + 2p - 3p²)*L_p{E(t)}

L_p{S(t)} - p³*L_p{S(t)} = 3*L_p{e(t)} + 2p*L_p{e(t)} - 3p²*L_p{e(t)}

L_p{S(t)} - L_p{S'''(t)} = 3*L_p{e(t)} + 2*L_p{e'(t)} - 3*L_p{e'(t)}
L_p{S(t) - S'''(t)} = L_p{3*e(t) + 2*e'(t) - 3*e'(t)}

E.D. ≡ S(t) - S'''(t) = 3e(t) + 2e'(t) + 2e'(t)
```

#### 6. Representation graphique

##### Courbe de bode

(voir schemas)

#### 7. Filtres

```math
e(t) = x(t) + ubrace(b(t))_bruit

S(t) = x(t)

x(t) + b(t) → ▩▩ → x'(t) + a(t)
```

##### Filtre passe bas

##### Filtre passe haut

##### Filtre passe bande

##### Filtre coupe bande

##### Gabarits de filtre

###### Butterworth

Le plus utiliser pour filtrer la musique.

F(p) = (N(p) = 1) / D(p)

n | D(p)                | N(p)
--|---------------------|-----
1 | p+1                 | 1
2 | p²+√2 * p + 1       | 1
3 | (p² + p + 1)(p + 1) | 1
4 |                     | 1

F_(p)^2 = 1/(p+1) → f_(wc)^1 = 1/(p/(wc) + 1) = wc/(p+wc)
F_(p)^2 = 1/(p²+√2 * p + 1)

f_w1^1(p) = w1/(p+w1)
f_w2^1(p) = w2/(p+w2)

F_(w1, w2)(p) = F'_(w2)(p)(1-F'_(w1)(p))
= w2/(p+w2) *(1 - W1 /(p+w1))
= (w2 * p)/((p+w2)*(p+w1))

###### Tchebichev

Meilleure coupure que butterworth mais ne respecte pas le signal.

###### Bessel

Dephasé par rapprot au signal.

### II. Signaux numérique

#### 1. Signal numérique

Série (suite) de nombres entiers x_1, x_2, ... , x_n ∈ ℕ qui représente un echantillonage d'un signal.

#### 2. Repliement du spectre

#### 3. Stratégie anti-repliement

Si T est la periode d'échantillonage on ne peut échantilloner sans repliement de spectre que des signaux dont le plan haute fréquence < fe/2, haute pulsation < we/2.

#### 4. Reconstruction

**Interpolation** : reconstruction du signal continu a partir de l'echantillon.

L_p^-1{H(p)} = h(+)

s(t) = (h * x^λ)(t)

#### 5. Fonction de transfert echantillonné

e^(-Tp) = z^(-1) ≡ retard d'un echantillon

F(z) = b_0 + b_1

$$
E(\mathbf{v}, \mathbf{h}) = -\sum_{i,j}w_{ij}v_i h_j - \sum_i b_i v_i - \sum_j c_j h_j
$$

\[3 < 4\]

\begin{align}
    p(v_i=1|\mathbf{h}) & = \sigma\left(\sum_j w_{ij}h_j + b_i\right) \\
    p(h_j=1|\mathbf{v}) & = \sigma\left(\sum_i w_{ij}v_i + c_j\right)
\end{align}

fczaefdza

### Convolution numerique

```
signal echantillonée Xk et Yk

Xk -> [Hk] -> Yk

transformé en Z: Z{Yk} = Z{Hk}.Z{Xk}
Y(z) = H(z) . Y(z)

H(z) = (b0 + b1*Z^-1 + b2*Z^-2 + ... + bm*Z^-n) / (a0 + a1*Z^-1 + a2*Z^-2 + ... + an*Z^-n)

algorithme réactif

H(z) = (1 + 2*z^-1) / (2 - 3*z^-2)

Y(z) = H(z).X(z) = ((1 + 2*z^-1) / (2 - 3*z^-2)).X(z)  

(2 - 3*z^-2).Y(z) = (1 + 2*z^-1).X(z)

2*Y(z) - 3*z^-2.Y(z) = X(z) + 2*z^-2.X(z)

2Z{y_k} - 3Z{y_(k-2)} = Z{x_k} + 2Z{x_(k+1)}
Z{2y_k-3y_(k-2)} = Z{x_k} + 2x_(k-1)
2y_k - 3y_(k-2) = x_k + 2x_(k-1)

convolution numerique -> y_n = sum_(k=0)^(oo) h_k . x_(k-n)

**filtre frequentiel**

p -> 1 / (p+1)

{B(p) w_c} -> B^w_c(p) = B(p/w_c) = 1/((p/w_c)+1) = w_c/(p+w_c)

**transformation conforme**

p = t(z) = (2/T) * (z - 1)/(z + 1) = (2/T) * (1 - z^-1)/(1 + z^-1)
```

### Comparaison des signaux numériques

#### 1. Distance

on utilise le rapport signal sur bruit

d_2(x, y) = 1/N sum_(k=0)^(N-1) (x_k - 2xk)^2 = 1 / N sum_(k=0)^(N-1) (x_k)^2

#### 2. Corrélation de deux signaux

|Corr(x, y) = 1 / N sum_(k=0)^(N-1) x_k.y_k| <= rac(Corr(x,x).Corr(y.y))

**Correlation de Pearson**

P(x,y) = Corr(x, y) / rac(Corr(x,x).Corr(y.y)) ∈ [-1, 1]

Y(z) / X(z) = H(z) = ((2z^-1 + 1) / (3 + z^-1 - 2z^-2)) -> algo?

y_k = (1 / 3) . (-y_(k-1) + 2y_(k-2) + 2x_(k-1) + x_k)

x_k -> [] -> y_k

h = [0,5 ; 0 ; -0,3]

4 premiers echantillons:

y_0 = 0,5.x_0
y_1 = 0,5.x_1
y_2 = 0,5.x_2 - 0,3.x_0
y_3 = 0,5.x_3 - 0,3.x_1

h_0 = y_0 = (1 / 3) . (1) = (1 / 3) -> h_0
h_1 = y_1 = (1 / 3) . (-(1 / 3) + 2) = (1 / 3) . (5 / 3) = (5 / 9) -> h_1
h_2 = y_2 = (1 / 3) . (-(5 / 9) + (2 / 3)) = (1 / 3) . ((-3 + 6) / 9) = (1 / 27) -> h_2

### Bruits

### Conclusion

# FIN



# Memo partie transmission signal

# TD1 - codage/decodage/transmission d'un signal avec verrification d'erreur

**terme**
	- bit parité : bit à 1 si message pair 0 sinon
	- LRC (longitudinal redundancy check) : bit protegean la parité de chaque caractère de meme rang
	- VRC (vertical redundancy check): bit protegeant tout les bit d'un caractère y compris le bit LRC 
	- CRC (Cyclic Redundancy Check) : le reste d'un division des bits de données, supposés etre le coefficient d'un polynome par un polynome generateur

- (Ex1) codage LRC
- (Ex1) verification d'erreur/ correction
- (Ex2) decodage
- (Ex2) correction erreur
- (Ex3) codage CRC
- (Ex4) representation signal en bande passante
	- NRZ
	- biphase
	- biphase différenciel
	- Miller
	- bipolaire simple
- (Ex5) resumé (memo)

# TD2 - transmission d'une signal appliqué aux image

**terme**
	- bande passante (Hz) :
	- rapport signal à bruit (dB = S/B) :
	- capacité de transmission = B . log_2(1 + S/N)
	- taux d'erreur binaire (Te = (nb_bits_erronnés/nb_bits_transmis)) :
	- taux de transfert d'information (TTI = nb_bits_utiles / durée_transmission) :
	- probabilité de transmission d'erreur (Pe = (1 -Te)^n avec n = nb_bits_msg)  
	
- (Ex1) capacité de transmission d'un canal
- (Ex2) probabilité d'erreur d'un message
- (EX3) TTI sans erreur/ avec erreur
- (Ex4) temps de transmission
- (Ex5) rapport signal bruit

# TD3 - compression d'image et multiplexeur

**terme**
	codage huffman :
	debit binaire :
	multiplexeur : permet de repartir la bande passante

- (Ex1) codage huffman
- (Ex2) transmission image comprimé
- (Ex3) debit binaire pour transmettre image
- (Ex4) multiplexeur
- (Ex5) multiplexeur
- (EX6) meme que Ex5 du td1

# TD4 - trafics téléphoniques

**terme** :
	- Erlangs (E) : 
	- Circuit : 
	- Faisceaux :

- (Ex1) nombre abonnés desservis
- (Ex2) efficacité traffic
- (Ex2) niveau de traffic
- (Ex3) nombre ligne selon charge
- (Ex4) resumé (memo)

# TD5 - gestion de reseaux TCP/IP

**terme** : 
	- fenetrage : reception d'accusé de reception ACK de plusieurs element d'information

- (Ex1) efficacité liaisons
- (Ex2) fenêtrage
- (Ex3) communication de messages/paquets

# Memo partie traitement signal

Dans le partiel :

Definir en traitement du signal :
- une impulsion :
- un echelon :
- un diagramme de Bode :

Donnez : 
- forme analytique d'un signal donné
- techniques différente permettant d'obtenir un même filtrage
- différentes familles de filtres fréquentiels
- différences Laplace vs Fourier
- algorithme recursif d'un filtre numérique. est-il causal ? 

Dessinez :
- qualitativement le diagramme de Bode d'une fonction de transfert F(p)

Question:
- pourquoi dit-on transformé de Laplace linéraire ?
- si y(t) est la sortie du filtre de fonction de tranfert F(p), quel est le signal x(t) en entrée de ce filtre ?

# AUTRE


# Exam

## Transmission

> Je dois réussir à faire les exos machinalement (memos)

- Transmission d'une image à partir d'un scanner #2018-2019 #2016-2017
- Tables de routage #2018-2019 #2016-2017
- Transmission d'un image TV #2018-2019 #2016-2017
- Transmission d'une image compirlée (huffman)

pixel → cm = px * 2.54 / dpi

cm → pixel = cm * dpi / 2.54

## Traitement

> Je dois apprendre les définition et poser quelques questions à raph demain

- Savoir différencier et definir les signaux (numeriques/analogiques) * 2
- Donner la relation entre les deux * 2
- Definir echelon
- Definir système lineaire
- Donner différentes fammilles de filtres fréquencielles
- Definir transformation de fourier / discrète * 2
- Definir transformé de Laplace
	- toujours la meme chose on applique un transformation a notre systeme qui nous permet de le resoudre dans un autre espace puis on le fait repasser dasn l'espace originale pour l'utiliser.
- Différences Laplace/Fourier
	- Laplace est un cas particuler de la fonction de fourier
- Definir reponse impulsionnelle * 2
- Donnez la fonction de transferts d'une equation
- Definir repliement de spectre
- Definir filtre causal
- Difference filtrage donmaine fréquentiel / temporel ? Comment s'applique chaque ?