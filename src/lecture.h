#ifndef H_LECTURE
#define H_LECTURE

#include "alphabet.h"

/*
 * Taille maximale de l'expression.
 */
#define MAX_SIZE_EXP 1000

/*
 * Lit le fichier nom contenant une expression régulière et la stocke
 * dans exp.
 */
void lecture(char *nom, char *exp);

/*
 * ajoute des points à la chaine source aux endroits des concaténations
 * (par exemple : ab devient a.b) et stocke le résultat dans dest.
 */
void add_concat(char *src, char *dest);

#endif