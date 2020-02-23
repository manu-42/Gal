#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> /* open */
#include <unistd.h>
#include "lecture.h"

/*
 * Indique si le caractère ch est acceptable.
 */
unsigned char is_accepted(char ch) {
    return letter_rank(ch)!=-1 || ch=='|' || ch=='*' || ch=='(' || ch==')';
}

/*
 * Lit le fichier nom contenant une expression régulière
 * et la stocke dans exp.
 */
void lecture(char *nom, char *exp) {
    int fd = open(nom, O_RDONLY);
    if (fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    int size = read(fd, exp, MAX_SIZE_EXP-1);
    close(fd);
    if (size <= 0 || exp[size-1] != '\n') {
        fprintf(stderr, "Problème dans le fichier %s.\n", nom);
        exit(EXIT_FAILURE);
    }
    for (int i=0; i<size-1; i++) {
        if (! is_accepted(exp[i])) {
            fprintf(stderr, "Caractère non reconnu '%c'.\n", exp[i]);
            exit(EXIT_FAILURE);
        }
    }
    exp[size-1] = '\0';
}

/*
 * ajoute des points à la chaine source aux endroits des concaténations
 * (par exemple : ab devient a.b), remplace les pairs () par le mot vide
 * et stocke le résultat dans dest.
 */
void add_concat(char *src, char *dest) {
    dest[0] = src[0];
    int i_dest = 1;
    for (int i=1; src[i] != '\0'; i++) {
        char ch = src[i];
        if (ch == ')' && src[i-1] == '(') {
            i_dest--;
            ch = EPSILON;
        }
        if (    ( letter_rank(ch)!=-1 || ch==EPSILON || ch == '(')
             && src[i-1] != '|'
             && src[i-1] != '('
           ) {
            dest[i_dest++] = '.';
        }
        dest[i_dest++] = ch;
    }
    dest[i_dest] = '\0';
}
