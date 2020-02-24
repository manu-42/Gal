#ifndef ARBRE_H
#define ARBRE_H

/*
 * un noeud de l'arbre.
 * ch désigne l'étiquette du noeud
 * left et right les fils gauche et droit.
 */
typedef struct node  {
    char val;
    struct node *left, *right;
}NODE, *TREE;

/*
 * Détermine l'écriture postfixe en fonction d'une entrée
 * en écriture infixe (algorithme de Shunting-yard).
 */
void to_postfix(char *entry, char *postfix);

/*
 * Renvoie l'arbre syntaxique correspondant à une expression src en
 * notation polonaise inversée.
 */
TREE npi_to_tree(char *src);

/*
 * Créée un fichier nommé `name` et y écrit les consignes pour dessiner
 * l'abre avec dot.
 */
void tree2file(TREE tree, char *name);

/*
 * affiche l'arbe en infixe.
 */
void print_infixe(TREE tree);

#endif
