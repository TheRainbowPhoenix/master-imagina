#ifndef CALC_H
#define CALC_H

typedef struct calc_msg {
    long mtype;
    char symb;
    int x;
    int y;
} calc_msg;

typedef struct eval_msg {
    long mtype;
    int res;
} eval_msg;

#endif