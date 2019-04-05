# Traitement du signal - E2018-2019 (Correction)

## Traitement

**1)** l'amplitude d'un signal continu est défini de manière continu dans le temps alors que pour un signal numérique elle est défini de manière discrete dans le temps.

**2)** Un echantillonnage et une mesure de l'amplitude du signal à intervalle de temps réguliers, qui permet d'approximer le signal et de le numeriser à l'aide d'une résolution, cepandant un signal échantilloné n'est pas forcément un signal numérique. 

**3)** ???

**4)** Une transformation de Fourier est un transformation qui permet de faire passé un signal définis sur le spectre du temps vers un signal défini sur le spectre des fréquences. La transformation de fourier discrète accompli la même tache pour les signaux numériques.

**5)** La transformation de Fourier discrète d'un signal discret est également discrète car elle ne fait qu'appliquer un transformation à chaque elements de se signal discret, ce qui nous donne un resultat discret.

**6)** Le repliement de spectre ce produit quand un signal numerique reconstruit à partir d'un signal analogique est différent de ce dernier. On peut l'eviter en appliquant le theorème de shanon qui stipule que pour obtenir un échantillonage correcte d'un signal analogique, la fréquence d'échantillonnage du signal numerique doit être strictement supérieure à deux fois la fréquence maximale du signal analogique.

**7)** La Fast-Fourier Transform est un algorithme plus rapide de Transformé de Fourier discrète O(n.log(n)) contre O(n²) cepandant la taille du signal d'entré doit être un multiple de 2.

**8)** ??? un système qui obeit au règle suivant

**9)** ???

**10)** ???

**11)** On appelle cela une réponse impulsionnelle, 
???

**12)** On devrait choisir un fréquence d'échantillonnage qui permettrait de representait un signal continu cadencé a 20 kHz sans repliement de spectre donc avec un échantillonnage double soit 44,1 kHz serai la fréquence idéal.

**13)** Pour réaliser un filtrage dans un domaine fréquenciel il faut avoir un échantillon de taille finit alors que cela n'est pas necessaire dans le domaine temporel.
???

**14)** ???

## Transmission

**