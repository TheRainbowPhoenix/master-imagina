# TD4

## Exercice 3

### Question 1

```
A <-> (C v E) \eq (!A v C v E) ^ (!C v !A) v (!E v A)
                        c1           c2          c3

B ^ F -> !C \eq (!B v !F v !C) 
                      c4

E -> C \eq (!E v C)
               c5

C -> F \eq (!C v F)
               c6

C -> B \eq (!C v B)
               c7
```

### Question 2

```
{c1, c2, c3, c4, c5, c6, c7}
|
| E <- vrai
v  
{c2, {A}, c4, {C}, c6, c7}
|
| PU
v
{{A}, {!B, !F}, {F}, {B}}
|
| PU
v
{{!B, !F}, {F}, {B}}
|
| PU
v
{{!B}, {B}}
|
| PU
v
{!0}

////////////////////////////

{c1, c2, c3, c4, c5, c6, c7}
|
| E <- faux
v  
{{!A, C}, c2, c4, c6, c7}
|
| C <- vrai
v
{{A}, {!B, !F}, {F}, {B}}
|
| PU
v
{{!B, !F}, {F}, {B}}
|
| PU
v
{{!B}, {B}}
|
| PU
v
{!0}

////////////////////////////

{{!A, C}, c2, c4, c6, c7}
|
| C <- faux
v
{{!A}}
|
| PU
v
{} Vrai !!!
```

### Question 3

```
{c1, c2, c3, c4, c5, c6, c7}
|
| C <- faux
v
{c1, c2, c3, c4, c5, c6, {!C}}
|
| PU
v
{{!A, E}, {!E, A}, {!E}}
|
| PU
v
{{!A}}
|
| C <- faux
v
{} Vrai !!!
#999999
```

### Exercice 5

```
{{~~!B~~, !D, !E, F}, {!G, !D, A}, {**!C**, !F, A}, {~~!B~~, X}, {!X, !A, ~H~}, {**!C**, D}, {!X, **!**C, A}, {!X, ~~!B~~, D}, {~~B~~}, {**C**}, {~!H~}}
|
| B <- vrai
v
|
| C <- vrai
v
|
| H <- faux
v
|
| X <- vrai
v
|
| A <- faux
v
|
| D <- vrai
v
|
| F <- faux
v
|
| G <- faux
v
|
| E <- faux
v
```