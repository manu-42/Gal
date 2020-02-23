#include "alphabet.h"

/*
 * Renvoie la lettre de rang i de l'alphabet.
 */
char letter(int i) {
     if (i < 26) return 'a' + i;
     if (i < 52) return 'A' + (i-26);
     return '0' + (i-52);
}

/*
 * Renvoie
 *     la première lettre de l'alphabet si ch = 0
 *     la lettre suivant ch dans l'alphabet s'il y en a une
 *     -1 s'il n'y en a plus.
 */
char next_letter(char ch) {
     switch (ch) {
          case '\0': return 'a';
          case 'z' : return 'A';
          case 'Z' : return '0';
          case '9' : return -1;
          default : return ++ch;
     }
}

/*
 * Renvoie le rang de la lettre ch dans l'alphabet, ou -1 si ce n'est
 * pas une lettre de l'alphabet.
 */
int letter_rank(char ch) {
     if ('a' <= ch && ch <= 'z') return ch - 'a';
     if ('A' <= ch && ch <= 'Z') return ch - 'A' + 26;
     if ('0' <= ch && ch <= '9') return ch - '0' + 52;
     return -1;
}
