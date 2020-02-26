#ifndef DFA_MIN_H
#define DFA_MIN_H

#include "dfa.h"

/*
 * un dfa minimal est codé par
 *    son nombre d'états
 *    son état initial
 *    une liste indiquant pour chaque état s'il est acceptant ou non
 *    sa table de transitions
 */
typedef struct {
    int nb_states;
    int init_state;
    unsigned char *lst_accept;
    int **trans;
}DFAMIN;

/*
 * Construit et renvoie un DFA minimal en partant du DFA dfa.
 */
DFAMIN dfa2min(DFA dfa);

/*
 * Créée un fichier nommé `name` et y écrit les consignes pour dessiner
 * le dfa minimal associé à l'expression `exp` avec dot.
 */
void dfamin2file(DFAMIN dfa, char *name, char *exp);
#endif
