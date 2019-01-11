# TD5

## Exercice 1

### Question 1

Etape  | BF          | Règles applicables   | nouvFait
------ | ----        | -------------------- | ---------
0      |             |                      | S,A
1      | S,A         | R2                   | T
2      | T,A,S       | R5                   | U
3      | S,A,U,T     | R6, R7               | R
4      | R,A,T,U,S   | R3                   | E
5      | S,A,T,U,R,E | FIN                  |

R1 | F
R2 | V
R3 | V
R4 | F
R5 | V
R6 | V
R7 | V

   |cpt| H1 | A | B | C | E | R | S | T | U | Conclusion
---|---|----|-|-|-|-|---|-|-|-|-|---|---|---|-----------
R1 | 2 | ---|-|-|>v-|---|-|-|>v |   |   |   | C
R2 | 1 | ---|>v |   |   | | | | |   |   |   | T
R3 | 2 | ---|---|---|---|-|-|>v-|---|---|>  | E
R4 | 2 |    |   |   |   | v |   |   |   |   | U 
R5 | 2 |    |   |   |   |   |   |   |   |   | U
R6 | 2 |    |   |   |   |   |   |   |   |   | R
R7 | 2 |    |   |   |   |   |   |   |   |   | A

A traiter:

1 | S,A
--|----
2 |
3 |
4 |
5 |
6 |
7 |