#ifndef NFA_H
#define NFA_H

#include "arbre.h"

enum {SPLIT, ACCEPT};

/*
 * état d'un automate NFA
 * num sert à numéroter l'état au moment de la génération
 *     du fichier pour dot.
 *     NOT_VISITED indique que le noeud n'a pas encore été
 *                 ajouté au fichier pour dot.
 * ch est l'étiquette de l'arc sortant, ou bien
 *     EPSILON pour une epsilon tansition
 *     SPLIT pour deux arcs sortants étiquetés par epsilon
 *     ACCEPT pour un état final
 */
typedef struct state {
    int num;
    char ch;
    struct state *suiv, *suiv2;
}STATE;

/* un nfa est codé par
 *    start : son état initial
 *    end   : son état acceptant
 */
typedef struct nfa {
    STATE *start, *end;
}NFA;

/*
 * Renvoie un NFA en partant de l'arbre syntaxique tree
 */
NFA tree2nfa(TREE tree);

/*
 * Créée un fichier nommé `name` et y écrit les consignes pour dessiner
 * l'automate nfa avec dot.
 */
void nfa2file(NFA nfa, char *name);

#endif
