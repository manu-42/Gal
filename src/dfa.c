#include <stdio.h>
#include <stdlib.h>
#include "dfa.h"

char tmp;

/*
 * Ajoute l'état s à la liste ordonnée d'états lst_states.
 * renvoie 0 si l'état était déjà présent, 1 sinon
 */
unsigned char add_state(LSTSTATES **lst_states_ptr, STATE *s) {
    LSTSTATES *new_state = malloc(sizeof(LSTSTATES));
    new_state->s = s;
    int num = s->num;
    LSTSTATES *lst_states = *lst_states_ptr;
    if (lst_states == NULL || (lst_states->s)->num > num) { // insertion
        new_state->suiv = lst_states;                       // en tête
        *lst_states_ptr = new_state;
        return 1;
    }
    if ((lst_states->s)->num == num) { // déjà présent en tête
        free(new_state);
        return 0;
    }
    LSTSTATES *suiv = lst_states->suiv;
    while (suiv != NULL && (suiv->s)->num < num) {
        lst_states = suiv;
        suiv = lst_states->suiv;
    }
    if (suiv != NULL && (suiv->s)->num == num) { // déjà présent
        free(new_state);
        return 0;
    }
    lst_states->suiv = new_state; // insertion du nouvel état
    new_state->suiv = suiv;
    return 1;
}

/*
 * Compare deux listes ordonnées d'états lst_s1 et lst_s2.
 */
unsigned char comp_lst_states(LSTSTATES *lst_s1, LSTSTATES *lst_s2) {
    while (lst_s1 != NULL && lst_s2 != NULL) {
        if ((lst_s1->s)->num != (lst_s2->s)->num) {
            return 0;
        }
        lst_s1 = lst_s1->suiv;
        lst_s2 = lst_s2->suiv;
    }
    return (lst_s1 == NULL && lst_s2 == NULL);
}

/*
 * Détermine récursivement l'epsilon cloture de l'état NFA s dans *cloture.
 */
void eps_cloture_single(STATE *s, LSTSTATES **cloture) {
    if (add_state(cloture, s)) {
        if (s->ch == EPSILON || s->ch == SPLIT) {
            eps_cloture_single(s->suiv, cloture);
        }
        if (s->ch == SPLIT) {
            eps_cloture_single(s->suiv2, cloture);
        }
    }
}

/*
 * Renvoie l'epsilon cloture de la liste d'états NFA lst_s.
 */
LSTSTATES *eps_cloture(LSTSTATES *lst_s) {
    LSTSTATES *cloture = NULL;
    while (lst_s != NULL) {
        eps_cloture_single(lst_s->s, &cloture);
        lst_s = lst_s->suiv;
    }
    return cloture;
}

/*
 * Renvoie l'ensemble des états vers lesquels il existe une transition
 * étiquetée par ch depuis un état de lst_state
 */
LSTSTATES *transition(LSTSTATES *lst_s, char ch) {
    LSTSTATES *trans = NULL;
    while (lst_s != NULL) {
        if ((lst_s->s)->ch == ch) {
            add_state(&trans, (lst_s->s)->suiv);
        }
        lst_s = lst_s->suiv;
    }
    return trans;
}
    

/*
 * Créé en renvoie un nouvel état DFA correspondant à la liste d'états
 * NFA lst_states, numéroté par nb_states qui est incrémenté.
 */
DSTATE *new_dfa_state(LSTSTATES *lst_states, int *nb_states) {
    DSTATE *new_dstate = malloc(sizeof(DSTATE));
    new_dstate->lst_states = lst_states;
    new_dstate->num = (*nb_states)++;
    new_dstate->accept = 0;
    while (lst_states != NULL) {
        if ((lst_states->s)->ch == ACCEPT) {
            new_dstate->accept = 1;
            break;
        }
        lst_states = lst_states->suiv;
    }
    new_dstate->suiv = NULL;
    return new_dstate;
}

/*
 * Renvoie le numéro d'un état DFA correspondant à la liste d'états NFA
 * lst. Si aucun ne correspond, renvoie -1.
 */
int num_state(LSTSTATES *lst, DSTATE *dstate) {
    while (dstate != NULL) {
        if (comp_lst_states(lst, dstate->lst_states)) {
            return dstate->num;
        }
        dstate = dstate->suiv;
    }
    return -1;
} 

/*
 * Construit un DFA en partant du NFA nfa, et renvoie son état initial.
 */
DSTATE *nfa2dfa(NFA nfa) {
    int nb_states = 0;
    DSTATE *head = new_dfa_state(NULL, &nb_states); // état 0 : état puits
    LSTSTATES *lst_states = NULL;
    eps_cloture_single(nfa.start, &lst_states);
    DSTATE *cur = new_dfa_state(lst_states, &nb_states);
    head->suiv = cur;
    DSTATE *tail = cur;
    while (cur != NULL) {
        /***
        printf("traitement de l'état %d (", cur->num);
        for (LSTSTATES *tmp=cur->lst_states; tmp!=NULL; tmp=tmp->suiv) {
            printf("%d ", (tmp->s)->num);
        }
        printf(")\n");
        ***/
        for (char ch=next_letter(0); ch != -1; ch=next_letter(ch)) {
            int i_char = letter_rank(ch);
            LSTSTATES *lst = transition(cur->lst_states, ch);
            lst = eps_cloture(lst);
            int num = num_state(lst, head);
            if (num == -1) {
                num = nb_states;
                /***
                printf("transition %c : Création de l'état %d (", ch, num);
                for (LSTSTATES *tmp=lst; tmp!=NULL; tmp=tmp->suiv) {
                    printf("%d ", (tmp->s)->num);
                }
                printf(")\n");
                ***/
                tail->suiv = new_dfa_state(lst, &nb_states);
                tail = tail->suiv;
            }
            (cur->trans)[i_char] = num;
        }
        cur = cur->suiv;
    }
    return head;
}

/*
 * Créée un fichier name et y écrit les consignes pour dessiner
 * l'automate dfa avec dot.
 */
void dfa2file(DSTATE dfa, char *name){
    FILE *fd = fopen(name, "w");
    if (fd == NULL) {
        fprintf(stderr, "Erreur à la création du %s.\n", name);
        perror("dfa2file");
        exit(EXIT_FAILURE);
    }    
    fprintf(fd, "digraph T {\n");
    fprintf(fd, "node [shape=circle];\n");
    fprintf(fd, "\"\" [shape=none]\n");
    DSTATE *cur = dfa.suiv; // On saute l'état puits
    fprintf(fd, "\"\" -> %d\n", cur->num); // État initial 
    while (cur != NULL) {
        if (cur->accept) fprintf(fd, "%d [shape=doublecircle];\n", cur->num);
        for (char ch=next_letter(0); ch != -1; ch=next_letter(ch)) {
            int i_char = letter_rank(ch);
            if ((cur->trans)[i_char] != 0) {
                fprintf(fd, "%d -> %d [label=%c]\n",
                        cur->num, (cur->trans)[i_char], ch);
            }
        }
        cur = cur->suiv;
    }
    fprintf(fd, "}\n");
    fclose(fd);
}
