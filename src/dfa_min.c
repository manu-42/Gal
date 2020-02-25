#include <stdio.h>
#include <stdlib.h>
#include "alphabet.h"
#include "dfa_min.h"

/*
 * Un groupe est composé de :
 *     num : son numéro
 *     nb_states : son nombre d'états (au maximum égal au nombre d'états
 *                 du nfa initial)
 *     lst_states : la liste de ses états
 *     trans : le tableau des transitions vers les autres groupes
 *     accept : indique si l'état est acceptant ou non
 */
typedef struct {
    int num;
    int nb_states;
    int *lst_states;
    int* trans;
    unsigned char accept;
}GROUPE;

/*
 * créée et renvoie un nouveau groupe en initialisant son numéro `num`
 * et le nombre `nb_states` maximum d'états qu'il peut contenir.
 */
GROUPE *create_grp(int num, int nb_states) {
    GROUPE *g = malloc(sizeof(GROUPE));
    g->num = num;
    g->nb_states = 0;
    g->lst_states = malloc(sizeof(int) * nb_states);
    g->accept = 0;
    return g;
}

/*
 * ajoute l'état `num_state` à la liste d'états du groupe pointé par `g`
 * et actualise le champs `accept` du groupe si l'état est acceptant.
 */
void add_state2grp(int num_state, GROUPE *g, unsigned char accept) {
    g->lst_states[(g->nb_states)++] = num_state;
    if (accept) g->accept = 1;
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
        for (int i=0; i<ALPHABET_LEN; i++) printf(" %d", (g->trans)[i]);
    printf("\n");
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
 * `trans`. Si aucun ne correspond, renvoie -1.
 * `start_grp` est le numéro du premier groupe à tester, `nb` le nombre
 *  de groupes à tester, et `lst_grp` la liste de groupe.
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
 * libère les pointeurs de la liste de groupes `lst_grp` de longueur `len`
 */
void free_grp(GROUPE **lst_grp, int len) {
    for (int i=0; i<len; i++) free(lst_grp[i]);
}

/*
 * Détermine les champs de l'automate minimal correspondant à la liste
 * de groupes finale. nb_grp est la longuer de cette liste.
 */
DFAMIN calc_dfamin(GROUPE **lst_grp, int nb_grp) {
    DFAMIN dfamin;
    dfamin.nb_states = nb_grp;
    dfamin.lst_accept = malloc(sizeof(unsigned char) * nb_grp);
    int **trans = malloc(sizeof(int*) * nb_grp);
    for (int i_grp=0; i_grp<nb_grp; i_grp++) {
        (dfamin.lst_accept)[i_grp] = lst_grp[i_grp]->accept;
        trans[i_grp] = malloc(sizeof(int) * ALPHABET_LEN);
        for (int i_char=0; i_char<ALPHABET_LEN; i_char++) {
            trans[i_grp][i_char] = (lst_grp[i_grp]->trans)[i_char];
        }
    }
    dfamin.trans = trans;
    // recherche de l'état initial (c'était l'état 1 dans le dfa initial
    dfamin.init_state = -1;
    int i_grp = 0;
    while (dfamin.init_state == -1) {
        for (int i_state=0; i_state<lst_grp[i_grp]->nb_states; i_state++) {
            if ((lst_grp[i_grp]->lst_states)[i_state] == 1) {
                dfamin.init_state = i_grp;
                break;
            }
        }
        i_grp++;
    }
    return dfamin;
}

/*
 * Renvoie le tableau de transitions associé au dfa initial.
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
 * Construit et renvoie un DFA minimal en partant du DFA dfa.
 */
DFAMIN dfa2min(DFA dfa) {
    int **trans = init_trans(dfa);
    const int NBSTATES = dfa.nb_states;
    // tableau indiquant quels états sont acceptants
    unsigned char accept[NBSTATES];
    // liste d'entiers indiquant pour chaque état du dfa initial le
    // numéro de son groupe
    int *pi = malloc(sizeof(int) * NBSTATES);
    // création des deux groupes initiaux
    int nb_grp = 2;
    GROUPE **lst_grp = malloc(sizeof(GROUPE*) * nb_grp);
    for (int i_grp=0; i_grp<nb_grp; i_grp++) {
        lst_grp[i_grp] = create_grp(i_grp, NBSTATES);
    }
    // allocation de chaque état à son groupe
    for (DSTATE *s=dfa.puits; s!=NULL; s=s->suiv) {
        accept[s->num] = s->accept;
        int i_grp =  s->accept ? 1 : 0;
        pi[s->num] = i_grp;
        add_state2grp(s->num, lst_grp[i_grp], s->accept);
    }
    // creation de pi_new
    int nb_grp_new;
    int *pi_new = malloc(sizeof(int) * NBSTATES);
    GROUPE **lst_grp_new = malloc(sizeof(GROUPE*) * NBSTATES);
    unsigned char finished = 0;
    while (! finished) {
        nb_grp_new = 0;
        lst_grp_new = malloc(sizeof(GROUPE*) * NBSTATES);
        // pour chaque groupe g de pi
        for (int i_grp=0; i_grp<nb_grp; i_grp++) {
            GROUPE *g = lst_grp[i_grp];
            int nb_deb = nb_grp_new, nb_sous_grp = 0;
            // distinguer si possible les états de g
            for (int i_state=0; i_state<(g->nb_states); i_state++) {
                int num_state = (g->lst_states)[i_state];
                int *grp_trans = calc_grp_trans(num_state, trans, pi);
                int num = num_grp(grp_trans, nb_deb, nb_sous_grp, lst_grp_new);
                if (num != -1) { // état indistinguable d'un précédent
                    free(grp_trans);
                }
                else { // création d'un nouveau groupe
                    num = nb_grp_new;
                    lst_grp_new[nb_grp_new] = create_grp(nb_grp_new, NBSTATES);
                    (lst_grp_new[nb_grp_new])->trans = grp_trans;
                    nb_grp_new++;
                    nb_sous_grp++;
                }
                // ajout de l'état num_state à son groupe
                add_state2grp(num_state, lst_grp_new[num], accept[num_state]);
                pi_new[num_state] = num;
            }
        }
        finished = (nb_grp_new == nb_grp); // pi == pi_new ?
        free_grp(lst_grp, nb_grp);
        free(lst_grp);
        // pi (ou pi_final) = pi_new
        nb_grp = nb_grp_new;
        pi = pi_new;
        lst_grp=lst_grp_new;
    }
    return calc_dfamin(lst_grp, nb_grp);
}

/*
 * Créée un fichier nommé `name` et y écrit les consignes pour dessiner
 * le dfa minimal avec dot.
 */
void dfamin2file(DFAMIN dfa, char *name) {
    FILE *fd = fopen(name, "w");
    if (fd == NULL) {
        fprintf(stderr, "Erreur à la création du %s.\n", name);
        perror("dfamin2file");
        exit(EXIT_FAILURE);
    }
    fprintf(fd, "digraph T {\n");
    fprintf(fd, "node [shape=circle];\n");
    fprintf(fd, "\"\" [shape=none]\n");
    fprintf(fd, "\"\" -> %d\n", dfa.init_state); // État initial
    for (int i_state=0; i_state<dfa.nb_states; i_state++) {
        if ((dfa.lst_accept)[i_state])
            fprintf(fd, "%d [shape=doublecircle];\n", i_state);
        for (char ch=next_letter(0); ch != -1; ch=next_letter(ch)) {
            int i_char = letter_rank(ch);
            // on n'affiche pas les transitions vers l'état puits
            if ((dfa.trans)[i_state][i_char] != 0) {
                fprintf(fd, "%d -> %d [label=%c]\n",
                        i_state, (dfa.trans)[i_state][i_char], ch);
            }
        }
    }
    fprintf(fd, "}\n");
    fclose(fd);
}
