#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lecture.h"
#include "arbre.h"
#include "nfa.h"
#include "dfa.h"


/***/
#include "alphabet.h"
/***/

/*
 * Renvoie le nom d'un fichier en enlevant le chemin et l'extension.
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

void usage(char *nom) {
    fprintf(stderr, "Usage : %s exp\n", nom);
    fprintf(stderr, "    exp est un fichier contenant une expression"
            " rationnelle sur une ligne, finissant par \\n.\n");
}

int main(int argc, char **args) {
    if (argc != 2) {
        usage(args[0]);
        exit(1);
    }
    char exp[MAX_SIZE_EXP];
    lecture(args[1], exp);
    printf("expression lue '%s'\n", exp);
    char *filename = get_filename(args[1]);
    char exp2[MAX_SIZE_EXP];
    add_concat(exp, exp2);
    printf("après modif '%s'\n", exp2);
    char postfix[MAX_SIZE_EXP];
    to_postfix(exp2, postfix);
    printf("Postfix : '%s'\n", postfix);

    printf("Conversion en arbre ...\n");
    TREE *tree = malloc(sizeof(TREE));
    to_tree(postfix, tree);
    char *extname = malloc(sizeof(char) * (strlen(filename)+5));
    sprintf(extname, "%s.tree", filename); 
    printf("Création de %s_tree.pdf ...\n", filename);
    tree2file(*tree, extname);
    char *dotstring = malloc(sizeof(char) * (2*strlen(extname)+30));
    sprintf(dotstring, "dot -T pdf -o %s_tree.pdf %s", filename, extname);
    system(dotstring);

    printf("Conversion en NFA ...\n");
    NFA *nfa;
    nfa = tree2nfa(*tree);
    sprintf(extname, "%s.nfa", filename); 
    printf("Création de %s_nfa.pdf ...\n", filename);
    nfa2file(*nfa, extname);
    sprintf(dotstring, "dot -T pdf -o %s_nfa.pdf %s", filename, extname);
    system(dotstring);

    printf("Conversion en DFA ...\n");
    DSTATE *dfa = nfa2dfa(*nfa);
    sprintf(extname, "%s.dfa", filename); 
    printf("Création de %s_dfa.pdf ...\n", filename);
    dfa2file(*dfa, extname);
    sprintf(dotstring, "dot -T pdf -o %s_dfa.pdf %s", filename, extname);
    system(dotstring);
}


