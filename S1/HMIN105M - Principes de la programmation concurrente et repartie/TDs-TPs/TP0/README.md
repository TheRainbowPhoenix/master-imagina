# EXERCICE 1

```c
int a = 10; 
int b = 25;
int* p = &b;
int* pp = &a;
```

**Question 1.1**
```
1. *(&(*(*(&p))))     -> la valeur de b
2. *(p-1)             -> la valeur de a
3. *(*(&p)-1)         -> la valeur de a
4. *(*(&pp)+1)        -> la valeur de b
5. *(&(*(*(&p))) - 1) -> la valeur de a
```