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
 * état d'un DFA
 * lst_states est la liste ordonnée des états du NFA
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
 * un dfa est codé par
 *    son état initial head
 *    son nombre d'états nb_states
 */
typedef struct {
    DSTATE *head;
    int nb_states;
}DFA;

/*
 * Construit et renvoie un DFA en partant du NFA nfa.
 */
DFA nfa2dfa(NFA nfa);

/*
 * Créée un fichier nommé `name` et y écrit les consignes pour dessiner
 * le dfa avec dot.
 */
void dfa2file(DFA dfa, char *name);

#endif
