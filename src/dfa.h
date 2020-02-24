#ifndef DFA_H
#define DFA_H

#include "alphabet.h"
#include "nfa.h"

/*
 * liste chaînée ordonnée d'états NFA, caractérisant un état DFA.
 */
typedef struct lststates {
    STATE *s;
    struct lststates *suiv;
}LSTSTATES;

/*
 * état d'un automate DFA
 * lst_states est la liste ordonnée des états NFA
 * num est le numéro de l'état
 * trans est un tableau de pointeurs vers les états DFA accessibles
 *     depuis l'état courant
 * accept indique si l'état est acceptant ou non
 * suiv est un pointeur sur l'état suivant à traiter
 */
typedef struct dstate {
    LSTSTATES *lst_states;
    int num;
    int trans[ALPHABET_LEN];
    unsigned char accept;
    struct dstate *suiv;
} DSTATE;

/*
 * Construit un DFA en partant du NFA nfa, et renvoie son état initial.
 */
DSTATE nfa2dfa(NFA nfa);

/*
 * Écrit dans le fichier fd les consignes pour dessiner l'automate avec dot.
 */
void dfa2file(DSTATE dfa_head, char *name);

#endif
