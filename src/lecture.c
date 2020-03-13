#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> /* open */
#include <unistd.h>
#include <string.h>
#include "alphabet.h"
#include "lecture.h"

/*
 * Prend en paramètre le chemin complet vers un fichier, et renvoie le
 * nom du fichier en enlevant le chemin et l'extension.
 */
char *get_filename(char *fullpath) {
    int istart=0, iend=strlen(fullpath)-1;
    for (int i=iend; i>=0; i--) {
        if (fullpath[i] == '/') {
            istart = i+1;
            break;
        }
    }
    for (int i=iend; i>=istart; i--) {
        if (fullpath[i] == '.') {
            iend = i-1;
            break;
        }
    }
    int len = (iend-istart+1);
    char *filename = malloc(sizeof(char) * (len+1));
    for (int i=0; i<len; i++) {
        filename[i] = fullpath[istart+i];
    }
    filename[len] = '\0';
    return filename;
}

/*
 * Indique si le caractère `ch` est acceptable.
 */
unsigned char is_accepted(char ch) {
    return letter_rank(ch)!=-1 || ch=='|' || ch=='*' || ch=='(' || ch==')';
}

/*
 * Lit le fichier `nom` contenant une expression régulière, et renvoie
 * son contenu.
 * Si le contenu du fichier ne finit pas par \n ou qu'il contient un
 * caractère non reconnu, affiche un message d'erreur et termine le
 * programme.
 */
char *lecture(char *nom) {
    FILE *fp = fopen(nom, "r");
    if (!fp) {
        perror("lecture");
        exit(EXIT_FAILURE);
    }
    int c, size = 0, max_size = 2;
    char *exp = malloc(max_size * sizeof(char));
    while ((c = getc(fp)) != EOF) {
        if (size >= max_size) {
            max_size *= 2;
            exp = realloc(exp, max_size);
            if (! exp) {
                fprintf(stderr, "Expression trop longue, plus de mémoire.\n");
                exit(EXIT_FAILURE);
            }
        }
        exp[size++] = c;
        if (c == '\n') break;
        if (! is_accepted(c)) {
            fprintf(stderr, "Caractère non reconnu '%c'.\n", c);
            exit(EXIT_FAILURE);
        }
    }
    fclose(fp);
    if (size <= 0 || exp[size-1] != '\n') {
        fprintf(stderr, "Problème dans le fichier %s.\n", nom);
        exit(EXIT_FAILURE);
    }
    exp[size-1] = '\0';
    exp = realloc(exp, size);
    return exp;
}

/*
 * Ajoute des points à la chaîne `src` aux endroits des concaténations
 * (par exemple ab devient a.b), remplace les couples de parenthèses
 * vides () par EPSILON et renvoie le résultat.
 */
char *add_concat(char *src) {
    char *dest = malloc(strlen(src)*2*sizeof(char));
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
    dest = realloc(dest, i_dest+1);
    return dest;
}
