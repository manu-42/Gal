#include <stdio.h>
#include <stdlib.h>
#include "alphabet.h"
#include "nfa.h"

#define NOT_VISITED -1

/*
 * Créé et renvoie un état.
 */
STATE *state(char ch, STATE *suiv, STATE *suiv2) {
    STATE *s = malloc(sizeof(STATE));
    s->num = NOT_VISITED;
    s->ch = ch;
    s->suiv = suiv;
    s->suiv2 = suiv2;
    return s;
}

/*
 * Créé et renvoie un nfa.
 */
NFA nfa(STATE *start, STATE *end) {
    NFA new_nfa;
    new_nfa.start = start;
    new_nfa.end = end;
    return new_nfa;
}

/*
 * Renvoie un NFA en partant de l'arbre syntaxique tree
 */
NFA tree2nfa(TREE tree) {
    STATE *s;
    STATE *e=state(ACCEPT, NULL, NULL); // l'état final
    NFA res, f, g; // on construit res en partant de f et g
    if (tree == NULL) { // langage vide
        STATE *s = malloc(sizeof(STATE));
        s->num = NOT_VISITED;
        s->ch = EPSILON;
        s->suiv = s;
        s->suiv2 = NULL;
        return nfa(s, e);
    }
    switch (tree->val) {
        default: // une lettre de l'alphabet ou epsilon
            s = state(tree->val, e, NULL);
            res = nfa(s, e);
            break;
        case '|':
            f = tree2nfa(tree->left);
            g = tree2nfa(tree->right);
            s = state(SPLIT, f.start, g.start);
            (f.end)->ch = EPSILON;
            (f.end)->suiv = e;
            (g.end)->ch = EPSILON;
            (g.end)->suiv = e;
            res = nfa(s, e);
            break;
        case '.':
            f = tree2nfa(tree->left);
            g = tree2nfa(tree->right);
            s = f.end;
            *(f.end) = *(g.start);
            free(g.start);
            res = nfa(f.start, g.end);
            break;
        case '*':
            f = tree2nfa(tree->left);
            s = state(SPLIT, f.start, e);
            (f.end)->ch = SPLIT;
            (f.end)->suiv = f.start;
            (f.end)->suiv2 = e;
            res = nfa(s, e);
            break;
    }
    return res;
}

/*
 * Procédure récursive pour écrire dans le fichier fd les consignes pour
 * dessiner l'état avec dot.
 * num_state permet de numéroter les noeuds, et d'éviter de traiter
 *           plusieurs fois le même noeud en cas de boucle.
 */
void state2file(FILE *fd, STATE *s, int *num_state) {
    if (s == NULL || s->num != NOT_VISITED) return;
    s->num = (*num_state)++;
    char ch = s->ch;
    if (ch == ACCEPT) { // pas de successeur
        fprintf(fd, "%d [shape=doublecircle];\n", s->num);
        return;
    }
    state2file(fd, s->suiv, num_state); // au moins un successeur
    if (ch == SPLIT || ch == EPSILON) {
        fprintf(fd, "%d -> %d;\n", s->num, (s->suiv)->num);
        if (ch == EPSILON) return;
        state2file(fd, s->suiv2, num_state);
        fprintf(fd, "%d -> %d;\n", s->num, (s->suiv2)->num);
        return;
    }
    // cas "général" : une transition étiqutée par ch
    fprintf(fd, "%d -> %d [label = %c];\n", s->num, (s->suiv)->num, ch);
}

/*
 * Créée un fichier nommé `name` et y écrit les consignes pour dessiner
 * le nfa avec dot.
 */
void nfa2file(NFA nfa, char *name) {
    FILE *fd = fopen(name, "w");
    if (fd == NULL) {
        fprintf(stderr, "Erreur à la création du %s.\n", name);
        perror("nfa2file");
        exit(EXIT_FAILURE);
    }
    fprintf(fd, "digraph T {\n");
    fprintf(fd, "node [shape=circle];\n");
    fprintf(fd, "\"\" [shape=none]\n");
    int num_state = 0;
    state2file(fd, nfa.start, &num_state);
    fprintf(fd, "\"\" -> %d;\n", (nfa.start)->num);  // état initial
    state2file(fd, nfa.end, &num_state);  // si l'état acceptant est isolé
    fprintf(fd, "}\n");
    fclose(fd);
}
