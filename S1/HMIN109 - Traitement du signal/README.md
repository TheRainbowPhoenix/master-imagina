william.puech@lirmm.fr / www.lirmm.fr/~wpuech/

Evaluation : 0.67 EXAM + 0.33 TP (note de TP definitive)

# SOMMAIRE

1. [Les réseaux](liens)
2. [Concepts des télécommunications](liens)
3. [Le réseau de télécommunication](liens)
4. [Compression](liens)
5. [Cryptage](liens)

# LES RÉSEAUX

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

l'information reçue doit être identique à l'information émise

Le signale peut être modifié, des bits ou octets perdus durant le transfert de l'information : erreurs

Il faut les détecter et les corriger.

- Detection d'une modification
- l'émetteur rajoute des bits, fonction des données qu'il transmet
- Le récepteur recalcule la fonction et vérifie
