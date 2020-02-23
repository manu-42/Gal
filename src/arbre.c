#include <stdio.h>
#include <stdlib.h>
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
            if (isEmpty(p)) {
                fprintf(stderr, "Erreur de parenthèse");
                exit(EXIT_FAILURE);
            }
            else {
                pop(&p);
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
        if (sommet_char(p) == '(') {
            fprintf(stderr, "Erreur de parenthèse");
            exit(EXIT_FAILURE);
        }
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
 * affiche l'arbe en infixe.
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
 * Écrit dans le fichier fd les consignes pour dessiner l'abre avec dot.
 */
void tree2file(TREE tree, char *name) {
    FILE *fd = fopen(name, "w");
    if (fd == NULL) {
        fprintf(stderr, "Erreur à la création du %s.\n", name);
        perror("tree2file");
        exit(EXIT_FAILURE);
    }    
    int num_node = 0;
    fprintf(fd, "graph T {\n");
    fprintf(fd, "node [shape=circle];\n");
    tree2file_rec(fd, tree, &num_node);
    fprintf(fd, "}\n");
    fclose(fd);
}

/*
 * Détermine l'arbre syntaxique correspondant à une expression src en
 * notation postfixée.
 */
void to_tree(char *src, TREE *tree) {
    if (src[0] == '\0') { // langage vide
        tree = NULL;
        return;
    }
    PILE p = create_PILE();
    for (int i=0; src[i] != '\0'; i++) {
        char ch = src[i];
        TREE tr = malloc(sizeof(NODE));
        tr->val = ch;
        if (letter_rank(ch)!=-1 || ch==EPSILON) { // feuille
            tr->left = NULL;
            tr->right = NULL;
        }
        else if (ch == '*') { // opérateur unaire
            tr->left = pop_tree(&p);
            tr->right = NULL;
        }
        else { // opérateur binaire
            tr->right = pop_tree(&p);
            tr->left = pop_tree(&p);
        }
        push_tree(&p, tr);      
    }
    *tree = pop_tree(&p);
    if (!isEmpty(p)) {
        fprintf(stderr, "Expression mal construite :\n %s", src);
        exit(EXIT_FAILURE);
    }
}

