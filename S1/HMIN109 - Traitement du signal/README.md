william.puech@lirmm.fr / www.lirmm.fr/~wpuech/

Evaluation : 0.67 EXAM + 0.33 TP (note de TP definitive)

# SOMMAIRE

1. [Les réseaux](#les%20reseaux)
2. [Concepts des télécommunications](#concepts%20des%20télécommunications)
3. [Le réseau de télécommunication](liens)
4. [Compression](liens)
5. [Cryptage](liens)

# I) LES RÉSEAUX

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

# II) CONCEPTS DES TÉLÉCOMMUNCATIONS

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

