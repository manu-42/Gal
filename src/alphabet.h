#ifndef ISLETTER_H
#define ISLETTER_H

#define EPSILON '_'  // Le mot vide
#define ALPHABET_LEN 62  // Nombre de lettres de l'alphabet

/*
 * Renvoie
 *     la première lettre de l'alphabet si `ch` = 0
 *     la lettre suivant `ch` dans l'alphabet s'il y en a une
 *     -1 s'il n'y en a plus.
 */
char next_letter(char ch);

/*
 * Renvoie le rang de la lettre ch dans l'alphabet, ou -1 si ce n'est
 * pas une lettre de l'alphabet.
 */
int letter_rank(char ch);

#endif
