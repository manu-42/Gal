#ifndef GAL_H
#define GAL_H

#include "dfa_min.h"

/*
 * Écriture du code source de l'analyseur lexicale de l'expression
 * régulière `exp` dans le fichier nommé `name`.
 */
void gal(DFAMIN dfa, char *name, char *exp);

#endif
