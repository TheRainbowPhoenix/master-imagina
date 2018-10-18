# HMIN107 - Intelligence Artificielle

##  Information et Ressources

- Examen : 0.50 * EXAM + 0.50 * CC
- Ressources : Moodle

## Definitions

**Probleme :** Collection d'information que l'agent utilise pour décider quelle(s) action(s) accomplir

**Definition d'un Problème :**

- Identification d'un **état initial** (choix d'un langage de description d'états du problème)
- Identification des **actions possibles** par définition d'opérateurs de changements d'état (definition de l'ensemble des états possibles du problème).

**Espace des états/de recherche d'un problème :** Ensemble des états atteignables depuis l'état initial par n'importe quelle séquence d'actions (representable par un graphe orienté).

**Solution d'un Problème :** séquence d'actions permettant de passer de l'état initial vers un état but (chemin orienté).

## Formalisation d'un problème

| Type    | Composants              | Opérations                                    |
|         |                         |                                               |
| Problem | InitialState, Operators | GoalTest(State), PathCost(SequenceOfOperator) |

**Problèmes à états simple :**

- On connaît l’état dans lequel on est
- On connaît précisément l’effet des actions
- On peut calculer à tout moment l’état dans lequel on se trouvera après une action

**Problèmes à états multiples :**

- On ne sait pas exactement dans quel état on se trouve (seulement un ensemble d’états possibles)
- On ne connaît pas précisément l’effet des actions
- On ne peut que caractériser par un ensemble d’états la situation où l’on est