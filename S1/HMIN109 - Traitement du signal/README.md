william.puech@lirmm.fr / www.lirmm.fr/~wpuech/

Evaluation : 0.67 EXAM + 0.33 TP (note de TP definitive)

# SOMMAIRE

1. [Les réseaux](#reseaux)
2. [Concepts des télécommunications](#concepts%20des%20télécommunications)
3. [Le réseau de télécommunication](liens)
4. [Compression](liens)
5. [Cryptage](liens)

# RÉSEAUX

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

**(EXEMPLE) Bit de parité**

On résonne sur les bits à 0

0 1 2 3 4 5 6 7
0 0 1 1 0 1 0|1 (bit de parité)

Ici on compte les bits à 0 et on ne rajoute pas de 0 sur le bit de parité pour avoir un nombre de 0 pair.

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

# CONCEPTS DES TÉLÉCOMMUNCATIONS


**Forme**

- texte
- musique
- images fixes

**Codage**

- Adaptation au canal de communication
- Capteurs et transducteurs

**Codage en bande de base**

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
