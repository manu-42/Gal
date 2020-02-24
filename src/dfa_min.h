#ifndef H_DFA_MIN
#define H_DFA_MIN

#include "dfa.h"

/*
 * état d'un automate DFA minimal
 * num est le numéro de l'état
 * trans est un tableau de pointeurs vers les états DFA accessibles
 *     depuis l'état courant
 * grp_trans est un tableau de pointeurs vers les groupes d'états DFA
 *     accessibles depuis l'état courant ?????
 * accept indique si l'état est acceptant ou non
 * suiv est un pointeur sur l'état suivant à traiter ???? possible d'éliminer ???
 */
typedef struct dminstate {
    int num;
    int trans[ALPHABET_LEN];
    int grp_trans[ALPHABET_LEN];
    unsigned char accept;
    struct dminstate *suiv;
} DMINSTATE;

/*
 * Construit un DFA minimal en partant du DFA dfa, et renvoie son état
 * initial.
 */
DMINSTATE dfa2min(DSTATE dfa);

#endif
