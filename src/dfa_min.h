#ifndef H_DFA_MIN
#define H_DFA_MIN

#include "dfa.h"

/*
 * un dfa minimal est codé par
 *    son nombre d'états
 *    sa table de transitions
 */
typedef struct {
    int nb_states;
    int **trans;
}DFAMIN;

/*
 * Construit et renvoie un DFA minimal en partant du DFA dfa.
 */
DFAMIN dfa2min(DFA dfa);

#endif
