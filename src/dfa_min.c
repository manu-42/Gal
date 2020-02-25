#include <stdio.h>
#include <stdlib.h>
#include "alphabet.h"
#include "dfa_min.h"

typedef struct {
    int num;
    int nb_states;
    int *lst_states;
    int* trans;
}GROUPE;

GROUPE *create_grp(int num, int nb_states) {
    GROUPE *g = malloc(sizeof(GROUPE));
    g->num = num;
    g->nb_states = 0;
    g->lst_states = malloc(sizeof(int) * nb_states);
    return g;
}

/*
 * ajoute l'état num_state à la liste d'états du groupe g.
 */
void add_state2grp(int num_state, GROUPE *g) {
    g->lst_states[(g->nb_states)++] = num_state;
}

/*
 * libère les pointeurs de la liste de groupes lst_grp de longeur len
 */
void free_grp(GROUPE **lst_grp, int len) {
    for (int i=0; i<len; i++) free(lst_grp[i]);
}

/*
 * affiche un groupe (débuggage)
 */
void print_grp(GROUPE *g) {
    printf("groupe %d (", g->num);
    for (int i=0; i<g->nb_states; i++) printf("%d ", (g->lst_states)[i]);
    printf(")\n");
    printf("Transitions vers les groupes");
    if (g->trans != NULL)
        for (int i=0; i<ALPHABET_LEN; i++) printf("%d ", (g->trans)[i]);
    printf("\n");
}

/*
 * Renvoie le tabelau de transitions associé au dfa.
 */
int **init_trans(DFA dfa) {
    int **trans = malloc(sizeof(int*) * dfa.nb_states);
    for (DSTATE *s=dfa.puits; s!=NULL; s=s->suiv) {
        trans[s->num] = malloc(sizeof(int) * ALPHABET_LEN);
        for (int i_char=0; i_char<ALPHABET_LEN; i_char++) {
            trans[s->num][i_char] = (s->trans)[i_char];
        }
    }
    return trans;
}

/*
 * Renvoie le tabelau de transitions associé à la liste de groupe de
 * longueur nb_grp.
 */
int **calc_trans_dfamin(GROUPE **lst_grp, int nb_grp) {
    int **trans = malloc(sizeof(int*) * nb_grp);
    for (int i_grp=0; i_grp<nb_grp; i_grp++) {
        trans[i_grp] = malloc(sizeof(int) * ALPHABET_LEN);
        for (int i_char=0; i_char<ALPHABET_LEN; i_char++) {
            trans[i_grp][i_char] = (lst_grp[i_grp]->trans)[i_char];
        }
    }
    return trans;
}

/*
 * Renvoie le tableau des numéros de groupe accessibles par des
 * transitions partant de l'état numéro num_state.
 */
int *calc_grp_trans(int num_state, int **trans, int *pi) {
    int *grp_trans = malloc(sizeof(int) * ALPHABET_LEN);
    for (int i_char=0; i_char<ALPHABET_LEN; i_char++) {
        grp_trans[i_char] = pi[trans[num_state][i_char]];
    }
    return grp_trans;
}

/*
 * Compare deux listes de transitions.
 */

unsigned char comp_trans(int *t1, int *t2) {
    for (int i_char=0; i_char<ALPHABET_LEN; i_char++) {
        if (t1[i_char] != t2[i_char]) return 0;
    }
    return 1;
}

/*
 * Renvoie le numéro d'un groupe correspondant au tableau de tansition
 * trans. Si aucun ne correspond, renvoie -1.
 * start_grp est le numéro du premier groupe à tester et nb le nombre
 * de groupes à tester.
 */
int num_grp(int* trans, int start_grp, int nb, GROUPE **lst_grp) {
    for (int i_grp=start_grp; i_grp<start_grp + nb; i_grp++) {
        if (comp_trans(trans, lst_grp[i_grp]->trans)) {
            return i_grp;
        }
    }
    return -1;
}

/*
 * Construit un DFA minimal en partant du DFA dfa, et renvoie ??????
 */
DFAMIN dfa2min(DFA dfa) {
    int **trans = init_trans(dfa);
    const int NBSTATES = dfa.nb_states;
    // initialisation de la partition pi
    int *pi = malloc(sizeof(int) * NBSTATES);
    int nb_grp = 2;
    GROUPE **lst_grp = malloc(sizeof(GROUPE*) * nb_grp);
    for (int i_grp=0; i_grp<nb_grp; i_grp++) {
        lst_grp[i_grp] = create_grp(i_grp, NBSTATES);
    }
    for (DSTATE *s=dfa.puits; s!=NULL; s=s->suiv) {
        int i_grp =  s->accept ? 1 : 0;
        pi[s->num] = i_grp;
        add_state2grp(s->num, lst_grp[i_grp]);
    }
    int nb_grp_new;
    int *pi_new = malloc(sizeof(int) * NBSTATES);
    GROUPE **lst_grp_new = malloc(sizeof(GROUPE*) * NBSTATES);
    unsigned char finished = 0;
    while (! finished) {
        printf("*******************************************\n");
        nb_grp_new = 0;
        lst_grp_new = malloc(sizeof(GROUPE*) * NBSTATES);
        for (int i_grp=0; i_grp<nb_grp; i_grp++) {
            GROUPE *g = lst_grp[i_grp];
            printf("Traitement du groupe ");
            print_grp(g);
            int nb_deb = nb_grp_new, nb_sous_grp = 0;
            for (int i_state=0; i_state<(g->nb_states); i_state++) {
                int num_state = (g->lst_states)[i_state];
                printf("état %d \n", num_state);
                int *grp_trans = calc_grp_trans(num_state, trans, pi);

                int num = num_grp(grp_trans, nb_deb, nb_sous_grp, lst_grp_new);
                if (num != -1) {
                    free(grp_trans);
                }
                else {
                    printf("Création du groupe %d\n", nb_grp_new);
                    num = nb_grp_new;
                    lst_grp_new[nb_grp_new] = create_grp(nb_grp_new, NBSTATES);
                    (lst_grp_new[nb_grp_new])->trans = grp_trans;
                    nb_grp_new++;
                    nb_sous_grp++;
                }

                add_state2grp(num_state, lst_grp_new[num]);
                printf("Ajout au ");
                print_grp(lst_grp_new[num]);
                pi_new[num_state] = num;
            }
        }
        finished = (nb_grp_new == nb_grp);
        nb_grp = nb_grp_new;
        pi = pi_new;
        free_grp(lst_grp, nb_grp);
        free(lst_grp);
        lst_grp=lst_grp_new;
    }
    DFAMIN dfamin;
    dfamin.nb_states = nb_grp;
    dfamin.trans = calc_trans_dfamin(lst_grp, nb_grp);
    return dfamin;
}
