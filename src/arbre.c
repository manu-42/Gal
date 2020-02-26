#include <stdio.h>
#include <stdlib.h>
#include "alphabet.h"
#include "pile.h"
#include "arbre.h"

/*
 * Renvoie la priorité d'un opérateur.
 */
int priority(char op) {
    switch (op) {
        case '*' : return 3;
        case '.' : return 2;
        case '|' : return 1;
        case '(' : return 0;
    }
    fprintf(stderr, "Erreur : %c n'est pas un opérateur\n", op);
    exit(EXIT_FAILURE);
    return -1;
}


/*
 * En cas d'expression mal construite, affiche un message d'erreur et
 * quitte.
 */
void erreur_exp(char* msg, char *src) {
    fprintf(stderr, "Problème de %s : '%s'\n", msg, src);
    exit(EXIT_FAILURE);
}

/*
 * Détermine l'écriture postfixe en fonction d'une entrée
 * en écriture infixe (algorithme de Shunting-yard).
 */
void to_postfix(char *entry, char *postfix) {
    PILE p = create_PILE();
    int i_pf = 0;
    for (int i=0; entry[i] != '\0'; i++) {
        char ch = entry[i];
        if (letter_rank(ch)!=-1 || ch==EPSILON) postfix[i_pf++] = ch;
        else if (ch == '(') push_char(&p, ch);
        else if (ch == ')') {
            while (!isEmpty(p) && sommet_char(p) != '('){
                postfix[i_pf++] = pop_char(&p);
            }
            if (isEmpty(p)) erreur_exp("parenthèse", entry);
            else {
                pop(&p);  // dépiler '('
            }
        }
        else { // opérateur
            while (!isEmpty(p) && priority(sommet_char(p)) >= priority(ch)) {
                postfix[i_pf++] = pop_char(&p);
            }
            push_char(&p, ch);
        }
    }
    while (!isEmpty(p)) {
        if (sommet_char(p) == '(') erreur_exp("parenthèse", entry);
        postfix[i_pf++] = pop_char(&p);
    }
    postfix[i_pf] = '\0';
}

/*
 * Empile un arbre.
 */
void push_tree(PILE *p, TREE tr) {
    TREE *val = malloc(sizeof(TREE));
    *val = tr;
    push(p, val);
}

/*
 * Dépile un arbre.
 */
TREE pop_tree(PILE *p) {
    void *v = pop(p);
    TREE tr = *((TREE*) v);
    free(v);
    return tr;
}

/*
 * Renvoie l'arbre au sommet de la pile.
 */
TREE sommet_tree(PILE p) {
    return *((TREE*) sommet(p));
}

/*
 * affiche l'arbe en notation infixe.
 */
void print_infixe(TREE tree) {
    if (tree == NULL) return;
    printf("(");
    print_infixe(tree->left);
    printf("%c", tree->val);
    print_infixe(tree->right);
    printf(")");
}

/*
 * Fonction récursive pour écrire dans le fichier fd les consignes pour
 * dessiner l'arbre avec dot.
 * num_node permet de numéroter les neouds.
 */
int tree2file_rec(FILE *fd, TREE tree, int *num_node) {
    if (tree == NULL) return -1;
    int num_cur = (*num_node)++;
    fprintf(fd, "%d [label=\"%c\"];\n", num_cur, tree->val);
    int num_left = tree2file_rec(fd, tree->left, num_node);
    if (num_left >= 0) fprintf(fd, "%d -- %d;\n", num_cur, num_left);
    int num_right = tree2file_rec(fd, tree->right, num_node);
    if (num_right >= 0) fprintf(fd, "%d -- %d;\n", num_cur, num_right);
    return num_cur;
}

/*
 * Créée un fichier nommé `name` et y écrit les consignes pour dessiner
 * l'arbre associé à l'expression `exp` avec dot.
 */
void tree2file(TREE tree, char *name, char *exp) {
    FILE *fd = fopen(name, "w");
    if (fd == NULL) {
        fprintf(stderr, "Erreur à la création du %s.\n", name);
        perror("tree2file");
        exit(EXIT_FAILURE);
    }
    int num_node = 0;
    fprintf(fd, "graph T {\n");
    fprintf(fd, "labelloc=\"t\";\n");
    fprintf(fd, "label=\"%s\";\n", exp);
    fprintf(fd, "node [shape=circle];\n");
    tree2file_rec(fd, tree, &num_node);
    fprintf(fd, "}\n");
    fclose(fd);
}

/*
 * Renvoie l'arbre syntaxique correspondant à une expression src en
 * notation polonaise inversée.
 */
TREE npi_to_tree(char *src) {
    if (src[0] == '\0') return NULL;  // langage vide
    PILE p = create_PILE();
    for (int i=0; src[i] != '\0'; i++) {
        char ch = src[i];
        NODE *nd = malloc(sizeof(NODE));
        nd->val = ch;
        if (letter_rank(ch)!=-1 || ch==EPSILON) { // feuille
            nd->left = NULL;
            nd->right = NULL;
        }
        else if (ch == '*') { // opérateur unaire
            if (isEmpty(p)) erreur_exp("syntaxe", src);
            nd->left = pop_tree(&p);
            nd->right = NULL;
        }
        else { // opérateur binaire
            if (isEmpty(p)) erreur_exp("syntaxe", src);
            nd->right = pop_tree(&p);
            if (isEmpty(p)) erreur_exp("syntaxe", src);
            nd->left = pop_tree(&p);
        }
        push_tree(&p, nd);
    }
    TREE tree = pop_tree(&p);
    if (!isEmpty(p)) erreur_exp("syntaxe", src);
    return tree;
}

